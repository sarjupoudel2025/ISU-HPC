#ifndef MESH_MPI_H
#define MESH_MPI_H

#include <mpi.h>

/* ======================================
   MPI Mesh structure
   ====================================== */
typedef struct {

    /* Global grid size */
    int nx_global;
    int ny_global;

    /* Local grid size (including ghost cells) */
    int nx_local;
    int ny_local;

    /* Interior cells (excluding ghosts) */
    int nx_interior;

    /* Global index range (interior only) */
    int i_start;
    int i_end;

    /* Geometry */
    double lx, ly;
    double dx, dy;

    /* MPI info */
    int rank;
    int size;

    int rank_left;
    int rank_right;

    MPI_Comm comm;

} MPIMesh;


/* ======================================
   Function declarations
   ====================================== */

/* Create MPI mesh */
MPIMesh *create_mpi_mesh(int nx_global, int ny_global,
                         double lx, double ly,
                         MPI_Comm comm);

/* Destroy mesh */
void destroy_mpi_mesh(MPIMesh *m);

/* Halo exchange (left-right) */
void exchange_halo(double *u, MPIMesh *m);

/* Get global x-coordinate from local index */
double get_x(int i_local, MPIMesh *m);

#endif