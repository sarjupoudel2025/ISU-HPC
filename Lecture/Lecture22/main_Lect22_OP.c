#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include <time.h>

int main() {
    long long int total_points = 10000000000;
    long long int inside_circle = 0;

    //omp_set_num_threads(10);
    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        long long int local_count = 0;

        #pragma omp for
        for (long long int i = 0; i < total_points; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

            if (x * x + y * y <= 1.0) {
                local_count++;
            }
        }

        #pragma omp atomic
        inside_circle += local_count;
    }

    double pi = 4.0 * inside_circle / total_points;
    printf("Estimated PI = %.10f\n", pi);

    return 0;
}