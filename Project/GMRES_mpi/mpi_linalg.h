#ifndef MPI_LINALG_H
#define MPI_LINALG_H

#include <mpi.h>
#include "mpi_mesh.h"

/*
 * MPI-aware linear algebra for distributed GMRES.
 * Provides global dot product and norm operations.
 */

/* Global dot product: sum of local products across all ranks */
double mpi_dot_product(const double *x, const double *y, int n_local, MPI_Comm comm);

/* Global 2-norm */
double mpi_norm2(const double *x, int n_local, MPI_Comm comm);

/* Vector operations (local, no MPI) */
void vec_copy(const double *src, double *dst, int n);
void vec_scale(double *x, double alpha, int n);
void vec_axpy(double alpha, const double *x, double *y, int n);  /* y += alpha*x */

/* Interior-only versions (exclude ghost cells) */
double mpi_dot_product_interior(const double *x, const double *y, int n_interior, 
                                 MPIMesh *mmesh, MPI_Comm comm);
double mpi_norm2_interior(const double *x, int n_interior, MPIMesh *mmesh, MPI_Comm comm);
void vec_copy_interior(const double *src, double *dst, int n_interior, MPIMesh *mmesh);
void vec_axpy_interior(double alpha, const double *x, double *y, int n_interior, MPIMesh *mmesh);
void vec_zero_ghosts(double *v, MPIMesh *mmesh);

#endif
