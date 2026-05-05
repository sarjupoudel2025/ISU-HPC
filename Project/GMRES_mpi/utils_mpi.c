#include <stdio.h>
#include <math.h>
#include <float.h>

#include "utils_mpi.h"



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



