#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mesh.h"
#include "solver.h"
#include "operator.h"
#include "linalg.h"

#include <time.h>

/* timing */
static double gmres_total_seconds = 0.0;
static int    gmres_call_count = 0;

void solver_timing_reset(void) {
    gmres_total_seconds = 0.0;
    gmres_call_count = 0;
}

void solver_timing_get(double *total_seconds, int *calls) {
    if (total_seconds) *total_seconds = gmres_total_seconds;
    if (calls) *calls = gmres_call_count;
}

int gmres_solve(double *x,
                const double *b,
                Mesh *mesh,
                GMRESParams params,
                int step)
{
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    gmres_call_count++;

    int nx = mesh->nx_global;
    int ny = mesh->ny_global;
    int N  = nx * ny;

    double *r  = calloc(N, sizeof(double));
    double *p  = calloc(N, sizeof(double));
    double *Ap = calloc(N, sizeof(double));

    if (!r || !p || !Ap) {
        printf("Memory allocation failed in GMRES\n");
        return 1;
    }

    // -------------------------------
    // r = b - A x
    // -------------------------------
    apply_operator(x, r, mesh);

    double r0 = 0.0;

    for (int i = 0; i < N; i++) {
        r[i] = b[i] - r[i];
        r0 += r[i] * r[i];
    }

    r0 = sqrt(r0);

    // -------------------------------
    // SAFETY: trivial or invalid system
    // -------------------------------
    if (!isfinite(r0) || r0 < 1e-14) {
        // printf("GMRES: trivial or invalid initial residual (r0=%e)\n", r0);
        free(r); free(p); free(Ap);
        return 1;
    }

    FILE *residual_fp = NULL;
    if (GMRES_Residual_Plot) {
        char residual_path[256];
        int rc = snprintf(residual_path, sizeof(residual_path),
                          "serial_vtk/GMRES/gmres_residual_%04d.dat", step);
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

    // -------------------------------
    // initial search direction
    // -------------------------------
    for (int i = 0; i < N; i++)
        p[i] = r[i];

    double rsold = r0 * r0;

    // -------------------------------
    // ITERATION LOOP
    // -------------------------------
    for (int iter = 0; iter < params.max_iter; iter++) {

        apply_operator(p, Ap, mesh);

        double denom = 0.0;

        for (int i = 0; i < N; i++)
            denom += p[i] * Ap[i];

        // -------------------------------
        // breakdown detection
        // -------------------------------
        if (!isfinite(denom) || fabs(denom) < 1e-14) {
            // printf("GMRES breakdown at iter %d (denom=%e)\n", iter, denom);
            free(r); free(p); free(Ap);
            return 1;
        }

        double alpha = rsold / denom;

        // -------------------------------
        // update solution + residual
        // -------------------------------
       
        double rsnew = 0.0;
        int nan_detected = 0;

        for (int i = 0; i < N; i++) {

            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];

            if (!isfinite(r[i])) {
                nan_detected = 1;
            }

            rsnew += r[i] * r[i];
        }

        if (nan_detected) {
            // printf(" NaN detected in residual at iter %d\n", iter);
            if (residual_fp) fclose(residual_fp);
            free(r); free(p); free(Ap);
            return 1;
        }

        rsnew = sqrt(rsnew);
        double rel = rsnew / r0;

        if (residual_fp) {
            fprintf(residual_fp, "%d %.16e %.16e\n", iter + 1, rsnew, rel);
        }

        // -------------------------------
        // progress output
        // -------------------------------
        if (iter % 10 == 0) {
            //printf("    GMRES iter %d | rel residual = %.3e\n", iter, rel);
        }

        // -------------------------------
        // convergence check
        // -------------------------------
        if (rel < params.tol) {
            //printf("✔ GMRES converged in %d iterations (rel=%.3e)\n", iter, rel);
            if (residual_fp) fclose(residual_fp);
            free(r); free(p); free(Ap);
            clock_gettime(CLOCK_MONOTONIC, &t1);
            gmres_total_seconds += (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)*1e-9;
            return 0;
        }

        double beta = (rsold > 1e-14) ? (rsnew * rsnew / rsold) : 0.0;

        for (int i = 0; i < N; i++)
            p[i] = r[i] + beta * p[i];

        rsold = rsnew * rsnew;
    }

    // printf("GMRES did not converge within max_iter=%d\n", params.max_iter);

    free(r);
    free(p);
    free(Ap);

    if (residual_fp) fclose(residual_fp);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    gmres_total_seconds += (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)*1e-9;

    return 1;
}


static void __attribute__((unused)) gmres_record_success_time(struct timespec t0) {
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    gmres_total_seconds += (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)*1e-9;
}