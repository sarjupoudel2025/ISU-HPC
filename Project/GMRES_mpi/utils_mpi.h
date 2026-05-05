#ifndef UTILS_MPI_H
#define UTILS_MPI_H

#include <stdbool.h>


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

#endif
