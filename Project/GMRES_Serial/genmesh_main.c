#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"
#include "field.h"
#include "io.h"
#include "vtk.h"

// 2D → 1D indexing macro
#define IDX(i,j,nx) ((j)*(nx) + (i))

int main(int argc, char *argv[]) {

    // -------------------------------
    // 1. Read parameter file
    // -------------------------------
    if (argc < 2) {
        printf("Usage: %s params.txt\n", argv[0]);
        return 1;
    }

    Params params = read_params(argv[1]);

    // -------------------------------
    // 2. Create mesh
    // -------------------------------
    Mesh *mesh = create_mesh(params.nx, params.ny,
                             params.lx, params.ly);

    printf("Mesh created:\n");
    printf("nx = %d, ny = %d\n", mesh->nx, mesh->ny);
    printf("dx = %f, dy = %f\n", mesh->dx, mesh->dy);

    // -------------------------------
    // 3. Create field
    // -------------------------------
    Field *field = create_field(mesh);

    // -------------------------------
    // 4. Initialize field (test case)
    // -------------------------------
    for (int j = 0; j < mesh->ny; j++) {
        for (int i = 0; i < mesh->nx; i++) {

            int id = IDX(i, j, mesh->nx);

            // Simple function for visualization
            field->phi[id] = mesh->x[i] * mesh->y[j];

            // RHS (for later solver use)
            field->rhs[id] = 1.0;
        }
    }

    // -------------------------------
    // 5. Write VTK file
    // -------------------------------
    write_vtk("mesh.vtk", mesh, field);
    printf("VTK file written: mesh.vtk\n");

    // -------------------------------
    // 6. Cleanup
    // -------------------------------
    free_field(field);
    free_mesh(mesh);

    return 0;
}
