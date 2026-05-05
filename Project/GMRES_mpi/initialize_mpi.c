#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "initialize_mpi.h"
#include "mesh_mpi.h"
#include "io_mpi.h"

/* ======================================
   Initialize simulation (MPI version)
   ====================================== */
Simulation *initialize_mpi(const char *param_file, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    /* ======================================
       Allocate simulation container
       ====================================== */
    Simulation *sim = (Simulation*)malloc(sizeof(Simulation));
    if (!sim) {
        fprintf(stderr, "[Rank %d] ERROR: Failed to allocate Simulation\n", rank);
        MPI_Abort(comm, 1);
    }

    /* initialize pointers safely */
    sim->mesh = NULL;
    sim->u = sim->v = sim->p = NULL;
    sim->u_star = sim->v_star = sim->b = NULL;

    /* ======================================
       READ PARAMETERS (rank 0 only)
       ====================================== */
    Params p;

    if (rank == 0) {

        // printf("Reading parameter file: %s\n", param_file);

        p = read_params(param_file);

        /* -------------------------------
           BASIC VALIDATION CHECKS
           ------------------------------- */
        if (p.nx < 3 || p.ny < 3) {
            fprintf(stderr, "[Rank 0] ERROR: nx and ny must be >= 3\n");
            MPI_Abort(comm, 1);
        }

        if (p.dt <= 0.0) {
            fprintf(stderr, "[Rank 0] ERROR: dt must be positive\n");
            MPI_Abort(comm, 1);
        }

        if (p.viscosity <= 0.0) {
            fprintf(stderr, "[Rank 0] ERROR: viscosity must be positive\n");
            MPI_Abort(comm, 1);
        }
    }

    /* ======================================
       BROADCAST PARAMETERS
       ====================================== */
    MPI_Bcast(&p, sizeof(Params), MPI_BYTE, 0, comm);

    sim->params = p;

    /* ======================================
       INITIALIZE GMRES PARAMETERS
       ====================================== */
    sim->gmres.max_iter = p.max_iter;
    sim->gmres.tol      = p.tol;

    /* ======================================
       CREATE MPI MESH
       ====================================== */
    sim->mesh = create_mpi_mesh(
        sim->params.nx,
        sim->params.ny,
        sim->params.lx,
        sim->params.ly,
        comm
    );

   

    if (!sim->mesh) {
        fprintf(stderr, "[Rank %d] ERROR: Mesh creation failed\n", rank);
        MPI_Abort(comm, 1);
    }

    int nx = sim->mesh->nx_local;
    int ny = sim->mesh->ny_local;
    int Nloc = nx * ny;

    /* ======================================
       ALLOCATE FIELD VARIABLES
       ====================================== */
    sim->u      = (double*)calloc(Nloc, sizeof(double));
    sim->v      = (double*)calloc(Nloc, sizeof(double));
    sim->p      = (double*)calloc(Nloc, sizeof(double));
    sim->u_star = (double*)calloc(Nloc, sizeof(double));
    sim->v_star = (double*)calloc(Nloc, sizeof(double));
    sim->b      = (double*)calloc(Nloc, sizeof(double));

    /* ---------------------------------
       Allocation safety check
       --------------------------------- */
    if (!sim->u || !sim->v || !sim->p ||
        !sim->u_star || !sim->v_star || !sim->b)
    {
        fprintf(stderr, "[Rank %d] ERROR: Memory allocation failed\n", rank);
        MPI_Abort(comm, 1);
    }

    /* ======================================
       INITIAL CONDITIONS
       ====================================== */
    for (int i = 0; i < Nloc; i++) {
        sim->u[i] = 0.0;
        sim->v[i] = 0.0;
        sim->p[i] = 0.0;

        sim->u_star[i] = 0.0;
        sim->v_star[i] = 0.0;
        sim->b[i] = 0.0;
    }

    /* ======================================
       DEBUG INFO (rank 0 only)
       ====================================== */
    // if (rank == 0) {
    //     printf("\n======================================\n");
    //     printf("MPI INITIALIZATION COMPLETE\n");
    //     printf("======================================\n");
    //     printf("Ranks        : %d\n", size);
    //     printf("Global grid  : %d x %d\n", sim->mesh->nx_global, sim->mesh->ny_global);
    //     printf("Local grid   : %d x %d\n", sim->mesh->nx_local, sim->mesh->ny_local);
    //     printf("Domain size  : %f x %f\n", sim->mesh->lx, sim->mesh->ly);
    //     printf("dx, dy       : %f, %f\n", sim->mesh->dx, sim->mesh->dy);
    //     printf("dt, NT       : %e, %d\n",
    //            sim->params.dt, sim->params.NT);
    //     printf("GMRES        : max_iter=%d tol=%e\n",
    //            sim->gmres.max_iter, sim->gmres.tol);
    //     printf("======================================\n\n");
    // }
    

    return sim;
}

/* ======================================
   DESTROY SIMULATION
   ====================================== */
void destroy_simulation(Simulation *sim)
{
    if (!sim) return;

    free(sim->u);
    free(sim->v);
    free(sim->p);
    free(sim->u_star);
    free(sim->v_star);
    free(sim->b);

    destroy_mpi_mesh(sim->mesh);

    free(sim);
}