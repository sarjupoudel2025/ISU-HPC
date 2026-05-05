#ifndef FIELD_H
#define FIELD_H

#include "mesh.h"

// -------------------------------
// CFD Field (Navier–Stokes ready)
// -------------------------------
typedef struct {

    // Velocity components
    double *u;   // x-velocity
    double *v;   // y-velocity

    // Pressure field
    double *p;

    // Derived quantity (wall shear stress)
    double *wss;

} Field;

// -------------------------------
// Allocation / cleanup
// -------------------------------
Field* create_field(const Mesh *mesh);
void free_field(Field *f);

#endif