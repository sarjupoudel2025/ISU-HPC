#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

Params read_params(const char *filename) {

    Params p;

    // -------------------------------
    // DEFAULT VALUES 
    // -------------------------------
    p.viscosity = 0.01;
    p.rho = 1.0;
    p.dp = 1.0;
    p.dt = 0.001;
    p.NT = 1000;

    p.output_every = 100;   
        p.vtk_enable = 1;   

    FILE *fp = fopen(filename, "r");

    if (!fp) {
        perror("Error opening parameter file");
        exit(EXIT_FAILURE);
    }

    char key[50];
    char dummy;

    while (fscanf(fp, "%49s", key) == 1) {

        // -------------------------------
        // SKIP COMMENTS (# ...)
        // -------------------------------
        if (key[0] == '#') {
            while ((dummy = fgetc(fp)) != '\n' && dummy != EOF);
            continue;
        }

        // -------------------------------
        // HANDLE '=' SYMBOL SAFELY
        // -------------------------------
        if (fscanf(fp, " %c", &dummy) == 1) {
            if (dummy != '=') {
                ungetc(dummy, fp);
            }
        }

        // -------------------------------
        // GRID PARAMETERS
        // -------------------------------
        if (strcmp(key, "nx") == 0) {
            if (fscanf(fp, "%d", &p.nx) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "ny") == 0) {
            if (fscanf(fp, "%d", &p.ny) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "lx") == 0) {
            if (fscanf(fp, "%lf", &p.lx) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "ly") == 0) {
            if (fscanf(fp, "%lf", &p.ly) != 1) exit(EXIT_FAILURE);
        }

        // -------------------------------
        // SOLVER PARAMETERS (GMRES)
        // -------------------------------
        else if (strcmp(key, "max_iter") == 0) {
            if (fscanf(fp, "%d", &p.max_iter) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "tolerance") == 0) {
            if (fscanf(fp, "%lf", &p.tol) != 1) exit(EXIT_FAILURE);
        }

        // -------------------------------
        // CFD PARAMETERS
        // -------------------------------
        else if (strcmp(key, "viscosity") == 0) {
            if (fscanf(fp, "%lf", &p.viscosity) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "rho") == 0) {
            if (fscanf(fp, "%lf", &p.rho) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "dp") == 0) {
            if (fscanf(fp, "%lf", &p.dp) != 1) exit(EXIT_FAILURE);
        }

        // -------------------------------
        // TIME STEPPING PARAMETERS
        // -------------------------------
        else if (strcmp(key, "dt") == 0) {
            if (fscanf(fp, "%lf", &p.dt) != 1) exit(EXIT_FAILURE);
        }
        else if (strcmp(key, "NT") == 0) {
            if (fscanf(fp, "%d", &p.NT) != 1) exit(EXIT_FAILURE);
        }
            else if (strcmp(key, "vtk_enable") == 0) {
                if (fscanf(fp, "%d", &p.vtk_enable) != 1) exit(EXIT_FAILURE);
            }
        else if (strcmp(key, "output_every") == 0) {  
            if (fscanf(fp, "%d", &p.output_every) != 1) exit(EXIT_FAILURE);
        }
    }

    fclose(fp);
    return p;
}