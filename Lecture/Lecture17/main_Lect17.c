#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

void save_img(const char *filename, int **data, int start_i, int start_j, int rows, int cols)
{
    FILE *filepointer = fopen(filename, "w");
    if (!filepointer) {
        printf("Error opening file %s\n", filename);
        return;
    }

    fprintf(filepointer, "P3\n%d %d\n255\n", cols, rows);

    for (int i = start_i; i < start_i + rows; i++) {
        for (int j = start_j; j < start_j + cols; j++) {
            int val = data[i][j];
            fprintf(filepointer, "%d %d %d\n", val, val, val);
        }
    }

    fclose(filepointer);
    printf("Saved: %s\n", filename);
}

int main(int argc, char *argv[])
{
    /*Arguments counts should be at least 3 
    First Argument: Program
    Second Argument: Number of threads (thread_count)
    Third Argument: Size of the image (N)
    */
    if (argc < 3) {
        printf("Usage: %s <threads> <N>\n", argv[0]);
        return 1;
    }

    int thread_count = atoi(argv[1]);
    int N = atoi(argv[2]);

    /*Validate the size of the image 
    Size of image has to be at least 3x3 for the blur operation to work,
    since we are using a 3x3 kernel. 
    */

    if (N < 3) {
        printf("N must be at least 3\n");
        return 1;
    }

    /* Allocate memory dynamically */
    int **img = malloc(N * sizeof(int *));
    int **blurred = malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++) {
        img[i] = malloc(N * sizeof(int));
        blurred[i] = malloc(N * sizeof(int));
    }

    /* Fill image with random values */
    srand(33);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            img[i][j] = rand() % 256;

    /* Apply 3x3 blur */
    double t1 = omp_get_wtime();

    #pragma omp parallel for collapse(2) num_threads(thread_count)
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            blurred[i][j] =
                (img[i-1][j-1] + img[i-1][j] + img[i-1][j+1] +
                 img[i  ][j-1] + img[i  ][j] + img[i  ][j+1] +
                 img[i+1][j-1] + img[i+1][j] + img[i+1][j+1]) / 9;
        }
    }

    double t2 = omp_get_wtime();

    printf("Threads = %d | N = %d | Walltime = %.6f sec\n",
           thread_count, N, t2 - t1);

    /* Save images */
    save_img("originalimage.ppm", img, 0, 0, N, N);
    save_img("blurredimage.ppm", blurred, 1, 1, N - 2, N - 2);

    /* Free memory */
    for (int i = 0; i < N; i++) {
        free(img[i]);
        free(blurred[i]);
    }
    free(img);
    free(blurred);

    return 0;
}
