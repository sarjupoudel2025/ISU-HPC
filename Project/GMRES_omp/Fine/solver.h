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

/*
 * GMRES solves:
 *      A x = b
 *
 * NOTE:
 * - Physics is handled in operator.c
 * - NOT inside this solver
 */
extern int GMRES_Residual_Plot;

int gmres_solve(double *x,
                const double *b,
                Mesh *mesh,
                GMRESParams params,
                int step);

/* Timing helpers (wall-clock) */
void solver_timing_reset(void);
void solver_timing_get(double *total_seconds, int *calls);

#endif