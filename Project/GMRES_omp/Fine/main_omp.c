#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <omp.h>
#include <string.h>

#include "mesh.h"
#include "field.h"
#include "vtk.h"
#include "solver.h"
#include "operator.h"
#include "postpro.h"
#include "utils.h"
#include "io.h"

// -------------------------------
static int output_every = 100;
static int vtk_enable = 1;
static char vtk_root[128] = "omp_vtk";
static char vtk_dir[160]  = "omp_vtk/t1";
static char postpro_file[200] = "omp_vtk/t1/postpro_omp.out";
static char log_file[200] = "omp_vtk/t1/run.log";
static FILE *run_log = NULL;
int GMRES_Residual_Plot = 0; /* Set to 1 to enable residual output for plotting */

static Mesh *mesh;
static Field *field;

static double *b;
static double *u_star;
static double *v_star;

static int nx, ny, N;

static double dt, nu;
static double dp;
static int NT;

static GMRESParams gmres_params;

static void log_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    va_list copy;
    va_copy(copy, args);

    vprintf(fmt, args);
    fflush(stdout);

    if (run_log) {
        vfprintf(run_log, fmt, copy);
        fflush(run_log);
    }

    va_end(copy);
    va_end(args);
}

static double wall_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

// -------------------------------
void initialize(int requested_threads) {

    Params p = read_params("params.txt");

    mesh = create_mesh(p.nx, p.ny, p.lx, p.ly);

    nx = mesh->nx_global;
    ny = mesh->ny_global;
    N  = nx * ny;

    dt = p.dt;
    nu = p.viscosity;
    dp = p.dp;
    NT = p.NT;
    vtk_enable = p.vtk_enable;
    output_every = p.output_every;

    if (output_every <= 0) {
        output_every = 100;
    }

    b      = calloc(N, sizeof(double));
    u_star = calloc(N, sizeof(double));
    v_star = calloc(N, sizeof(double));

    gmres_params.max_iter = p.max_iter;
    gmres_params.tol      = p.tol;

    omp_set_dynamic(0);

    if (requested_threads <= 0) {
        requested_threads = omp_get_max_threads();
    }

    omp_set_num_threads(requested_threads);

    snprintf(vtk_dir, sizeof(vtk_dir), "%s/t%d", vtk_root, requested_threads);
    snprintf(postpro_file, sizeof(postpro_file), "%s/postpro_omp.out", vtk_dir);
    snprintf(log_file, sizeof(log_file), "%s/run.log", vtk_dir);

    mkdir(vtk_root, 0777);
    mkdir(vtk_dir, 0777);

    /* create GMRES residual output directory under the thread-specific vtk dir */
    char gmres_dir[256];
    snprintf(gmres_dir, sizeof(gmres_dir), "%s/GMRES", vtk_dir);
    mkdir(gmres_dir, 0777);

    field  = create_field(mesh);

    run_log = fopen(log_file, "w");
    if (!run_log) {
        printf("Warning: could not open %s for logging\n", log_file);
    }

    postpro_init(postpro_file);

    log_printf("Using %d OpenMP threads\n", omp_get_max_threads());

    /* Write initialization header to log */
    log_printf("\n======================================\n");
    log_printf("INITIALIZATION COMPLETE\n");
    log_printf("======================================\n");
    log_printf("Global grid  : %d x %d\n", nx, ny);
    log_printf("Domain size  : %f x %f\n", mesh->lx, mesh->ly);
    log_printf("dx, dy       : %f, %f\n", mesh->dx, mesh->dy);
    log_printf("dt, NT       : %e, %d\n", dt, NT);
    log_printf("GMRES        : max_iter=%d tol=%e\n", gmres_params.max_iter, gmres_params.tol);
    log_printf("======================================\n\n");
}

