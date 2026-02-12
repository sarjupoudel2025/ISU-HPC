/*
1. Develop source codes ("main program") with name lab8.c define two functions (1)
computing factorial of an integer n and (2) computing exponential of a real number x.
* For computing factorial(n), define a function with recursive calls.
*For computing exp(x), define a function with Taylor expansions (recall Python codes), where, the constant
e= 2.718281828459, x0 = round(x) is the closest integert to x, and use the pow(.,.) from math.h
for computing powers. Use your own function.
*Generate a set of points x = 0, 0.02, 0.04, . . ., 0.98, 1, compute 
exp(x) at the given points (with your own function).
*Output the data to a data file, and visualize the exp(x) function 
with the data in python
2. Update Git repository
3. Submit codes, output data file and screenshots.
*/
#include <stdio.h>
#include <math.h>

// Function Declarations

unsigned long long my_c_factorial(int n);
long double my_c_exponential(double x);

int main() {
    //Test the factorial function
    int n = 5;
    printf("Factorial of %d is %llu\n", n, my_c_factorial(n));

    //Test the exponential function
    double x = 1.0;
    printf("Exponential of %.2f is %.12Lf\n", x, my_c_exponential(x));

    //Generate data points for exp(x) from x = 0 to 1 with step size 0.02
    FILE *file = fopen("exp_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    double step = 0.02;
    for (double x = 0.0; x <= 1.0; x += step) {
        long double exp_x = my_c_exponential(x);
        fprintf(file, "%.2f %.12Lf\n", x, exp_x);
    }
    fclose(file);
}

//Function Definitions
//Used Unsigned long long; since factorials are non-negative integers and can grow very large.
unsigned long long my_c_factorial(int n){
    if (n==0 || n==1) {
        return 1; // factorial of 0 or 1 is 1
    }
    return my_c_factorial(n-1) *n;
}

long double my_c_exponential(double x) {
    /*Taylor expansion of exponential function (This is expansion with x around 0)
    e^x = 1 + x + x^2/2! + x^3/3! + ...+ x^n/n!*/

    /*Taylor series expansion e^x about point x0 = round(x)
    e^x = e^x0 * e^(x - x0)
    e^x = e^x0 * e^(h) where h = (x - x0)
    Now, we can expand e^(h) using Taylor series about h = 0
    e^(h) = 1 + h + h^2/2! + h^3/3! + ... + h^n/n!
    Therefore,
    e^x = e^x0* [1 + (x - x0)/1! + (x - x0)^2/2! + (x - x0)^3/3! + ... + (x - x0)^n/n! ]
    where e^x0 can be computed using pow function from math.h
    */


    const long double e = 2.718281828459; // Constant e
    int x0 = round(x); // Closest integer to x
    long double exp_x0 = pow(e, x0); // Compute e^x0 using pow function
    long double h = x - x0; // Compute h
    
    //For Taylor series expansion of e^(h)
    long double term = 1.0; // First term of the series
    long double sum = term; // Initialize sum of series
    long double tolerance = 1e-10; // Set a tolerance level for convergence
    int max_iterations = 1000; // Set a maximum number of iterations to prevent infinite loops
    int iteration = 0; // Initialize iteration counter

    int n = 1; // Denominator of second term (x) is 1.  i.e. n (x^n / n!)

    while (fabsl(term) > tolerance) {
        term *= h / n; // Calculate the next term
        sum += term; // Add the next term to the sum
        n++; // Move to the next term index

        iteration++;
        if (iteration > max_iterations) {
            break; // Prevent infinite loop in case of non-convergence
        }
    }

    return exp_x0 * sum;
}