/* Compute the angles of a quadrilateral given the four vertex points
Input: a pointer to a quadrilateral structure with all four points defined */
#include "quadessentials.h"

const double CUS_PI = 3.14159265358979323846;


void compute_angles_quad(quadrilateral *quad) {
    compute_angle_three_points(quad->B, quad->A, quad->D, &quad->angleA);
    compute_angle_three_points(quad->C, quad->B, quad->A, &quad->angleB);
    compute_angle_three_points(quad->D, quad->C, quad->B, &quad->angleC);
    compute_angle_three_points(quad->A, quad->D, quad->C, &quad->angleD);
}



/* Essentials for computing the angles at vertices of quadrilaterals*/
void compute_angle_three_points(point A, point B, point C, double *angle) {
    double AB, BC, AC;
    compute_dist_two_points(A, B, &AB);
    compute_dist_two_points(B, C, &BC);
    compute_dist_two_points(A, C, &AC);

    //Use the Law of Cosines to compute the angle at point B
    *angle = acos((AB*AB + BC*BC - AC*AC) / (2 * AB * BC)) * (180.0 / CUS_PI);
}