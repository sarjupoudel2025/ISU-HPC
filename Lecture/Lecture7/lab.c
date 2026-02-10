#include<stdio.h>
/* Develop a script ("main program") with name lab.c: computing factorial of an integer n,
exponentials of a real number x, and logarithm of a real number y

submit code and screenshot*/

int my_c_factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * my_c_factorial(n - 1);
}

double my_c_exponential(double x) {
    /*Taylor expansion of exponential function
    e^x = 1 + x + x^2/2! + x^3/3! + ...+ x^n/n!*/


    double term = 1.0; // First term of the series
    double sum = term; // Initialize sum of series
    double tolerance = 1e-10; // Set a tolerance level for convergence
    double max_iterations = 1000; // Set a maximum number of iterations to prevent infinite loops
    double iteration = 0; // Initialize iteration counter

    int n = 1; // Denominator of second term (x) is 1.  i.e. n (x^n / n!)

    while (term > tolerance) {
        term *= x / n; // Calculate the next term
        sum += term; // Add the next term to the sum
        n++; // Move to the next term index

        iteration++;
        if (iteration > max_iterations) {
            break; // Prevent infinite loop in case of non-convergence
        }
    }

    return sum;
}

double my_c_abs(double x) {
    return (x < 0) ? -x : x;
}


double my_c_natural_logarithm(double y) {
    /*Using Newton's iteration compute the logarithm of y
    compute x such that e^(x) = y
    */

    if (y <= 0) {
        printf("Error: Logarithm is undefined for non-positive numbers.\n");
        return -1; // Logarithm is undefined for non-positive numbers
    }

    //Initial Guess for x
    double x = y / 2.0; 
    double fx;

    //Tolerance level for convergence
    double tolerance = 1e-10;

    //Maximum number of iterations to prevent infinite loops
    int max_iterations = 1000;
    int iteration = 0;

    // Newton's iteration
    while (iteration < max_iterations) {
        double fx = my_c_exponential(x); // Compute e^x (f(x))
        double error = fx - y; // Compute the error
        double new_x = x - error / fx; // Update x using Newton's method

        if (my_c_abs(new_x - x) < tolerance) {
            break; // Convergence achieved
        }

        x = new_x; // Update x for the next iteration
        iteration++;
    }
    return x; // Return the computed logarithm
        
}

int main() {

    /*Test factorial, exponential and logarithm function programed in C*/
    int n = 5;
    double x = 2.0;
    double y = 7.389056; // Approximately e^2

    printf("Factorial of %d is %d\n", n, my_c_factorial(n));
    printf("Exponential of %.2f is %.5f\n", x, my_c_exponential(x));
    printf("Logarithm of %.6f is %.5f\n", y, my_c_natural_logarithm(y));
    printf("\n");


    /*Another Test Numbers*/
    int n2 = 8;
    double x2 = 3;
    double y2 = 20.085537; // Approximately e^3

    printf("Factorial of %d is %d\n", n2, my_c_factorial(n2));
    printf("Exponential of %.2f is %.5f\n", x2, my_c_exponential(x2));
    printf("Logarithm of %.6f is %.5f\n", y2, my_c_natural_logarithm(y2));
    printf("\n");

    int n3 = 0;
    double x3 = -1.0;
    double y3 = 0.367879; // Approximately e^-1

    printf("Factorial of %d is %d\n", n3, my_c_factorial(n3));
    printf("Exponential of %.2f is %.5f\n", x3, my_c_exponential(x3));
    printf("Logarithm of %.6f is %.5f\n", y3, my_c_natural_logarithm(y3));
    printf("\n");

    return 0;
}