// -------------------------------
void apply_bc_predictor() {

    double U_in = 0.01;

    #pragma omp parallel for schedule(static)
    for (int j = 0; j < ny; j++) {
        field->u[IDX(0,j,nx)] = U_in;
        field->v[IDX(0,j,nx)] = 0.0;
    }

    #pragma omp parallel for schedule(static)
    for (int j = 0; j < ny; j++) {
        field->u[IDX(nx-1,j,nx)] = field->u[IDX(nx-2,j,nx)];
        field->v[IDX(nx-1,j,nx)] = field->v[IDX(nx-2,j,nx)];
    }

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < nx; i++) {
        field->u[IDX(i,0,nx)]    = 0.0;
        field->u[IDX(i,ny-1,nx)] = 0.0;

        field->v[IDX(i,0,nx)]    = 0.0;
        field->v[IDX(i,ny-1,nx)] = 0.0;
    }
}

// -------------------------------
void apply_bc_corrector() {

    #pragma omp parallel for schedule(static)
    for (int j = 0; j < ny; j++) {
        field->u[IDX(nx-1,j,nx)] = field->u[IDX(nx-2,j,nx)];
        field->v[IDX(nx-1,j,nx)] = field->v[IDX(nx-2,j,nx)];
    }

    #pragma omp parallel for
    for (int i = 0; i < nx; i++) {
        field->u[IDX(i,0,nx)]    = 0.0;
        field->u[IDX(i,ny-1,nx)] = 0.0;

        field->v[IDX(i,0,nx)]    = 0.0;
        field->v[IDX(i,ny-1,nx)] = 0.0;
    }
}

// -------------------------------
void update_pressure_profile() {

    #pragma omp parallel for collapse(2)
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            double x = mesh->x[i];
            field->p[IDX(i,j,nx)] = dp * (1.0 - x / mesh->lx);
        }
    }
}

// -------------------------------
void compute_intermediate_velocity() {

    double G = dp / mesh->lx;

    #pragma omp parallel for collapse(2)
    for (int j = 1; j < ny-1; j++) {
        for (int i = 1; i < nx-1; i++) {

            int id = IDX(i,j,nx);
            int ip = IDX(i+1,j,nx);
            int im = IDX(i-1,j,nx);
            int jp = IDX(i,j+1,nx);
            int jm = IDX(i,j-1,nx);

            double u = field->u[id];
            double v = field->v[id];

            double dudx = (field->u[ip] - field->u[im]) / (2.0*mesh->dx);
            double dudy = (field->u[jp] - field->u[jm]) / (2.0*mesh->dy);

            double dvdx = (field->v[ip] - field->v[im]) / (2.0*mesh->dx);
            double dvdy = (field->v[jp] - field->v[jm]) / (2.0*mesh->dy);

            double adv_u = u*dudx + v*dudy;
            double adv_v = u*dvdx + v*dvdy;

            u_star[id] = field->u[id]
                        - dt * adv_u
                        + dt * nu * laplacian(field->u, i, j, mesh)
                        + dt * G;

            v_star[id] = field->v[id]
                        - dt * adv_v
                        + dt * nu * laplacian(field->v, i, j, mesh);
        }
    }
}

// -------------------------------
void build_rhs() {

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) b[i] = 0.0;

    #pragma omp parallel for collapse(2)
    for (int j = 1; j < ny-1; j++) {
        for (int i = 1; i < nx-1; i++) {

            int id = IDX(i,j,nx);
            int ip = IDX(i+1,j,nx);
            int im = IDX(i-1,j,nx);
            int jp = IDX(i,j+1,nx);
            int jm = IDX(i,j-1,nx);

            double dudx = (u_star[ip] - u_star[im]) / (2.0*mesh->dx);
            double dvdy = (v_star[jp] - v_star[jm]) / (2.0*mesh->dy);

            b[id] = (dudx + dvdy) / dt;
        }
    }

    //Enforce pressure-profile values on boundary rows.
    #pragma omp parallel for collapse(2)
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            double x = mesh->x[i];
            b[IDX(i,j,nx)] = dp * (1.0 - x / mesh->lx);
        }
    }
}

// -------------------------------
int solve_pressure(int step) {

    gmres_solve(field->p, b, mesh, gmres_params, step);

    // Pressure profile 
    #pragma omp parallel for collapse(2)
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            double x = mesh->x[i];
            field->p[IDX(i,j,nx)] = dp * (1.0 - x / mesh->lx);
        }
    }

    return check_nan("pressure", field->p, N, step);
}

