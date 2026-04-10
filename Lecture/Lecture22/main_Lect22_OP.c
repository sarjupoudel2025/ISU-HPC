#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <threads> <total_points>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    long long int total_points = atoll(argv[2]);

    long long int inside_circle = 0;

    omp_set_num_threads(num_threads);

    double start_time = omp_get_wtime();

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

    double end_time = omp_get_wtime();
    double wall_time = end_time - start_time;

    double pi = 4.0 * inside_circle / total_points;

    printf("Threads used     : %d\n", num_threads);
    printf("Total N          : %lld\n", total_points);
    printf("Estimated PI     : %.10f\n", pi);
    printf("Wall time (sec)  : %f\n", wall_time);

    return 0;
}