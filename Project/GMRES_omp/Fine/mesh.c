#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"

Mesh* create_mesh(int nx, int ny, double lx, double ly) {

    if (nx < 2 || ny < 2) {
        printf("Invalid mesh size\n");
        return NULL;
    }

    Mesh *mesh = (Mesh*)calloc(1, sizeof(Mesh));  // 
    if (!mesh) {
        printf("Failed to allocate mesh\n");
        return NULL;
    }

    // -------------------------------
    // Global grid size
    // -------------------------------
    mesh->nx_global = nx;
    mesh->ny_global = ny;

    // -------------------------------
    // Local grid size 
    // -------------------------------
    mesh->nx_local = nx;
    mesh->ny_local = ny;

    // -------------------------------
    // Geometry
    // -------------------------------
    mesh->lx = lx;
    mesh->ly = ly;

    mesh->dx = lx / (nx - 1);
    mesh->dy = ly / (ny - 1);

    // -------------------------------
    // MPI placeholders 
    // -------------------------------
    mesh->rank = 0;
    mesh->size = 1;

    mesh->px = 1;
    mesh->py = 1;

    mesh->i_start = 0;
    mesh->i_end   = nx - 1;

    mesh->j_start = 0;
    mesh->j_end   = ny - 1;

    mesh->nghost = 1;

    // -------------------------------
    // Coordinate arrays
    // -------------------------------
    mesh->x = (double*)malloc(nx * sizeof(double));
    mesh->y = (double*)malloc(ny * sizeof(double));

    if (!mesh->x || !mesh->y) {
        printf("Failed to allocate mesh coordinates\n");
        free(mesh->x);
        free(mesh->y);
        free(mesh);
        return NULL;
    }

    for (int i = 0; i < nx; i++)
        mesh->x[i] = i * mesh->dx;

    for (int j = 0; j < ny; j++)
        mesh->y[j] = j * mesh->dy;

    return mesh;
}

void free_mesh(Mesh *mesh) {
    if (!mesh) return;

    free(mesh->x);
    free(mesh->y);
    free(mesh);
}