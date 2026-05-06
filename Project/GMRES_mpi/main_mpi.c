#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#include "initialize_mpi.h"
#include "utils_mpi.h"
#include "vtk_mpi.h"

int GMRES_Residual_Plot = 1;  /* Set to 1 to enable residual output for plotting */

void exchange_all(Simulation *sim)
{
    exchange_halo(sim->u, sim->mesh);
    exchange_halo(sim->v, sim->mesh);
    exchange_halo(sim->p, sim->mesh);
}

// =======================================================
// BOUNDARY CONDITIONS (PREDICTOR STEP)
// =======================================================

void apply_bc_predictor_mpi(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    double U_in = 0.01;

    /* -----------------------------------
       INLET (global left boundary)
       ----------------------------------- */
    if (m->rank == 0) {

        int i = 0;  // local left boundary ghost/physical edge

        for (int j = 0; j < ny; j++) {

            int id = i * ny + j;

            sim->u[id] = U_in;
            sim->v[id] = 0.0; 
        }
    }

    /* -----------------------------------
       OUTLET (global right boundary)
       ----------------------------------- */
    if (m->rank_right == MPI_PROC_NULL) {

        int i = nx - 1;

        for (int j = 0; j < ny; j++) {

            int id = i * ny + j;
            int im = (i - 1) * ny + j;

            sim->u[id] = sim->u[im];
            sim->v[id] = sim->v[im];
        }
    }

    /* -----------------------------------
       WALLS
       ----------------------------------- */
    for (int i = 0; i < nx; i++) {

        // bottom wall
        int j0 = 0;
        int id0 = i * ny + j0;

        sim->u[id0] = 0.0;
        sim->v[id0] = 0.0;

        // top wall
        int j1 = ny - 1;
        int id1 = i * ny + j1;

        sim->u[id1] = 0.0;
        sim->v[id1] = 0.0;
    }
}

// =======================================================
// BOUNDARY CONDITIONS (CORRECTOR STEP)
// =======================================================

void apply_bc_corrector_mpi(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    /* -----------------------------------
       OUTLET (right boundary only)
       ----------------------------------- */
    if (m->rank_right == MPI_PROC_NULL) {

        int i = nx - 1;   // global right ghost boundary

        for (int j = 0; j < ny; j++) {

            int id = i * ny + j;
            int im = (i - 1) * ny + j;

            sim->u[id] = sim->u[im];
            sim->v[id] = sim->v[im];
        }
    }

    /* -----------------------------------
       WALLS (apply on ALL ranks)
       ----------------------------------- */
    for (int i = 0; i < nx; i++) {

        // bottom wall
        int j0 = 0;
        int id0 = i * ny + j0;

        sim->u[id0] = 0.0;
        sim->v[id0] = 0.0;

        // top wall
        int j1 = ny - 1;
        int id1 = i * ny + j1;

        sim->u[id1] = 0.0;
        sim->v[id1] = 0.0;
    }

    
}

// =======================================================
// PRESSURE PROFILE FOR OUTPUT
// =======================================================

void update_pressure_profile_mpi(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    double Lx = m->lx;
    double dp = sim->params.dp;

    for (int i = 1; i < nx - 1; i++) {   // skip ghost cells
        int i_global = m->i_start + (i-1);    // FIXED (no -1)
        double x = i_global * m->dx;

        for (int j = 0; j < ny; j++) {

            int id = i * ny + j;
            sim->p[id] = dp * (1.0 - x / Lx);
        }
    }
}

// =======================================================
// INTERMEDIATE VELOCITY STEP
// =======================================================
void compute_intermediate_velocity(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    double dx = m->dx;
    double dy = m->dy;
    double dt = sim->params.dt;
    double nu = sim->params.viscosity;
    double G  = sim->params.dp / m->lx;

    double dx2 = dx * dx;
    double dy2 = dy * dy;

    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {

            int id  = i * ny + j;

            int ip = (i+1) * ny + j;
            int im = (i-1) * ny + j;
            int jp = i * ny + (j+1);
            int jm = i * ny + (j-1);

            double u = sim->u[id];
            double v = sim->v[id];

            double dudx = (sim->u[ip] - sim->u[im]) / (2.0 * dx);
            double dudy = (sim->u[jp] - sim->u[jm]) / (2.0 * dy);

            double dvdx = (sim->v[ip] - sim->v[im]) / (2.0 * dx);
            double dvdy = (sim->v[jp] - sim->v[jm]) / (2.0 * dy);

            /* Laplacian */
            double lap_u =
                (sim->u[ip] - 2.0*u + sim->u[im]) / dx2 +
                (sim->u[jp] - 2.0*u + sim->u[jm]) / dy2;

            double lap_v =
                (sim->v[ip] - 2.0*v + sim->v[im]) / dx2 +
                (sim->v[jp] - 2.0*v + sim->v[jm]) / dy2;

            sim->u_star[id] =
                u
                - dt * (u*dudx + v*dudy)
                + dt * nu * lap_u
                + dt * G;

            sim->v_star[id] =
                v
                - dt * (u*dvdx + v*dvdy)
                + dt * nu * lap_v;
        }
    }
}

