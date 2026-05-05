#include <stdlib.h>
#include "mesh.h"
#include "field.h"

Field* create_field(const Mesh *mesh) {

    int N = mesh->nx_global * mesh->ny_global;

    Field *f = (Field*)malloc(sizeof(Field));
    if (!f) return NULL;

    // -------------------------------
    // Velocity field
    // -------------------------------
    f->u = (double*)calloc(N, sizeof(double));
    f->v = (double*)calloc(N, sizeof(double));

    // -------------------------------
    // Pressure field
    // -------------------------------
    f->p = (double*)calloc(N, sizeof(double));

    // -------------------------------
    // Wall shear stress
    // -------------------------------
    f->wss = (double*)calloc(N, sizeof(double));

    // -------------------------------
    // Safety check
    // -------------------------------
    if (!f->u || !f->v || !f->p || !f->wss) {
        free(f->u);
        free(f->v);
        free(f->p);
        free(f->wss);
        free(f);
        return NULL;
    }

    return f;
}

void free_field(Field *f) {

    if (!f) return;

    free(f->u);
    free(f->v);
    free(f->p);
    free(f->wss);

    free(f);
}