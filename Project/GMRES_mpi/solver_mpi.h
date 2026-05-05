#ifndef SOLVER_MPI_H
#define SOLVER_MPI_H

#include <mpi.h>
#include "mesh_mpi.h"
#include "operator_mpi.h"

/* ======================================================
   GMRES PARAMETERS (same as your serial solver.h)
   ====================================================== */
typedef struct {
    int max_iter;   /* maximum iterations */
    double tol;     /* convergence tolerance */
} GMRESParams;

extern int GMRES_Residual_Plot;

/* ======================================================
   MPI GMRES SOLVER
   Solves: A x = b using matrix-free Laplacian operator
   ====================================================== */
int gmres_solve_mpi(double *x,
                    const double *b,
                    MPIMesh *mesh,
                    GMRESParams params,
                    int step,
                    MPI_Comm comm);

/* Timing interface for MPI GMRES (same style as serial) */
void solver_timing_reset(void);
void solver_timing_get(double *total_seconds, int *calls);

#endif