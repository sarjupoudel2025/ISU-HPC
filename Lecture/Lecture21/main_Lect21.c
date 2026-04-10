#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int token;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    // Rank 0 starts the token
    if (rank == 0) {
        token = 1000;
        printf("Rank %d initialized token %d\n", rank, token);

        // Send to Rank 1
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Rank %d sent token %d to Rank %d\n", rank, token, next);

        // Receive from last rank
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received token %d from Rank %d\n", rank, token, prev);

        printf("Final token value at Rank 0: %d\n", token);
    } else {
        // Receive from previous rank
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received token %d from Rank %d\n", rank, token, prev);

        // Increment token
        token++;

        // Send to next rank
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Rank %d sent token %d to Rank %d\n", rank, token, next);
    }

    MPI_Finalize();
    return 0;
}