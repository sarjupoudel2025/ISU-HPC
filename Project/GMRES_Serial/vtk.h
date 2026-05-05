#ifndef VTK_H
#define VTK_H

#include "mesh.h"
#include "field.h"

/*
 * Writes full CFD solution to VTK file:
 * - velocity (u, v)
 * - pressure (p)
 * - wall shear stress (wss)
 */
void write_vtk(const char *filename, Mesh *mesh, Field *field);

#endif