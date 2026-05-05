#include <math.h>
#include <string.h>
#include "mpi_linalg.h"

double mpi_dot_product(const double *x, const double *y, int n_local, MPI_Comm comm) {
    double local_dot = 0.0;
    for (int i = 0; i < n_local; i++) {
        local_dot += x[i] * y[i];
    }
    
    double global_dot = 0.0;
    MPI_Allreduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, comm);
    return global_dot;
}

double mpi_norm2(const double *x, int n_local, MPI_Comm comm) {
    double local_norm_sq = 0.0;
    for (int i = 0; i < n_local; i++) {
        local_norm_sq += x[i] * x[i];
    }
    
    double global_norm_sq = 0.0;
    MPI_Allreduce(&local_norm_sq, &global_norm_sq, 1, MPI_DOUBLE, MPI_SUM, comm);
    return sqrt(global_norm_sq);
}

void vec_copy(const double *src, double *dst, int n) {
    memcpy(dst, src, n * sizeof(double));
}

void vec_scale(double *x, double alpha, int n) {
    for (int i = 0; i < n; i++) {
        x[i] *= alpha;
    }
}

void vec_axpy(double alpha, const double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] += alpha * x[i];
    }
}

/* Interior-only versions (exclude ghost cells) */
double mpi_dot_product_interior(const double *x, const double *y, int n_interior, 
                                 MPIMesh *mmesh, MPI_Comm comm) {
    double local_dot = 0.0;
    
    int nx_local = mmesh->nx_local;
    int ny = mmesh->ny_local;
    
    /* Calculate correct loop ranges: exclude ghost cells on non-boundary sides */
    int i_local_start = (mmesh->rank_left < 0) ? 0 : 1;  /* exclude left ghost if not on boundary */
    int i_local_end = (mmesh->rank_right < 0) ? nx_local : nx_local - 1;  /* exclude right ghost if not on boundary */
    
    for (int j = 0; j < ny; j++) {
        for (int i = i_local_start; i < i_local_end; i++) {
            int idx = j * nx_local + i;
            local_dot += x[idx] * y[idx];
        }
    }
    
    double global_dot = 0.0;
    MPI_Allreduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, comm);
    return global_dot;
}

double mpi_norm2_interior(const double *x, int n_interior, MPIMesh *mmesh, MPI_Comm comm) {
    double local_norm_sq = 0.0;
    
    int nx_local = mmesh->nx_local;
    int ny = mmesh->ny_local;
    
    /* Calculate correct loop ranges: exclude ghost cells on non-boundary sides */
    int i_local_start = (mmesh->rank_left < 0) ? 0 : 1;
    int i_local_end = (mmesh->rank_right < 0) ? nx_local : nx_local - 1;
    
    for (int j = 0; j < ny; j++) {
        for (int i = i_local_start; i < i_local_end; i++) {
            int idx = j * nx_local + i;
            local_norm_sq += x[idx] * x[idx];
        }
    }
    
    double global_norm_sq = 0.0;
    MPI_Allreduce(&local_norm_sq, &global_norm_sq, 1, MPI_DOUBLE, MPI_SUM, comm);
    return sqrt(global_norm_sq);
}

void vec_copy_interior(const double *src, double *dst, int n_interior, MPIMesh *mmesh) {
    int nx_local = mmesh->nx_local;
    int ny = mmesh->ny_local;
    
    /* Copy only non-ghost indices AND zero out ghost cells */
    int i_local_start = (mmesh->rank_left < 0) ? 0 : 1;
    int i_local_end = (mmesh->rank_right < 0) ? nx_local : nx_local - 1;
    
    for (int j = 0; j < ny; j++) {
        /* Zero left ghost if not on boundary */
        if (mmesh->rank_left >= 0) {
            dst[j * nx_local + 0] = 0.0;
        }
        
        /* Copy interior cells */
        for (int i = i_local_start; i < i_local_end; i++) {
            int idx = j * nx_local + i;
            dst[idx] = src[idx];
        }
        
        /* Zero right ghost if not on boundary */
        if (mmesh->rank_right >= 0) {
            dst[j * nx_local + (nx_local - 1)] = 0.0;
        }
    }
}

void vec_axpy_interior(double alpha, const double *x, double *y, int n_interior, MPIMesh *mmesh) {
    int nx_local = mmesh->nx_local;
    int ny = mmesh->ny_local;
    
    /* y += alpha * x for interior cells only; preserve boundary values */
    int i_local_start = (mmesh->rank_left < 0) ? 0 : 1;
    int i_local_end = (mmesh->rank_right < 0) ? nx_local : nx_local - 1;
    
    for (int j = 0; j < ny; j++) {
        for (int i = i_local_start; i < i_local_end; i++) {
            int idx = j * nx_local + i;
            y[idx] += alpha * x[idx];
        }
    }
}

void vec_zero_ghosts(double *v, MPIMesh *mmesh) {
    int nx_local = mmesh->nx_local;
    int ny = mmesh->ny_local;
    
    /* Zero ghost cells but preserve boundary values */
    for (int j = 0; j < ny; j++) {
        /* Zero left ghost if not on boundary */
        if (mmesh->rank_left >= 0) {
            v[j * nx_local + 0] = 0.0;
        }
        
        /* Zero right ghost if not on boundary */
        if (mmesh->rank_right >= 0) {
            v[j * nx_local + (nx_local - 1)] = 0.0;
        }
    }
}
