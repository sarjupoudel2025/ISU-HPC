#include <stdio.h>
#include "vtk.h"

#define IDX(i,j,nx) ((j)*(nx) + (i))

void write_vtk(const char *filename, Mesh *mesh, Field *field) {

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error writing VTK file\n");
        return;
    }

    // -------------------------------
    // Use GLOBAL mesh size 
    // -------------------------------
    int nx = mesh->nx_global;
    int ny = mesh->ny_global;
    int N  = nx * ny;

    // -------------------------------
    // Header
    // -------------------------------
    fprintf(fp, "# vtk DataFile Version 3.0\n");
    fprintf(fp, "2D CFD Solution (u,v,p,wss)\n");
    fprintf(fp, "ASCII\n");
    fprintf(fp, "DATASET STRUCTURED_GRID\n");

    fprintf(fp, "DIMENSIONS %d %d 1\n", nx, ny);
    fprintf(fp, "POINTS %d float\n", N);

    // -------------------------------
    // Coordinates
    // -------------------------------
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            // int id = IDX(i, j, nx);  // unused

            fprintf(fp, "%f %f %f\n",
                    mesh->x[i],
                    mesh->y[j],
                    0.0);
        }
    }

    // -------------------------------
    // Write CFD fields
    // -------------------------------
    fprintf(fp, "\nPOINT_DATA %d\n", N);

    // -------------------------------
    // Velocity magnitude or components
    // -------------------------------
    fprintf(fp, "VECTORS velocity float\n");
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%f %f %f\n",
                field->u[i],
                field->v[i],
                0.0);
    }

    // -------------------------------
    // Pressure field
    // -------------------------------
    fprintf(fp, "\nSCALARS pressure float 1\n");
    fprintf(fp, "LOOKUP_TABLE default\n");
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%f\n", field->p[i]);
    }

    // -------------------------------
    // Wall shear stress
    // -------------------------------
    fprintf(fp, "\nSCALARS wall_shear_stress float 1\n");
    fprintf(fp, "LOOKUP_TABLE default\n");
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%f\n", field->wss[i]);
    }

    fclose(fp);
}