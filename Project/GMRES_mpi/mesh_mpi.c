#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mesh_mpi.h"

/* ================================
    Create MPI mesh (1D decomposition)
    ================================ */
MPIMesh *create_mpi_mesh(int nx_global, int ny_global,
                         double lx, double ly,
                         MPI_Comm comm)
{
    MPIMesh *m = (MPIMesh*)malloc(sizeof(MPIMesh));

    MPI_Comm_rank(comm, &m->rank);
    MPI_Comm_size(comm, &m->size);

    m->nx_global = nx_global;
    m->ny_global = ny_global;

    m->lx = lx;
    m->ly = ly;

    m->dx = lx / (nx_global - 1);
    m->dy = ly / (ny_global - 1);
    
    m->ny_local = ny_global;

    m->comm = comm;

    /* ---------------------------------
       Decompose interior points only
       --------------------------------- */
    int nx_interior = nx_global - 2;

    int base = nx_interior / m->size;
    int rem  = nx_interior % m->size;

    if (m->rank < rem) {
        m->nx_interior = base + 1;
        m->i_start = 1 + m->rank * (base + 1);
    } else {
        m->nx_interior = base;
        m->i_start = 1 + rem * (base + 1)
                       + (m->rank - rem) * base;
    }

    m->i_end = m->i_start + m->nx_interior - 1;

    /* ---------------------------------
       Local size (+2 ghost cells)
       --------------------------------- */
    m->nx_local = m->nx_interior + 2;

    /* ---------------------------------
       Neighbors
       --------------------------------- */
    m->rank_left  = (m->rank == 0) ? MPI_PROC_NULL : m->rank - 1;
    m->rank_right = (m->rank == m->size - 1) ? MPI_PROC_NULL : m->rank + 1;

    if (m->rank == 0) {
        printf("MPI mesh: %d ranks | global %d x %d\n",
               m->size, nx_global, ny_global);
    }

    printf("Rank %d: i=[%d,%d], nx_local=%d\n, dx=%f, dy=%f\n, lx=%f, ly=%f\n",
           m->rank, m->i_start, m->i_end, m->nx_local, m->dx, m->dy, m->lx, m->ly);


    return m;
}


/* ================================
   Halo exchange (LEFT-RIGHT)
   ================================ */
void exchange_halo(double *u, MPIMesh *m)
{
    int ny = m->ny_local;

    /* send left, recv right */
    MPI_Sendrecv(
        &u[1 * ny], ny, MPI_DOUBLE, m->rank_left, 0,
        &u[(m->nx_local - 1) * ny], ny, MPI_DOUBLE, m->rank_right, 0,
        m->comm, MPI_STATUS_IGNORE
    );

    /* send right, recv left */
    MPI_Sendrecv(
        &u[(m->nx_local - 2) * ny], ny, MPI_DOUBLE, m->rank_right, 1,
        &u[0], ny, MPI_DOUBLE, m->rank_left, 1,
        m->comm, MPI_STATUS_IGNORE
    );
}


/* ================================
   Utility: global x coordinate
   ================================ */
double get_x(int i_local, MPIMesh *m)
{
    int i_global = m->i_start + (i_local - 1);
    return i_global * m->dx;
}


/* ================================
   Free mesh
   ================================ */
void destroy_mpi_mesh(MPIMesh *m)
{
    free(m);
}