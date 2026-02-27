/* Develop a program with Stack (application to Bissection Method), update Git,
submit both source codes and screenshots*/
#include "BissectionMethod.h"

int main() {
    double a = 1.0; // Starting point a
    double b = 2.0; // Starting point b
    double tol = 0.000001; // Tolerance
    int maxIter = 100; // Maximum iterations

    bisectionMethod(a, b, tol, maxIter);

    return 0;
}