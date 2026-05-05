#include <stdio.h>
#include <math.h>
#include <float.h>

#include "utils.h"
#include "field.h"
#include "mesh.h"

// -------------------------------
// NAN (Safety) CHECK
// -------------------------------
int check_nan(const char *name, double *a, int N, int step) {

    for (int i = 0; i < N; i++) {
        if (!isfinite(a[i])) {
            printf("NaN in %s at index %d (step %d)\n",
                   name, i, step);
            return 1;
        }
    }
    return 0;
}

// -------------------------------
// CFL COMPUTATION
// -------------------------------
double compute_cfl(Field *f, Mesh *mesh, int N, double dt, double nu) {

    double umax = 1e-12;
    double vmax = 1e-12;

    for (int i = 0; i < N; i++) {
        double u = fabs(f->u[i]);
        double v = fabs(f->v[i]);

        if (u > umax) umax = u;
        if (v > vmax) vmax = v;
    }

    double cfl_u  = umax * dt / mesh->dx;
    double cfl_v  = vmax * dt / mesh->dy;
    double cfl_nu = nu * dt * (1.0/(mesh->dx*mesh->dx) + 1.0/(mesh->dy*mesh->dy));

    return fmax(fmax(cfl_u, cfl_v), cfl_nu);
}

// -------------------------------
// GENERIC LAPLACIAN
// -------------------------------
double laplacian(double *f, int i, int j, Mesh *mesh) {

    int nx = mesh->nx_global;

    int id = IDX(i,j,nx);
    int ip = IDX(i+1,j,nx);
    int im = IDX(i-1,j,nx);
    int jp = IDX(i,j+1,nx);
    int jm = IDX(i,j-1,nx);

    double dx = mesh->dx;
    double dy = mesh->dy;

    return (
        (f[ip] - 2.0*f[id] + f[im]) / (dx*dx) +
        (f[jp] - 2.0*f[id] + f[jm]) / (dy*dy)
    );
}