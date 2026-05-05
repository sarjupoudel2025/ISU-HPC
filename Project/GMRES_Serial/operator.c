#define _POSIX_C_SOURCE 200809L

#include "mesh.h"

#define IDX(i,j,nx) ((j)*(nx) + (i))

#include <time.h>

static double total_apply_seconds = 0.0;
static int apply_call_count = 0;

void apply_operator(const double *x,
                    double *y,
                    Mesh *mesh)
{
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    apply_call_count++;

    int nx = mesh->nx_global;
    int ny = mesh->ny_global;

    double dx2 = mesh->dx * mesh->dx;
    double dy2 = mesh->dy * mesh->dy;

    // initialize
    for (int i = 0; i < nx*ny; i++)
        y[i] = 0.0;

    for (int j = 1; j < ny-1; j++) {
        for (int i = 1; i < nx-1; i++) {

            int id = IDX(i,j,nx);

            double center = x[id];
            double left   = x[IDX(i-1,j,nx)];
            double right  = x[IDX(i+1,j,nx)];
            double down   = x[IDX(i,j-1,nx)];
            double up     = x[IDX(i,j+1,nx)];

            double lap =
                (left - 2.0*center + right) / dx2 +
                (down - 2.0*center + up) / dy2;

            y[id] = -lap;
        }
    }

    // Dirichlet boundary 
    for (int j = 0; j < ny; j++) {
        y[IDX(0,j,nx)] = x[IDX(0,j,nx)];
        y[IDX(nx-1,j,nx)] = x[IDX(nx-1,j,nx)];
    }
    for (int i = 0; i < nx; i++) {
        y[IDX(i,0,nx)] = x[IDX(i,0,nx)];
        y[IDX(i,ny-1,nx)] = x[IDX(i,ny-1,nx)];
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    total_apply_seconds += (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)*1e-9;
}

void operator_timing_reset(void) {
    total_apply_seconds = 0.0;
    apply_call_count = 0;
}

void operator_timing_get(double *total_seconds, int *calls) {
    if (total_seconds) *total_seconds = total_apply_seconds;
    if (calls) *calls = apply_call_count;
}