// =======================================================
// PRESSURE RHS
// =======================================================

void build_rhs_mpi(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    double dx = m->dx;
    double dy = m->dy;
    double dt = sim->params.dt;

    /* -----------------------------------
       Initialize
       ----------------------------------- */
    memset(sim->b, 0, nx * ny * sizeof(double));

    /* -----------------------------------
       Compute divergence 
       ----------------------------------- */
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {

            int id = i * ny + j;

            int ip = (i + 1) * ny + j;
            int im = (i - 1) * ny + j;
            int jp = i * ny + (j + 1);
            int jm = i * ny + (j - 1);

            double dudx = (sim->u_star[ip] - sim->u_star[im]) / (2.0 * dx);
            double dvdy = (sim->v_star[jp] - sim->v_star[jm]) / (2.0 * dy);

            sim->b[id] = (dudx + dvdy) / dt;
        }
    }

    /* -----------------------------------
       Pressure profile
       ----------------------------------- */
    for (int i = 0; i < nx; i++) {

        double x = get_x(i, m);   // global coordinate

        for (int j = 0; j < ny; j++) {

            int id = i * ny + j;

            sim->b[id] = sim->params.dp * (1.0 - x / m->lx);
        }
    }
}

// =======================================================
// PRESSURE SOLVE
// =======================================================
int solve_pressure_mpi(Simulation *sim, int step)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    /* -----------------------------------
       Solve pressure (MPI version)
       ----------------------------------- */
    gmres_solve_mpi(sim->p, sim->b, m, sim->gmres, step, m->comm  );

    /* -----------------------------------
       Dirichlet pressure profile
       ----------------------------------- */
    for (int i = 0; i < nx; i++) {

        double x = get_x(i, m);   // global coordinate

        for (int j = 0; j < ny; j++) {

            int id = i * ny + j;

            sim->p[id] = sim->params.dp * (1.0 - x / m->lx);
        }
    }

    /* -----------------------------------
       NaN check (local)
       ----------------------------------- */
    int local_ok = check_nan("pressure", sim->p, nx * ny, step);

    /* -----------------------------------
       NaN check (global)
       ----------------------------------- */
    int global_ok;
    MPI_Allreduce(&local_ok, &global_ok, 1, MPI_INT, MPI_MIN, m->comm);

    return global_ok;
}

// =======================================================
// VELOCITY PROJECTION
// =======================================================
void update_velocity_mpi(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    double dx = m->dx;
    double dy = m->dy;
    double dt = sim->params.dt;

    // assumes p already has valid ghost values
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {

            int id = i * ny + j;

            int ip = (i + 1) * ny + j;
            int im = (i - 1) * ny + j;
            int jp = i * ny + (j + 1);
            int jm = i * ny + (j - 1);

            double dpdx = (sim->p[ip] - sim->p[im]) / (2.0 * dx);
            double dpdy = (sim->p[jp] - sim->p[jm]) / (2.0 * dy);

            sim->u[id] = sim->u_star[id] - dt * dpdx;
            sim->v[id] = sim->v_star[id] - dt * dpdy;
        }
    }
}

void apply_bc_mpi(Simulation *sim)
{
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;

    double Uin = 0.01;

    /* ---------------- inlet (rank 0 only physically) */
    if (m->rank_left == -1) {
        for (int j = 0; j < ny; j++) {
            sim->u[j] = Uin;
            sim->v[j] = 0.0;
            sim->u_star[j] = Uin;
            sim->v_star[j] = 0.0;
        }
    }

    /* ---------------- outlet (last rank) */
    if (m->rank_right == -1) {
        for (int j = 0; j < ny; j++) {
            int i = nx - 1;
            sim->u[i*ny + j] = sim->u[(i-1)*ny + j];
            sim->v[i*ny + j] = sim->v[(i-1)*ny + j];
            sim->u_star[i*ny + j] = sim->u_star[(i-1)*ny + j];
            sim->v_star[i*ny + j] = sim->v_star[(i-1)*ny + j];
        }
    }

    /* ---------------- walls */
    for (int i = 0; i < nx; i++) {
        sim->u[i*ny + 0] = 0.0;
        sim->u[i*ny + ny-1] = 0.0;

        sim->v[i*ny + 0] = 0.0;
        sim->v[i*ny + ny-1] = 0.0;

        sim->u_star[i*ny + 0] = 0.0;
        sim->u_star[i*ny + ny-1] = 0.0;

        sim->v_star[i*ny + 0] = 0.0;
        sim->v_star[i*ny + ny-1] = 0.0;
    }
}




