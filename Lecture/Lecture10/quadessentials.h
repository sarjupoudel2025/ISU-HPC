#ifndef __QUADESSENTIALS_H__
#define __QUADESSENTIALS_H__

#include <math.h>

typedef struct point point;
struct point {
    double x;
    double y;
};

typedef struct quadrilateral quadrilateral;
struct quadrilateral {
    point A;
    point B;
    point C;
    point D;

    double area;
    double perimeter;
    double angleA;
    double angleB;
    double angleC;
    double angleD;
};
void compute_perimeter_quad(quadrilateral *quad);
void compute_area_quad(quadrilateral *quad);
void compute_angles_quad(quadrilateral *quad);

typedef struct triangle triangle;
struct triangle {
    point A;
    point B;
    point C;
    
    double area;
    double perimeter;
    double angleA;
    double angleB;
    double angleC;
};
void compute_perimeter_triangle(triangle *tri);
void compute_area_triangle(triangle *tri);
void compute_angles_triangle(triangle *tri);

void cross_product(double u[3], double v[3], double w[3]);
void compute_dist_two_points(point p1, point p2, double *distance);
void compute_angle_three_points(point A, point B, point C, double *angle);


#endif