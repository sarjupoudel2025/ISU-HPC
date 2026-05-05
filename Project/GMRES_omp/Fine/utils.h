#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "mesh.h"
#include "field.h"

// -------------------------------
// INDEX MACRO (shared utility)
// -------------------------------
#ifndef IDX
#define IDX(i,j,nx) ((j)*(nx) + (i))
#endif

// -------------------------------
// SAFETY CHECK
// -------------------------------
int check_nan(const char *name, double *a, int N, int step);

// -------------------------------
// CFL
// -------------------------------
double compute_cfl(Field *f, Mesh *mesh, int N, double dt, double nu);

// -------------------------------
// LAPLACIAN (generic scalar field)
// -------------------------------
double laplacian(double *f,
                 int i, int j,
                 Mesh *mesh);

#endif