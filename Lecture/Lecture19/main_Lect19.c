/*Normalize a given signal of length N in 2-norm (energy of the signal) and
max norm (peak of the signal).
Try both find and coarse-grained parallelism.
Use reduction on sum and max if possible.
For N=1e8, with 1, 2, 4, 8, and 16 threads
Submit source codes, screenshots, and update Git. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

// =========================
// FINE-GRAINED VERSION
// =========================
void normalize_signal_fine(double* signal, int n, int thread_count) {
    double sum = 0.0;
    double max_val = 0.0;

    double t1 = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum) reduction(max:max_val) num_threads(thread_count)
    for (int i = 0; i < n; i++) {
        sum += signal[i] * signal[i];

        double val = fabs(signal[i]);
        if (val > max_val)
            max_val = val;
    }

    double norm_2 = sqrt(sum);

    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < n; i++) {
        signal[i] /= norm_2;
    }

    double t2 = omp_get_wtime();

    printf("[FINE] Threads = %d | N = %d | Time = %.6f sec\n",
           thread_count, n, t2 - t1);
}

// =========================
// COARSE-GRAINED VERSION
// =========================
void normalize_signal_coarse(double* signal, int n, int thread_count) {
    double sum = 0.0;
    double max_val = 0.0;

    double t1 = omp_get_wtime();

    #pragma omp parallel num_threads(thread_count)
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        long chunk = n / nthreads;
        long start = tid * chunk;
        long end = (tid == nthreads - 1) ? n : start + chunk;

        double local_sum = 0.0;
        double local_max = 0.0;

        for (long i = start; i < end; i++) {
            local_sum += signal[i] * signal[i];

            double val = fabs(signal[i]);
            if (val > local_max)
                local_max = val;
        }

        #pragma omp atomic
        sum += local_sum;

        #pragma omp critical
        {
            if (local_max > max_val)
                max_val = local_max;
        }
    }

    double norm_2 = sqrt(sum);

    #pragma omp parallel num_threads(thread_count)
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        long chunk = n / nthreads;
        long start = tid * chunk;
        long end = (tid == nthreads - 1) ? n : start + chunk;

        for (long i = start; i < end; i++) {
            signal[i] /= norm_2;
        }
    }

    double t2 = omp_get_wtime();

    printf("[COARSE] Threads = %d | N = %d | Time = %.6f sec\n",
           thread_count, n, t2 - t1);
}

// =========================
// MAIN FUNCTION
// =========================
int main(int argc, char *argv[]) {

    // Input: ./normalize <threads> <N>
    if (argc < 3) {
        printf("Usage: %s <threads> <N>\n", argv[0]);
        return 1;
    }

    int thread_count = atoi(argv[1]);
    int n = atoi(argv[2]);

    if (n <= 0) {
        printf("N must be positive\n");
        return 1;
    }

    double* signal1 = (double*) malloc(n * sizeof(double));
    double* signal2 = (double*) malloc(n * sizeof(double));

    if (signal1 == NULL || signal2 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize
    srand(33);
    for (int i = 0; i < n; i++) {
        double val = rand() / (double)RAND_MAX;
        signal1[i] = val;
        signal2[i] = val;
    }

    // Run both versions
    normalize_signal_fine(signal1, n, thread_count);
    normalize_signal_coarse(signal2, n, thread_count);

    free(signal1);
    free(signal2);

    return 0;
}