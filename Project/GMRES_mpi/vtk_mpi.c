#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "vtk_mpi.h"
#include <sys/stat.h>
#include <errno.h>


#define IDX_XY(i,j,ny) ((i)*(ny) + (j))

// =======================================================
// VTK WRITER (rank 0 only)
// =======================================================
void write_vtk(const char *filename,
               MPIMesh *mesh,
               double *u,
               double *v,
               double *p)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error writing VTK file\n");
        return;
    }

    int nx = mesh->nx_global;
    int ny = mesh->ny_global;
    int N  = nx * ny;

    fprintf(fp, "# vtk DataFile Version 3.0\n");
    fprintf(fp, "2D CFD Solution (MPI)\n");
    fprintf(fp, "ASCII\n");
    fprintf(fp, "DATASET STRUCTURED_GRID\n");

    fprintf(fp, "DIMENSIONS %d %d 1\n", nx, ny);
    fprintf(fp, "POINTS %d float\n", N);

    // coordinates
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            fprintf(fp, "%f %f %f\n",
                    i * mesh->dx,
                    j * mesh->dy,
                    0.0);
        }
    }

    fprintf(fp, "\nPOINT_DATA %d\n", N);

    // velocity
    fprintf(fp, "VECTORS velocity float\n");
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            int id = IDX_XY(i, j, ny);
            fprintf(fp, "%f %f %f\n", u[id], v[id], 0.0);
        }
    }

    // pressure
    fprintf(fp, "\nSCALARS pressure float 1\n");
    fprintf(fp, "LOOKUP_TABLE default\n");
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            int id = IDX_XY(i, j, ny);
            fprintf(fp, "%f\n", p[id]);
        }
    }

    fclose(fp);
}


void gather_field_to_root(double *local,
                          double *global,
                          MPIMesh *m)
{
    int rank = m->rank;
    int size = m->size;
    int ny   = m->ny_global;   // global y

    int add_left  = (rank == 0) ? 1 : 0;
    int add_right = (rank == size - 1) ? 1 : 0;

    int nx_send = m->nx_interior + add_left + add_right;
    int i0_local = (rank == 0) ? 0 : 1;
    int i0_global = (rank == 0) ? 0 : m->i_start;

    int local_size = nx_send * ny;

    int *counts = NULL;
    int *displs = NULL;
    int *starts = NULL;

    if (rank == 0) {
        counts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));
        starts = malloc(size * sizeof(int));
    }

    MPI_Gather(&local_size, 1, MPI_INT,
               counts, 1, MPI_INT,
               0, m->comm);

    MPI_Gather(&i0_global, 1, MPI_INT,
               starts, 1, MPI_INT,
               0, m->comm);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            displs[i] = starts[i] * ny;
        }
    }

    MPI_Gatherv(&local[i0_local * ny], local_size, MPI_DOUBLE,
                global, counts, displs, MPI_DOUBLE,
                0, m->comm);

    if (rank == 0) {
        free(counts);
        free(displs);
        free(starts);
    }
}


void ensure_vtk_folder(const char *path)
{
    struct stat st = {0};

    if (stat(path, &st) == 0) {
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "Error: %s exists but is not a directory\n", path);
        }
        return;
    }

    if (mkdir(path, 0755) != 0) {
        if (errno != EEXIST) {
            perror("Failed to create output directory");
        }
    }
}