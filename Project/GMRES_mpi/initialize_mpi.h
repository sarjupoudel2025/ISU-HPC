#ifndef INITIALIZE_MPI_H
#define INITIALIZE_MPI_H

#include <mpi.h>
#include "mesh_mpi.h"
#include "io_mpi.h"
#include "solver_mpi.h"

/* ======================================
   FULL SIMULATION STRUCT
   ====================================== */
typedef struct {

    Params params;

    MPIMesh *mesh;

    GMRESParams gmres;

    /* -------------------------------
       primary fields
       ------------------------------- */
    double *u;
    double *v;
    double *p;

    /* -------------------------------
       derived field arrays
       ------------------------------- */
    double *wss;

    /* -------------------------------
       intermediate velocity and RHS arrays
       ------------------------------- */
    double *u_star;
    double *v_star;
    double *b;

} Simulation;

/* ======================================
   API
   ====================================== */
Simulation *initialize_mpi(const char *param_file, MPI_Comm comm);

void destroy_simulation(Simulation *sim);

#endif