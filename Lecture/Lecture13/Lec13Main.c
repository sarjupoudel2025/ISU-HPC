/*Create matrix.c and matrix.h for linear algebra subroutines, including Gaussian Elimination with partial pivoting
and Back Substitution for solving a linear system. 
Application: 1D Imaging Deblurring by solving Ax = b, where x is the true signal (unknown), A is the blur operator
and b is the observed blurred signal

# Blur Model: Discrete Convolution with symmetric blur kernel:
b_i = Sum(-k,k) w_j * x_{i-j} , w=[1,2,1]/4, so b_i = (x_{i-1} + 2*x_i + x_{i+1})/4
Assuming zero boundary conditions, this defines the linear system with
A= 1/4 [[2 1 0 . . .], [1 2 1 . . .], [0, 1, 2 . . .]. . .]

# Generate Test Signal= x_i = 1 for n/4 <= i <= n/2, and 0 else.
And Compute b= Ax

# Add noise to the observed signal b as b_i = b_i + e_i with
e_i = N(0, sigma^2). Test with signma = 10^(-6), 10^(-4), and 10^(-2).

#Solve  Ax_rec = b + e, and  measure the error ||x-x_rec|| / ||x||.

#With  Tikhonov regularization: (A^T A + lambda*I)x = A^T b*, with
lambda = 10^(-6), 10^(-4), and 10^(-2), ALSO MEASURE ERRORS.

# Test  with n = 64, 128. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "Lec13_matrix.h"

int main() {

    srand(20); // Seed the random number generator for noise generation
   
    //Sample Matrix
    Matrix* A = new_matrix(3, 3);
    A->data[0] = 1; A->data[1] = 2; A->data[2] = 3;
    A->data[3] = 0; A->data[4] = 1; A->data[5] = 4;
    A->data[6] = 1; A->data[7] = 0; A->data[8] = 2;
    print_matrix(A);


    //Sample Vector
    Vector* x = new_vector(3);
    x->data[0] = 1; x->data[1] = 2; x->data[2] = 3;
    print_vector(x);


    //Sample solve Ax = b
    Vector* b = matrix_vector_multiply(A, x);
    print_vector(b);
    Vector* x_rec0 = solve(A, b);
    print_vector(x_rec0);
    // Clean up
    free_vector(x);
    free_vector(b);
    free_vector(x_rec0);
    free_matrix(A);
    /* Testing of Functional Definition Completed and Starting Assignment Problem*/

    printf("\n********************************************************************************************************\n");

    // Size of Matrix/vector test for n = 64, 128
    int n_values[] = {64, 128};
    int num_ns = sizeof(n_values) / sizeof(n_values[0]);

    // Noise levels σ and regularization parameters λ to test
    double sigma_values[]  = {1e-6, 1e-4, 1e-2};
    double lambda_values[] = {1e-6, 1e-4, 1e-2};

    int num_sigmas  = sizeof(sigma_values)  / sizeof(sigma_values[0]);
    int num_lambdas = sizeof(lambda_values) / sizeof(lambda_values[0]);

    for (int ni = 0; ni < num_ns; ni++) {

        int n = n_values[ni];
        printf("\n========================\nTesting with n = %d\n========================\n", n);

        // -----------------------
        // Allocate matrices/vectors
        // -----------------------
        Matrix* A_blur    = new_matrix(n, n);
        Vector* x_true    = new_vector(n);
        Vector* b_observed = new_vector(n);

        // -----------------------
        // Create Blur Matrix A
        // -----------------------
        for (int i = 0; i < n; i++) {
            if (i > 0)        A_blur->data[i * n + (i - 1)] = 0.25;
            A_blur->data[i * n + i] = 0.5;
            if (i < n - 1)    A_blur->data[i * n + (i + 1)] = 0.25;
        }

        // -----------------------
        // Create True Signal x
        // -----------------------
        for (int i = 0; i < n; i++) {
            if (i >= n / 4 && i <= n / 2) x_true->data[i] = 1.0;
            else x_true->data[i] = 0.0;
        }

        // -----------------------
        // Compute blurred signal b = A x
        // -----------------------
        Vector* b_blurred = matrix_vector_multiply(A_blur, x_true);

        Vector* x_rec = NULL;

        // Transpose of A
        Matrix* At = matrix_transpose(A_blur);

        // -----------------------
        // Loop over noise levels σ
        // -----------------------
        for (int s = 0; s < num_sigmas; s++) {

            double sigma = sigma_values[s];

            // Add noise
            for (int i = 0; i < n; i++) {
                b_observed->data[i] = b_blurred->data[i] + generate_normal(sigma);
            }

            printf("\n----- Sigma = %e -----\n", sigma);

            // -----------------------
            // Solve WITHOUT Tikhonov
            // -----------------------
            Matrix* AtA_plain = matrix_multiply(At, A_blur);
            Vector* Atb_plain = matrix_vector_multiply(At, b_observed);

            if (x_rec != NULL) free_vector(x_rec);
            x_rec = solve(AtA_plain, Atb_plain);

            Vector* diff_plain = vector_subtract(x_true, x_rec);
            double error_plain = vector_dot(diff_plain, diff_plain) /
                                vector_dot(x_true, x_true);

            printf("No regularization | Relative Error = %e\n", error_plain);

            free_vector(diff_plain);
            free_matrix(AtA_plain);
            free_vector(Atb_plain);

            // ---------------------------------------------
            // Tikhonov Regularization Applied to each sigma
            // ---------------------------------------------

            // Loop over regularization parameters λ
            for (int l = 0; l < num_lambdas; l++) {

                double lambda = lambda_values[l];

                // Compute (A^T A + λI) and A^T b
                Matrix* AtA = matrix_multiply(At, A_blur);
                Vector* Atb = matrix_vector_multiply(At, b_observed);

                // Add λI to AtA
                for (int i = 0; i < n; i++) {
                    AtA->data[i * n + i] += lambda;
                }

                free_vector(x_rec);
                x_rec = solve(AtA, Atb);

                Vector* diff = vector_subtract(x_true, x_rec);
                double error = vector_dot(diff, diff) /
                            vector_dot(x_true, x_true);

                printf("Lambda = %e | Relative Error = %e\n", lambda, error);

                free_vector(diff);
                free_matrix(AtA);
                free_vector(Atb);
            }
        }

        // -----------------------
        // Cleanup for this n
        // -----------------------
        free_matrix(At);
        free_matrix(A_blur);
        free_vector(x_true);
        free_vector(b_blurred);
        free_vector(b_observed);
        if (x_rec != NULL) free_vector(x_rec);
    }
    return 0;
}