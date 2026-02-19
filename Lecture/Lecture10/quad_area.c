/* Compute the area of a quadrilateral given the four vertex points
Input: a pointer to a quadrilateral structure with all four points defined */
#include "quadessentials.h"



void compute_area_quad(quadrilateral *quad) {

    //Break the quadrilateral into two triangles: ABC and ACD
    triangle tri1, tri2;
    tri1.A = quad->A;
    tri1.B = quad->B;
    tri1.C = quad->C;

    tri2.A = quad->A;
    tri2.B = quad->C;
    tri2.C = quad->D;

    compute_area_triangle(&tri1);
    compute_area_triangle(&tri2);
    quad->area = tri1.area + tri2.area;

}

/*
Essential Functions to compute the area*/

void compute_area_triangle(triangle *tri){
    double u[3], v[3], w[3];
    
    //AB = B - A vector u
    u[0] = tri->B.x - tri->A.x;
    u[1] = tri->B.y - tri->A.y;
    u[2] = 0;

    //AC = C - A vector v
    v[0] = tri->C.x - tri->A.x;
    v[1] = tri->C.y - tri->A.y;
    v[2] = 0;

    cross_product(u, v, w);

    /*fabs gives the magnitude of the cross product, 
    which is the area of the parallelogram formed by u and v. */
    tri->area = 0.5 * fabs(w[2]);

}

void cross_product(double u[3], double v[3], double w[3]) {
    /* Compute cross product of u and v, store in w 
    Magnitude of vector w gives the area of the parallelogram formed by u and v */
    w[0] = u[1]*v[2] - u[2]*v[1];
    w[1] = u[2]*v[0] - u[0]*v[2];
    w[2] = u[0]*v[1] - u[1]*v[0];
}



