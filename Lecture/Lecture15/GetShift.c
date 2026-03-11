#include <stdlib.h>
#include <math.h>
#include "trimatrix.h"

static inline double sign(const double x)
{
   if (x<0.0)
   { return -1.0; }
   else
   { return  1.0; }
}

double GetShift(trimatrix* T)
{
   // Wilkinson shift
   int N = T->rows;
   double d  = (tget(T,N-1,N-1)-tget(T,N,N))/2.0;
   double ds = sign(d);
   double mu = tget(T,N,N)-(ds*pow(tget(T,N-1,N),2))
      /(fabs(d)+sqrt(pow(d,2)+pow(tget(T,N-1,N),2)));

   return mu;
}
