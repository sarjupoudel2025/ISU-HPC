#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "Lec13_matrix.h"

Matrix* new_matrix(const int rows, const int cols) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    assert(mat != NULL); 
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (double*)malloc(rows * cols * sizeof(double));
    assert(mat->data != NULL); 

    for(int i = 0; i < rows * cols; i++) {
        mat->data[i] = 0.0; 
    }
    return mat;
}

void free_matrix(const Matrix* mat) {
    if (mat != NULL) {
        free(mat->data);
        free((void*)mat); 
    }
}

void print_matrix(const Matrix* mat) {
    assert(mat != NULL);
    assert(mat->rows>0 && mat->cols>0);
    assert(mat->data != NULL);
    for(int i = 0; i < mat->rows; i++) {
        for(int j = 0; j < mat->cols; j++) {
            printf("%8.4f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

Matrix* matrix_add(const Matrix* A, const Matrix* B) {
    assert(A != NULL && B != NULL);
    assert(A->rows == B->rows && A->cols == B->cols);
    Matrix* C = new_matrix(A->rows, A->cols);
    for(int i = 0; i < A->rows * A->cols; i++) {
        C->data[i] = A->data[i] + B->data[i];
    }
    return C;
}

Matrix* matrix_subtract(const Matrix* A, const Matrix* B) {
    assert(A != NULL && B != NULL);
    assert(A->rows == B->rows && A->cols == B->cols);
    Matrix* C = new_matrix(A->rows, A->cols);
    for(int i = 0; i < A->rows * A->cols; i++) {
        C->data[i] = A->data[i] - B->data[i];
    }
    return C;
}

Matrix* matrix_multiply(const Matrix* A, const Matrix* B) {
    assert(A != NULL && B != NULL);
    assert(A->cols == B->rows);
    Matrix* C = new_matrix(A->rows, B->cols);
    for(int i = 0; i < A->rows; i++) {
        for(int j = 0; j < B->cols; j++) {
            double sum = 0.0;
            for(int k = 0; k < A->cols; k++) {
                sum += A->data[i * A->cols + k] * B->data[k * B->cols + j];
            }
            C->data[i * C->cols + j] = sum;
        }
    }
    return C;
}

Matrix* matrix_transpose(const Matrix* A) {
    assert(A != NULL);
    Matrix* At = new_matrix(A->cols, A->rows);
    for(int i = 0; i < A->rows; i++) {
        for(int j = 0; j < A->cols; j++) {
            At->data[j * At->cols + i] = A->data[i * A->cols + j];
        }
    }
    return At;
}


Vector* new_vector(int size) {
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    assert(vec != NULL);
    vec->size = size;
    vec->data = (double*)malloc(size * sizeof(double));
    assert(vec->data != NULL);
    for(int i = 0; i < size; i++) {
        vec->data[i] = 0.0;
    }
    return vec;
}

void free_vector(const Vector* vec) {
    if (vec != NULL) {
        free(vec->data);
        free((void*)vec);
    }
}

void print_vector(const Vector* vec) {
    assert(vec != NULL);
    assert(vec->size > 0);
    assert(vec->data != NULL);
    for(int i = 0; i < vec->size; i++) {
        printf("%8.4f\n", vec->data[i]);
    }
    printf("\n");
}

Vector* vector_add(const Vector* A, const Vector* B) {
    assert(A != NULL && B != NULL);
    assert(A->size == B->size);
    Vector* C = new_vector(A->size);
    for(int i = 0; i < A->size; i++) {
        C->data[i] = A->data[i] + B->data[i];
    }
    return C;
}

Vector* vector_subtract(const Vector* A, const Vector* B) {
    assert(A != NULL && B != NULL);
    assert(A->size == B->size);
    Vector* C = new_vector(A->size);
    for(int i = 0; i < A->size; i++) {
        C->data[i] = A->data[i] - B->data[i];
    }
    return C;
}

double vector_dot(const Vector* A, const Vector* B) {
    assert(A != NULL && B != NULL);
    assert(A->size == B->size);
    double sum = 0.0;
    for(int i = 0; i < A->size; i++) {
        sum += A->data[i] * B->data[i];
    }
    return sum;
}

Vector* matrix_vector_multiply(const Matrix* A, const Vector* x) {
    assert(A != NULL && x != NULL);
    assert(A->cols == x->size);
    Vector* b = new_vector(A->rows);
    for(int i = 0; i < A->rows; i++) {
        double sum = 0.0;
        for(int j = 0; j < A->cols; j++) {
            sum += A->data[i * A->cols + j] * x->data[j];
        }
        b->data[i] = sum;
    }
    return b;
}

Vector* solve(const Matrix* A, const Vector* b) {
    const int rows = A->rows;
    const int cols = A->cols;

    assert(rows == cols);
    assert(b->size == rows);

    //Creating copies of A and b to modify during Gaussian elimination without altering original A and b
    Matrix* M = new_matrix(rows, cols);
    Vector* rhs = new_vector(rows);
    Vector* x   = new_vector(rows);

    //Copy A into M
    for (int i = 0; i < rows * cols; i++) {
        M->data[i] = A->data[i];
    }

    //Copy b into rhs
    for (int i = 0; i < rows; i++) {
        rhs->data[i] = b->data[i];
    }

    // -------- Forward Elimination --------
    for (int k = 0; k < cols; k++) {

        // Find pivot row
        int max_row = k;
        for (int i = k + 1; i < rows; i++) {
            if (fabs(M->data[i * cols + k]) >
                fabs(M->data[max_row * cols + k])) {
                max_row = i;
            }
        }

        // Check singular
        if (fabs(M->data[max_row * cols + k]) < 1e-12) {
            fprintf(stderr, "Matrix is singular or nearly singular\n");
            free_matrix(M);
            free_vector(rhs);
            free_vector(x);
            return NULL;
        }

        // Swap rows if needed
        if (max_row != k) {
            for (int j = 0; j < cols; j++) {
                double temp = M->data[k * cols + j];
                M->data[k * cols + j] = M->data[max_row * cols + j];
                M->data[max_row * cols + j] = temp;
            }

            double temp_b = rhs->data[k];
            rhs->data[k] = rhs->data[max_row];
            rhs->data[max_row] = temp_b;
        }

        // Eliminate below pivot
        for (int i = k + 1; i < rows; i++) {
            double factor = M->data[i * cols + k] /
                            M->data[k * cols + k];

            for (int j = k; j < cols; j++) {
                M->data[i * cols + j] -=
                    factor * M->data[k * cols + j];
            }

            rhs->data[i] -= factor * rhs->data[k];
        }
    }

    // -------- Back Substitution --------
    for (int i = rows - 1; i >= 0; i--) {
        double sum = rhs->data[i];

        for (int j = i + 1; j < cols; j++) {
            sum -= M->data[i * cols + j] * x->data[j];
        }

        x->data[i] = sum / M->data[i * cols + i];
    }

    free_matrix(M);
    free_vector(rhs);

    return x;
}

double generate_normal(double sigma) {
    //Generate two uniform random numbers in (0,1)
    double u1 = (double)rand() / (double)RAND_MAX;
    double u2 = (double)rand() / (double)RAND_MAX;

    //Avoid log(0)
    if (u1 < 1e-12) {
        u1 = 1e-12;
    }

    //Box-Mullter Transform for Z ~ N(0,1)
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
    
    //Scale by sigma to get N(0, sigma^2)
    return sigma * z0;

}