#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#include "mesh_mpi.h"
#include "operator_mpi.h"
#include "solver_mpi.h"

/* ======================================================
   MPI GMRES SOLVER IMPLEMENTATION
   ====================================================== */
/* --------------------------------------------------
    Timing support
    -------------------------------------------------- */
static double gmres_total_seconds = 0.0;
static int gmres_calls = 0;

static inline int i_owned_start(const MPIMesh *mesh)
{
    return (mesh->rank_left == MPI_PROC_NULL) ? 0 : 1;
}

static inline int i_owned_end(const MPIMesh *mesh)
{
    return (mesh->rank_right == MPI_PROC_NULL) ? (mesh->nx_local - 1) : (mesh->nx_local - 2);
}


int gmres_solve_mpi(double *x,
                    const double *b,
                    MPIMesh *mesh,
                    GMRESParams params,
                    int step,
                    MPI_Comm comm)
{
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    int rank, nprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nprocs);

    int nx = mesh->nx_local;
    int ny = mesh->ny_local;
    int N  = nx * ny;
    int i_start = i_owned_start(mesh);
    int i_end = i_owned_end(mesh);

    double *r  = calloc(N, sizeof(double));
    double *p  = calloc(N, sizeof(double));
    double *Ap = calloc(N, sizeof(double));

    if (!r || !p || !Ap) {
        fprintf(stderr, "GMRES MPI: allocation failed\n");
        MPI_Abort(comm, 1);
    }

    /* ======================================================
       INITIAL RESIDUAL: r = b - A x
       ====================================================== */
    exchange_halo(x, mesh);
    apply_operator_mpi(x, r, mesh);

    double r0_local = 0.0, r0;

    for (int i = 0; i < N; i++) {
        r[i] = b[i] - r[i];
    }

    for (int i = i_start; i <= i_end; i++) {
        for (int j = 0; j < ny; j++) {
            int id = i * ny + j;
            r0_local += r[id] * r[id];
        }
    }

    MPI_Allreduce(&r0_local, &r0, 1, MPI_DOUBLE, MPI_SUM, comm);
    r0 = sqrt(r0);

    if (!isfinite(r0) || r0 < 1e-14) {
        // if (rank == 0)
        //     printf("GMRES MPI: trivial system (r0=%e)\n", r0);
        free(r); free(p); free(Ap);
        return 0;
    }

    /* optional residual output for plotting (rank 0 only) */
    FILE *residual_fp = NULL;
    if (GMRES_Residual_Plot && rank == 0) {
        char residual_path[256];
        int rc = snprintf(residual_path, sizeof(residual_path),
                          "vtk_mpi/t%d/GMRES/gmres_residual_%04d.dat", nprocs, step);
        if (rc > 0 && (size_t)rc < sizeof(residual_path)) {
            residual_fp = fopen(residual_path, "w");
            if (residual_fp) {
                fprintf(residual_fp, "# step %d\n", step);
                fprintf(residual_fp, "# iter abs_residual rel_residual\n");
                fprintf(residual_fp, "%d %.16e %.16e\n", 0, r0, 1.0);
            } else {
                printf("Warning: could not open %s for GMRES residual output\n", residual_path);
            }
        }
    }

    /* initial search direction */
    for (int i = 0; i < N; i++)
        p[i] = r[i];

    double rsold = r0 * r0;

    /* ======================================================
       GMRES LOOP
       ====================================================== */
    for (int iter = 0; iter < params.max_iter; iter++)
    {
        /* --- apply operator on search direction --- */
        exchange_halo(p, mesh);
        apply_operator_mpi(p, Ap, mesh);

        /* --- compute dot product p·Ap --- */
        double denom_local = 0.0, denom;

        for (int i = i_start; i <= i_end; i++) {
            for (int j = 0; j < ny; j++) {
                int id = i * ny + j;
                denom_local += p[id] * Ap[id];
            }
        }

        MPI_Allreduce(&denom_local, &denom, 1, MPI_DOUBLE, MPI_SUM, comm);

        if (!isfinite(denom) || fabs(denom) < 1e-14) {
            // if (rank == 0)
            //     printf("GMRES breakdown at iter %d\n", iter);
            if (residual_fp) fclose(residual_fp);
            free(r); free(p); free(Ap);
            return 1;
        }

        double alpha = rsold / denom;

        /* --- update solution and residual on owned physical cells --- */
        double rsnew_local = 0.0, rsnew;

        for (int i = i_start; i <= i_end; i++) {
            for (int j = 0; j < ny; j++) {

                int id = i * ny + j;

                x[id] += alpha * p[id];
                r[id] -= alpha * Ap[id];

                rsnew_local += r[id] * r[id];
            }
        }

        MPI_Allreduce(&rsnew_local, &rsnew, 1, MPI_DOUBLE, MPI_SUM, comm);

        rsnew = sqrt(rsnew);
        double rel = rsnew / r0;

        if (residual_fp && rank == 0) {
            fprintf(residual_fp, "%d %.16e %.16e\n", iter + 1, rsnew, rel);
        }

           if (rank == 0 && iter % 50 == 0) {
               // printf("GMRES iter %d | rel residual = %.3e\n", iter, rel);
           }

        /* --- convergence check --- */
        if (rel < params.tol) {
            // if (rank == 0)
                // printf("✔ GMRES converged in %d iterations\n", iter);
            if (residual_fp) {
                fclose(residual_fp);
                residual_fp = NULL;
            }
            break;
        }

        /* --- update search direction --- */
        double beta = (rsold > 1e-14) ? (rsnew * rsnew / rsold) : 0.0;

        for (int i = i_start; i <= i_end; i++) {
            for (int j = 0; j < ny; j++) {
                int id = i * ny + j;
                p[id] = r[id] + beta * p[id];
            }
        }

        rsold = rsnew * rsnew;

        /* ======================================================
           update halo AFTER modifying x
           ====================================================== */
        exchange_halo(x, mesh);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    /* accumulate solver time */
    gmres_total_seconds += (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)*1e-9;
    gmres_calls++;

    if (residual_fp) fclose(residual_fp);

    free(r);
    free(p);
    free(Ap);

    return 0;
}

void solver_timing_reset(void) {
    gmres_total_seconds = 0.0;
    gmres_calls = 0;
}

void solver_timing_get(double *total_seconds, int *calls) {
    if (total_seconds) *total_seconds = gmres_total_seconds;
    if (calls) *calls = gmres_calls;
}