int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    Simulation *sim = initialize_mpi("params.txt", MPI_COMM_WORLD);
    MPIMesh *m = sim->mesh;

    int nx = m->nx_local;
    int ny = m->ny_local;
    int N_local = nx * ny;

    // ======================================================
    // GLOBAL ARRAYS ONLY ON RANK 0
    // ======================================================
    double *u_global = NULL;
    double *v_global = NULL;
    double *p_global = NULL;

    if (m->rank == 0) {

        int N_global = m->nx_global * m->ny_global;

        u_global = malloc(N_global * sizeof(double));
        v_global = malloc(N_global * sizeof(double));
        p_global = malloc(N_global * sizeof(double));
    }

    /* ----------------------------------
       Create per-run output folder
       e.g. vtk_mpi/t<NPROCS>/
       and open run log (rank 0)
       ---------------------------------- */
    char vtk_root[] = "vtk_mpi";
    char proc_dir[2048];
    snprintf(proc_dir, sizeof(proc_dir), "%s/t%d", vtk_root, m->size);

    ensure_vtk_folder(vtk_root);
    ensure_vtk_folder(proc_dir);

    /* create GMRES residual output directory under the processor-specific vtk dir */
    char gmres_dir[2560];  
    snprintf(gmres_dir, sizeof(gmres_dir), "%s/GMRES", proc_dir);
    if (m->rank == 0) {
        ensure_vtk_folder(gmres_dir);
    }
    MPI_Barrier(m->comm);  /* ensure directory is created before solves */

    FILE *run_log = NULL;
    if (m->rank == 0) {
        char logpath[2048];
        const char *log_suffix = "/run.log";
        size_t proc_len = strlen(proc_dir);
        size_t suffix_len = strlen(log_suffix);
        if (proc_len + suffix_len + 1 > sizeof(logpath)) {
            // printf("Warning: log path too long, cannot open run.log\n");
        } else {
            memcpy(logpath, proc_dir, proc_len);
            memcpy(logpath + proc_len, log_suffix, suffix_len + 1);
            run_log = fopen(logpath, "w");
            if (!run_log) {
                // printf("Warning: could not open %s for logging\n", logpath);
            }
        }
    }

    /* write initialization header to run_log */
    if (m->rank == 0 && run_log) {
        fprintf(run_log, "\n======================================\n");
        fprintf(run_log, "MPI INITIALIZATION COMPLETE\n");
        fprintf(run_log, "======================================\n");
        fprintf(run_log, "Ranks        : %d\n", m->size);
        fprintf(run_log, "Global grid  : %d x %d\n", m->nx_global, m->ny_global);
        fprintf(run_log, "Local grid   : %d x %d\n", m->nx_local, m->ny_local);
        fprintf(run_log, "Domain size  : %f x %f\n", m->lx, m->ly);
        fprintf(run_log, "dx, dy       : %f, %f\n", m->dx, m->dy);
        fprintf(run_log, "dt, NT       : %e, %d\n", sim->params.dt, sim->params.NT);
        fprintf(run_log, "GMRES        : max_iter=%d tol=%e\n", sim->gmres.max_iter, sim->gmres.tol);
        fprintf(run_log, "======================================\n\n");
        fflush(run_log);
    }

    /* reset timing counters */
    operator_timing_reset();
    solver_timing_reset();

    double t_start = 0.0;
    if (m->rank == 0) t_start = MPI_Wtime();

    // ======================================================
    // TIME LOOP
    // ======================================================
    for (int n = 0; n < sim->params.NT; n++) {

        /* -----------------------------------
           STEP 1: Predictor BC
           ----------------------------------- */
        apply_bc_predictor_mpi(sim);

        /* -----------------------------------
           STEP 2: Halo exchange
           ----------------------------------- */
        exchange_halo(sim->u, m);
        exchange_halo(sim->v, m);

        /* -----------------------------------
           STEP 3: Intermediate velocity
           ----------------------------------- */
        compute_intermediate_velocity(sim);

        /* -----------------------------------
           STEP 4: Copy star fields
           ----------------------------------- */
        for (int i = 0; i < N_local; i++) {
            sim->u[i] = sim->u_star[i];
            sim->v[i] = sim->v_star[i];
        }

        /* -----------------------------------
           STEP 5: Pressure seed
           ----------------------------------- */
        update_pressure_profile_mpi(sim);

        /* -----------------------------------
           STEP 6: RHS
           ----------------------------------- */
        exchange_halo(sim->u_star, m);
        exchange_halo(sim->v_star, m);

        build_rhs_mpi(sim);

        /* -----------------------------------
           STEP 7: Pressure solve
           ----------------------------------- */
        if (!solve_pressure_mpi(sim, n)) {
            // if (m->rank == 0)
            //     printf("NaN detected in pressure at step %d\n", n);
        }

        /* -----------------------------------
           STEP 8: Pressure halo
           ----------------------------------- */
        exchange_halo(sim->p, m);

        /* -----------------------------------
           STEP 9: Velocity correction
           ----------------------------------- */
        update_velocity_mpi(sim);

        /* -----------------------------------
           STEP 10: Corrector BC
           ----------------------------------- */
        apply_bc_corrector_mpi(sim);

        /* -----------------------------------
           STEP 11: Final halo exchange
           ----------------------------------- */
        exchange_halo(sim->u, m);
        exchange_halo(sim->v, m);

        /* -----------------------------------
           STEP 12: OUTPUT (VTK)
           ----------------------------------- */
        if (sim->params.vtk_enable && (n % sim->params.output_every == 0)) {

            gather_field_to_root(sim->u, u_global, m);
            gather_field_to_root(sim->v, v_global, m);
            gather_field_to_root(sim->p, p_global, m);

            if (m->rank == 0) {

                char filename[2048];
                char numbuf[64];
                int rc = snprintf(numbuf, sizeof(numbuf), "/output_mpi_%04d.vtk", n);
                if (rc < 0) {
                    if (run_log) fprintf(run_log, "Warning: failed to format filename suffix\n");
                } else {
                    size_t proc_len = strlen(proc_dir);
                    size_t suf_len = (size_t)rc;
                    if (proc_len + suf_len + 1 > sizeof(filename)) {
                        if (run_log) fprintf(run_log, "Warning: filename too long, skipping VTK write\n");
                    } else {
                        memcpy(filename, proc_dir, proc_len);
                        memcpy(filename + proc_len, numbuf, suf_len + 1);

                        write_vtk(filename, m, u_global, v_global, p_global);

                        if (run_log) fprintf(run_log, "Wrote %s\n", filename);
                    }
                }
            }
        }
        
        
        double local_umax = 0.0;
        for (int i = 0; i < N_local; i++) {
            double u = fabs(sim->u[i]);
            if (u > local_umax) local_umax = u;
        }

        double global_umax;
        MPI_Allreduce(&local_umax, &global_umax, 1,
                      MPI_DOUBLE, MPI_MAX, m->comm);

        // if (m->rank == 0) {
        //     printf("Step %d | Umax = %e\n", n, global_umax);
        // }
    }

    // ======================================================
    // CLEANUP
    // ======================================================

    if (m->rank == 0) {
        double t_end = MPI_Wtime();
        double total = t_end - t_start;

        double op_seconds = 0.0; int op_calls = 0;
        operator_timing_get(&op_seconds, &op_calls);

        double gmres_seconds = 0.0; int gmres_calls = 0;
        solver_timing_get(&gmres_seconds, &gmres_calls);

        if (run_log) {
            fprintf(run_log, "MPI run summary\n");
            fprintf(run_log, "Ranks        : %d\n", m->size);
            fprintf(run_log, "Global grid  : %d x %d\n", m->nx_global, m->ny_global);
            fprintf(run_log, "Local grid   : %d x %d\n", m->nx_local, m->ny_local);
            fprintf(run_log, "Domain size  : %f x %f\n", m->lx, m->ly);
            fprintf(run_log, "dx, dy       : %f, %f\n", m->dx, m->dy);
            fprintf(run_log, "dt, NT       : %e, %d\n", sim->params.dt, sim->params.NT);
            fprintf(run_log, "GMRES        : max_iter=%d tol=%e\n", sim->gmres.max_iter, sim->gmres.tol);
            fprintf(run_log, "Total runtime : %f s\n", total);
            fprintf(run_log, "Operator total : %f s (%d calls)\n", op_seconds, op_calls);
            fprintf(run_log, "GMRES total : %f s (%d calls)\n", gmres_seconds, gmres_calls);
            fclose(run_log);
        }

        free(u_global);
        free(v_global);
        free(p_global);
    }

    destroy_simulation(sim);

    MPI_Finalize();
    return 0;
}