// -------------------------------
void update_velocity() {

    #pragma omp parallel for collapse(2)
    for (int j = 1; j < ny-1; j++) {
        for (int i = 1; i < nx-1; i++) {

            int id = IDX(i,j,nx);
            int ip = IDX(i+1,j,nx);
            int im = IDX(i-1,j,nx);
            int jp = IDX(i,j+1,nx);
            int jm = IDX(i,j-1,nx);

            double dpdx = (field->p[ip] - field->p[im]) / (2.0*mesh->dx);
            double dpdy = (field->p[jp] - field->p[jm]) / (2.0*mesh->dy);

            field->u[id] = u_star[id] - dt * dpdx;
            field->v[id] = v_star[id] - dt * dpdy;
        }
    }
}

// -------------------------------
int main(int argc, char **argv) {

    int requested_threads = 0;

    if (argc > 1) {
        requested_threads = atoi(argv[1]);
        if (requested_threads <= 0) {
            log_printf("Invalid thread count '%s'. Using OpenMP default.\n", argv[1]);
            requested_threads = 0;
        }
    }

   

    initialize(requested_threads);

    /* reset timing counters */
    solver_timing_reset();
    operator_timing_reset();

    double start = wall_seconds();

    for (int n = 0; n < NT; n++) {

        double time = n * dt;

        // log_printf("\nSTEP %d | time = %.5f\n", n, time);

        apply_bc_predictor();

        compute_intermediate_velocity();

        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            field->u[i] = u_star[i];
            field->v[i] = v_star[i];
        }

        update_pressure_profile();

        build_rhs();
        solve_pressure(n);

        update_velocity();

        apply_bc_corrector();

        // diagnostics
        double umax = 0.0;

        #pragma omp parallel for reduction(max:umax) schedule(static)
        for (int i = 0; i < N; i++) {
            double u = fabs(field->u[i]);
            if (u > umax) umax = u;
        }

        // double cfl = compute_cfl(field, mesh, N, dt, nu);

        // log_printf("CFL = %.6f | Umax = %.6e\n", cfl, umax);

        if (check_nan("u", field->u, N, n)) break;
        if (check_nan("v", field->v, N, n)) break;

        // output
        if (vtk_enable && (n % output_every == 0)) {
            char filename[256];
            snprintf(filename, sizeof(filename), "%s/output_omp_%04d_%04d.vtk", vtk_dir, omp_get_max_threads(), n);
            write_vtk(filename, mesh, field);
            log_printf("wrote %s\n", filename);

            postpro_write(n, time, field, mesh, dt);
        }
    }

    postpro_close();

    double end = wall_seconds();

    if (vtk_enable) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/final_omp.vtk", vtk_dir);
        write_vtk(filename, mesh, field);
    }

    /* Write summary to log */
    log_printf("OpenMP run summary\n");
    log_printf("Threads      : %d\n", omp_get_max_threads());
    log_printf("Global grid  : %d x %d\n", nx, ny);
    log_printf("Domain size  : %f x %f\n", mesh->lx, mesh->ly);
    log_printf("dx, dy       : %f, %f\n", mesh->dx, mesh->dy);
    log_printf("dt, NT       : %e, %d\n", dt, NT);
    log_printf("GMRES        : max_iter=%d tol=%e\n", gmres_params.max_iter, gmres_params.tol);
    log_printf("Total runtime : %f s\n", end - start);

    /* report solver/operator timings */
    double s_time = 0.0; int s_calls = 0;
    double o_time = 0.0; int o_calls = 0;
    solver_timing_get(&s_time, &s_calls);
    operator_timing_get(&o_time, &o_calls);
    log_printf("Operator total : %f s (%d calls)\n", o_time, o_calls);
    log_printf("GMRES total : %f s (%d calls)\n", s_time, s_calls);

    if (run_log) {
        fclose(run_log);
        run_log = NULL;
    }

    return 0;
}