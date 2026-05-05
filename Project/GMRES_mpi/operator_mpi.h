#ifndef OPERATOR_MPI_H
#define OPERATOR_MPI_H

#include <mpi.h>
#include "mesh_mpi.h"

/* ======================================================
   MPI LAPLACIAN OPERATOR
   y = -∇² x  (on decomposed domain with ghost cells)
   ====================================================== */
void apply_operator_mpi(const double *x,
                        double *y,
                        MPIMesh *mesh);

/* ======================================================
   TIMING INTERFACE (same style as serial code)
   ====================================================== */
void operator_timing_reset(void);

void operator_timing_get(double *total_seconds, int *calls);

#endif