#ifndef LEC13_MATRIX_H
#define LEC13_MATRIX_H

#define PI 3.14159265358979323846

typedef struct {
    int rows;
    int cols;
    double* data;
} Matrix;

typedef struct {
    int size;
    double* data;
} Vector;

Matrix* new_matrix(const int rows, const int cols);
void free_matrix(const Matrix* mat);
void print_matrix(const Matrix* mat);
Matrix* matrix_add(const Matrix* A, const Matrix* B);
Matrix* matrix_subtract(const Matrix* A, const Matrix* B);
Matrix* matrix_multiply(const Matrix* A, const Matrix* B);
Matrix* matrix_transpose(const Matrix* A);
Matrix* matrix_inverse(const Matrix* A);

Vector* new_vector(int size);
void free_vector(const Vector* vec);
void print_vector(const Vector* vec);
Vector* vector_add(const Vector* A, const Vector* B);
Vector* vector_subtract(const Vector* A, const Vector* B);
double vector_dot(const Vector* A, const Vector* B);
Vector* matrix_vector_multiply(const Matrix* A, const Vector* x);

Vector* solve(const Matrix* A, const Vector* b);

double generate_normal(double sigma);
#endif // LEC13_MATRIX_H