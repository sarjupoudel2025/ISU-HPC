#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    long long int N;
    long long int local_count = 0, global_count = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // number of processes

    // Root takes input
    if (rank == 0) {
        printf("Enter total number of points: ");
        scanf("%lld", &N);
    }

    // Broadcast N to all processes
    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    long long int local_points = N / size;

    // Unique seed per process
    unsigned int seed = time(NULL) + rank;

    for (long long int i = 0; i < local_points; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    // Reduce all local counts into global count at root
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4.0 * global_count / N;
        printf("Estimated PI = %.10f\n", pi);
    }

    MPI_Finalize();
    return 0;
}