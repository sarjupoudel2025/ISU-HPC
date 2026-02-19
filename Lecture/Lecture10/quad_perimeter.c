/* Compute the perimeter of a quadrilateral 
Input: a pointer to a quadrilateral structure with all four points defined */

#include "quadessentials.h"

void compute_perimeter_quad(quadrilateral *quad) {
    double AB, BC, CD, DA;
    compute_dist_two_points(quad->A, quad->B, &AB);
    compute_dist_two_points(quad->B, quad->C, &BC);
    compute_dist_two_points(quad->C, quad->D, &CD);
    compute_dist_two_points(quad->D, quad->A, &DA);

    quad->perimeter = AB + BC + CD + DA;
}

/* Essential function to compute the perimeter of quadrilateral */
void compute_dist_two_points(point p1, point p2, double *distance) {
    *distance = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}