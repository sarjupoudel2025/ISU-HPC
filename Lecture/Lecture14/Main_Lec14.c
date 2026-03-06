#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Matrix_lib_Lec14.h"

int main() {
    srand(time(NULL));  // Seed random number generator

    int n_values[] = {5, 10};
    int num_ns = sizeof(n_values) / sizeof(n_values[0]);

    double u[] = {0,2};
    int num_us = sizeof(u) / sizeof(u[0]);

    for (int ni = 0; ni < num_ns; ni++) {
        int n = n_values[ni];
        Matrix* A = new_matrix(n, n);

        for (int i = 0; i < n; i++) {
            if (i > 0)      A->data[i*n + i-1] = 0.25;   // sub-diagonal
            A->data[i*n + i] = 0.5;                     // main diagonal
            if (i < n-1)    A->data[i*n + i+1] = 0.25;  // super-diagonal
        }

        printf("Matrix A:\n");
        print_matrix(A);

        Vector* eigenvec = new_vector(n);
        int max_iter = 1000;
        double tol = 1e-12;

        // --- Power Iteration ---
        double lambda_power = power_iteration(A, eigenvec, max_iter, tol);
        printf("Power Iteration:\n");
        printf("Dominant Eigenvalue: %.8f\n", lambda_power);
        printf("Corresponding Eigenvector:\n");
        print_vector(eigenvec);

        // --- Shifted Inverse Iteration ---
        for (int ui = 0; ui < num_us; ui++) {
            double shift= u[ui];
            double lambda_shifted = shifted_inverse_iteration(A, eigenvec, shift, max_iter, tol);
            printf("Shifted Inverse Iteration (shift = %.2f):\n", shift);
            printf("Eigenvalue closest to shift: %.8f\n", lambda_shifted);
            printf("Corresponding Eigenvector:\n");
            print_vector(eigenvec);
        }

        /*
        double shift = 0; // Example shift near desired eigenvalue
        double lambda_shifted = shifted_inverse_iteration(A, eigenvec, shift, max_iter, tol);
        printf("Shifted Inverse Iteration (shift = %.2f):\n", shift);
        printf("Eigenvalue closest to shift: %.8f\n", lambda_shifted);
        printf("Corresponding Eigenvector:\n");
        print_vector(eigenvec);*/

        // --- Rayleigh Quotient Iteration ---
        /*for (int i = 0; i < n; i++)
            eigenvec->data[i] = 1; // Re-initialize eigenvector randomly*/
        double lambda_rqi = rayleigh_quotient_iteration(A, eigenvec, max_iter, tol);
        printf("Rayleigh Quotient Iteration:\n");
        printf("Converged Eigenvalue: %.8f\n", lambda_rqi);
        printf("Corresponding Eigenvector:\n");
        print_vector(eigenvec);

        // --- Free Memory ---
        free_vector(eigenvec);
        free_matrix(A);
    }
    return 0;
}