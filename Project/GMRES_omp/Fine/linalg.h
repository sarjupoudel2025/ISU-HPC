#ifndef LINALG_H
#define LINALG_H

double dot(int n, const double *a, const double *b);

void axpy(int n, double *y, const double *x, double a);

void copy_vec(int n, double *dst, const double *src);

double norm(int n, const double *x);

#endif