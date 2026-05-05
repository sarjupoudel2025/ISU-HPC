#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "postpro.h"

static FILE *fp = NULL;

#define IDX(i,j,nx) ((j)*(nx) + (i))

// -------------------------------
// OPEN FILE
// -------------------------------
void postpro_init(const char *filename)
{
    fp = fopen(filename, "w");
    if (!fp) {
        printf("❌ Cannot open postpro.out\n");
        exit(1);
    }

    // header for Python / MATLAB / Pandas
    fprintf(fp,
        "step time u_center v_center p_center u_max v_max cfl\n"
    );
}

// -------------------------------
// WRITE DATA EACH STEP
// -------------------------------
void postpro_write(int step,
                   double time,
                   Field *f,
                   Mesh *mesh,
                   double dt)
{
    if (!fp) return;

    int nx = mesh->nx_global;
    int ny = mesh->ny_global;
    int N  = nx * ny;

    // -------------------------------
    // center probe
    // -------------------------------
    int cx = nx / 2;
    int cy = ny / 2;
    int c  = IDX(cx, cy, nx);

    double u_center = f->u[c];
    double v_center = f->v[c];
    double p_center = f->p[c];

    // -------------------------------
    // max velocity norms
    // -------------------------------
    double u_max = 0.0;
    double v_max = 0.0;

    for (int i = 0; i < N; i++) {

        double u = fabs(f->u[i]);
        double v = fabs(f->v[i]);

        if (u > u_max) u_max = u;
        if (v > v_max) v_max = v;
    }

    // -------------------------------
    // CFL (CORRECT FORM)
    // -------------------------------
    double cfl = fmax(
        u_max * dt / mesh->dx,
        v_max * dt / mesh->dy
    );

    // -------------------------------
    // WRITE LINE
    // -------------------------------
    fprintf(fp,
        "%d %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",
        step, time,
        u_center, v_center, p_center,
        u_max, v_max,
        cfl
    );
}

// -------------------------------
// CLOSE FILE
// -------------------------------
void postpro_close()
{
    if (fp) {
        fclose(fp);
        fp = NULL;
    }
}