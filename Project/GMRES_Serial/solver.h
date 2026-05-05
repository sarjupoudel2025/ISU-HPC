#ifndef SOLVER_H
#define SOLVER_H

#include "mesh.h"

// -------------------------------
// GMRES control parameters
// -------------------------------
typedef struct {
    int max_iter;
    double tol;
} GMRESParams;

extern int GMRES_Residual_Plot;

/*
 * GMRES solves:
 *      A x = b
 * where A is the matrix-free Laplacian operator defined in operator.c
 */
int gmres_solve(double *x,
                const double *b,
                Mesh *mesh,
                GMRESParams params,
                int step);

/* Timing helpers (wall-clock) */
void solver_timing_reset(void);
void solver_timing_get(double *total_seconds, int *calls);

#endif