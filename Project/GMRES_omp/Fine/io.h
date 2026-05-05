#ifndef IO_H
#define IO_H

typedef struct {

    // -------------------------------
    // GRID PARAMETERS
    // -------------------------------
    int nx, ny;          // number of grid points in x and y directions
    double lx, ly;       // physical domain size

    // -------------------------------
    // SOLVER PARAMETERS (GMRES)
    // -------------------------------
    int max_iter;        // maximum iterations for GMRES pressure solve
    double tol;          // convergence tolerance

    // -------------------------------
    // CFD PHYSICS PARAMETERS
    // -------------------------------
    double viscosity;    // ν: kinematic viscosity (controls diffusion)
    double rho;          // density (used in scaling pressure term)
    double dp;           // imposed pressure difference (reference forcing)

    // -------------------------------
    // TIME STEPPING PARAMETERS
    // -------------------------------
    double dt;           // time step size (must satisfy CFL condition)
    int NT;              // total number of time steps

    // -------------------------------
    // OUTPUT CONTROL
    // -------------------------------
    int vtk_enable;       // 1 = write VTK output, 0 = disable VTK output
    int output_every;     // write VTK every N steps

} Params;

// -------------------------------
// FUNCTION: read parameter file
// -------------------------------
Params read_params(const char *filename);

#endif