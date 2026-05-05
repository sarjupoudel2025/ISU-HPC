#include <math.h>
#include "linalg.h"

double dot(int n, const double *a, const double *b) {

    // -------------------------------
    // Kahan summation (reduces floating error)
    // -------------------------------
    double sum = 0.0;
    double c = 0.0;

    for (int i = 0; i < n; i++) {
        double y = a[i] * b[i] - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

void axpy(int n, double *y, const double *x, double a) {

    for (int i = 0; i < n; i++) {
        y[i] += a * x[i];
    }
}

void copy_vec(int n, double *dst, const double *src) {

    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

double norm(int n, const double *x) {
    return sqrt(dot(n, x, x));
}