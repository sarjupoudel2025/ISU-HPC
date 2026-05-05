#ifndef VTK_MPI_H
#define VTK_MPI_H

#include "mesh_mpi.h"
#include "initialize_mpi.h"
#include "mpi.h"

/*
 * Writes full CFD solution to VTK file:
 * - velocity (u, v)
 * - pressure (p)
 * - wall shear stress (wss)
 *
 * NOTE:
 * In MPI version, this is called ONLY on rank 0
 * after gathering global fields.
 */
void write_vtk(const char *filename,
               MPIMesh *mesh,
               double *u,
               double *v,
               double *p);

/*
 * Gathers distributed MPI field (local subdomain data)
 * into a full global array on rank 0.
 *
 * - local  : local rank data (size nx_local * ny_local)
 * - global : full array (ONLY valid on rank 0)
 * - m      : MPI mesh structure (contains decomposition info)
 *
 * NOTE:
 * Must be called by ALL ranks.
 */
void gather_field_to_root(double *local, double *global, MPIMesh *m);

void ensure_vtk_folder(const char *path);

#endif