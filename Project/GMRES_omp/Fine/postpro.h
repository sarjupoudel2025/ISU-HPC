#ifndef POSTPRO_H
#define POSTPRO_H

#include "field.h"
#include "mesh.h"

// -------------------------------
// POSTPROCESSING API
// -------------------------------
void postpro_init(const char *filename);

void postpro_write(int step,
                   double time,
                   Field *f,
                   Mesh *mesh,
                   double dt);

void postpro_close();

#endif