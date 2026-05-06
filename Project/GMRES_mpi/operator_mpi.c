#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "operator_mpi.h"
#include "time.h"

#include "mesh_mpi.h"

/* ======================================================
   INDEXING MACRO (LOCAL GRID)
   ====================================================== */
#define IDX(i,j,ny) ((i)*(ny) + (j))

/* ======================================================
   HALO EXCHANGE EXTERNAL DECLARATION
   ====================================================== */
void exchange_halo(double *x, MPIMesh *mesh);

/* ======================================================
   MPI OPERATOR TIMING 
   ====================================================== */
static double total_apply_time = 0.0;
static int apply_calls = 0;

/* ======================================================
   APPLY LAPLACIAN OPERATOR (MPI VERSION)
   y = -∇² x
   ====================================================== */
void apply_operator_mpi(const double *x,
                        double *y,
                        MPIMesh *mesh)
{
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    apply_calls++;

    int nx = mesh->nx_local;
    int ny = mesh->ny_local;

    double dx2 = mesh->dx * mesh->dx;
    double dy2 = mesh->dy * mesh->dy;

    /* --------------------------------------
       IMPORTANT:
       Ensure ghost cells are valid BEFORE compute
       -------------------------------------- */
    exchange_halo((double*)x, mesh);

    /* --------------------------------------
       initialize output
       -------------------------------------- */
    for (int i = 0; i < nx * ny; i++)
        y[i] = 0.0;

    /* ======================================
       INTERIOR STENCIL COMPUTATION
       (skip ghost boundaries: 1 .. nx-2)
       ====================================== */
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {

            int id = IDX(i, j, ny);

            double center = x[id];
            double left   = x[IDX(i-1, j, ny)];
            double right  = x[IDX(i+1, j, ny)];
            double down   = x[IDX(i, j-1, ny)];
            double up     = x[IDX(i, j+1, ny)];

            double lap =
                (left  - 2.0 * center + right) / dx2 +
                (down  - 2.0 * center + up)    / dy2;

            y[id] = -lap;
        }
    }

    /* ======================================
       BOUNDARY HANDLING (LOCAL DOMAIN ONLY)
       ====================================== */

    /* LEFT / RIGHT boundaries (domain decomposition boundaries handled via ghosts) */
    for (int j = 0; j < ny; j++) {

        /* left physical boundary */
        if (mesh->rank_left == MPI_PROC_NULL) {
            y[IDX(0, j, ny)] = x[IDX(0, j, ny)];
        }

        /* right physical boundary */
        if (mesh->rank_right == MPI_PROC_NULL) {
            y[IDX(nx-1, j, ny)] = x[IDX(nx-1, j, ny)];
        }
    }

    /* TOP / BOTTOM boundaries (global domain walls) */
    for (int i = 0; i < nx; i++) {

        y[IDX(i, 0, ny)]     = x[IDX(i, 0, ny)];
        y[IDX(i, ny-1, ny)]  = x[IDX(i, ny-1, ny)];
    }

    /* ======================================
       TIMING END
       ====================================== */
    clock_gettime(CLOCK_MONOTONIC, &t1);
    total_apply_time +=
        (t1.tv_sec - t0.tv_sec) +
        (t1.tv_nsec - t0.tv_nsec) * 1e-9;
}

/* ======================================================
   TIMING HELPERS
   ====================================================== */
void operator_timing_reset(void)
{
    total_apply_time = 0.0;
    apply_calls = 0;
}

void operator_timing_get(double *total_seconds, int *calls)
{
    if (total_seconds) *total_seconds = total_apply_time;
    if (calls) *calls = apply_calls;
}