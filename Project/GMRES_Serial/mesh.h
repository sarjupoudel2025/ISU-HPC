#ifndef MESH_H
#define MESH_H

typedef struct {

    // -------------------------------
    // Global grid
    // -------------------------------
    int nx_global, ny_global;

    // -------------------------------
    // Local grid 
    // -------------------------------
    int nx_local, ny_local;

    // -------------------------------
    // Geometry
    // -------------------------------
    double lx, ly;
    double dx, dy;

    // -------------------------------
    // Coordinates
    // -------------------------------
    double *x;
    double *y;

    // -------------------------------
    // MPI placeholders 
    // -------------------------------
    int rank, size;
    int px, py;

    int i_start, i_end;
    int j_start, j_end;

    int nghost;

} Mesh;

Mesh* create_mesh(int nx, int ny, double lx, double ly);
void free_mesh(Mesh *mesh);

#endif