#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"
#include "trimatrix.h"

const double Math_PI = 3.14159265358979323846;


int main()
{
    int n_values[] = {6, 12};

    for (int s = 0; s < 2; s++)
    {
        int N = n_values[s];
        printf("************Size of Matrix A ********************\n");
        printf("  N = %d\n", N);
        printf("***************************************************\n");

        // Create tridiagonal matrix A used in previous lectures.
        matrix A = new_matrix(N, N);
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= N; j++)
                mget(A, i, j) = 2.0*(i == j) - 1.0*(abs(i-j) == 1);

        printf("\n Tridiagonal Matrix A = \n");
        print_matrix(&A);

        /*Hessenberg (tridiagonal) reduction matrix*/
        trimatrix H = new_trimatrix(N);
        Hessenberg(&A, &H);

        printf("Hessenberg reduction to Matrix A is H:\n");
        print_trimatrix(&H);

        /*Apply QRA algorithm on Hessenberg matrix to get eigenvalues*/
        QRA(&H);

        printf("Matrix H after QRA Applied: \n");
        print_trimatrix(&H);

        /* Eigenvalues values obtained from QRA on Hessenberg matrix */
        printf("Obtained Eigen Values After QRA Algorithm on Hessenberg Matrix or Triagonal Matrix A: \n");
        for (int i = 1; i <= N; i++)
            printf("  lambda_%d = %18.10f\n", i, tget(&H, i, i));


        delete_trimatrix(&H);
        delete_matrix(&A);
        printf("\n");
    }

    return 0;
}