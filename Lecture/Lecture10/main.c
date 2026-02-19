/*
Develop source codes main.c (with main()) for computing area, perimeter and 
inner angles of an imput quadrilatera, use/declare struct in a header file, define functions
for computing area, perimeter and inner angles in separated files
(quad_perimeter.c,  quad_area.c and quad_angles.c), and call them in main().
1. Try compile multiple files together to single executable (main.exe)
2. Try compile multiple files to Objects(*.o) and link them to single executable (main.exe)
3. Update Git Repository
4. submit source codes and screenshots.*/

#include <stdio.h>
#include "quadessentials.h"

int main() {
    quadrilateral quad1;
    quadrilateral quad2;
    

    /*Input the coordinates of the quadrilateral vertices
    For example, let's input a quadrilateral with vertices at:
    A(0, 0), B(4, 0), C(4, 3), D(0, 4) */
    quad1.A.x = 0; quad1.A.y = 0;
    quad1.B.x = 4; quad1.B.y = 0;
    quad1.C.x = 4; quad1.C.y = 3;
    quad1.D.x = 0; quad1.D.y = 4;

    /* New example quadrilateral
       A(1, 1), B(5, 2), C(4, 6), D(0, 4) */

    quad2.A.x = 1; quad2.A.y = 1;
    quad2.B.x = 5; quad2.B.y = 2;
    quad2.C.x = 4; quad2.C.y = 6;
    quad2.D.x = 0; quad2.D.y = 4;

    // Select which quadrilateral to compute properties for
    quadrilateral quad = quad1;

    //Compute area, perimeter and angles
    compute_area_quad(&quad);
    compute_perimeter_quad(&quad);
    compute_angles_quad(&quad);

    // Draw the quadrilateral 
    printf("\nQuadrilateral Shape:\n\n");

    printf("        D(%.2lf, %.2lf) -------- C(%.2lf, %.2lf)\n",
        quad.D.x, quad.D.y,
        quad.C.x, quad.C.y);

    printf("           |                          |\n");
    printf("           |                          |\n");

    printf("        A(%.2lf, %.2lf) -------- B(%.2lf, %.2lf)\n\n",
        quad.A.x, quad.A.y,
        quad.B.x, quad.B.y);



    //Output results
    printf("\nQuadrilateral Properties:\n");
    printf("Area: %.2lf\n", quad.area);
    printf("Perimeter: %.2lf\n", quad.perimeter);
    printf("Angle A: %.2lf degrees\n", quad.angleA);
    printf("Angle B: %.2lf degrees\n", quad.angleB);
    printf("Angle C: %.2lf degrees\n", quad.angleC);
    printf("Angle D: %.2lf degrees\n", quad.angleD);

    return 0;
}