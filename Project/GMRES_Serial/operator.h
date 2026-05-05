#ifndef OPERATOR_H
#define OPERATOR_H

#include "mesh.h"

/*
 * Matrix-free Laplace operator:
 *
 *     A(x) = -∇²x
 *
 * Used for:
 *   - pressure Poisson equation
 *   - diffusion problems
 */
void apply_operator(const double *x,
                    double *y,
                    Mesh *mesh);

/* Timing helpers for operator application */
void operator_timing_reset(void);
void operator_timing_get(double *total_seconds, int *calls);

#endif