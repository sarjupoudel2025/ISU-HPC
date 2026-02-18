/*
Expansion of Chebyshev Polynomials:
phi(x) E { 1, x, 2*x^1-1 , 4*x^3-3x, 8x^4-8x^2+1, 16x^5-20x^3+5x, . . .}
Then,
PN(x) = sum (i=0, N) b_i * phi_i(x),
for 0<=N<=5.
1. Using switch statement (with name chebyshev.c)
2. Plotting  the polynomials with Python, using system() to run
Python script from C.
3. Submit code, figures, and screenshots.
Update Git.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double my_chebychev_c(double x, int N) {
    double result;

    switch (N) {
        case 0:
            result = 1;
            break;
        case 1:
            result = x;
            break;
        case 2:
            result = 2 * pow(x, 2) - 1;
            break;
        case 3:
            result = 4 * pow(x, 3) - 3 * x;
            break;
        case 4:
            result = 8 * pow(x, 4) - 8 * pow(x, 2) + 1;
            break;
        case 5:
            result = 16 * pow(x, 5) - 20 * pow(x, 3) + 5 * x;
            break;
        default:
            printf("Invalid degree.\n");
            exit(1);
    }

    return result;

}

char* print_my_chebychev_c(int N) {
    switch (N) {
        case 0:
            return "1";
        case 1:
            return "x";
        case 2:
            return "2*x^2 - 1";
        case 3:
            return "4*x^3 - 3*x";
        case 4:
            return "8*x^4 - 8*x^2 + 1";
        case 5:
            return "16*x^5 - 20*x^3 + 5*x";
        default:
            return "Invalid degree.";
    }
}

int main(){

    // Get user input for degree of Chebyshev polynomial
    int N=-1;
    while(N < 0 || N > 5) {
        printf("Enter the degree of the Chebyshev polynomial (0-5): ");
        scanf("%d", &N);
        if (N < 0 || N > 5) {
            printf("Invalid input. Please enter a number between 0 and 5.\n");
        }
    }
    
    // Get user input for coefficients b_i
    //Container for coefficients b_i, initialized to 0
    double b_i[6]= {0}; 
    printf("Current Set Coefficients b_i: ");
    for (int i = 0; i <= N; i++) {
        printf("%lf ", b_i[i]);
    }
    printf("\n");

    for (int i = 0; i <= N; i++) {
        printf("Enter the coefficient b_%d: ", i);
        scanf("%lf", &b_i[i]);
    }

    printf("Updated Set Coefficients b_i: ");
    for (int i = 0; i <= N; i++) {
        printf("%lf ", b_i[i]);
    }
    printf("\n");
    printf("The Chebyshev polynomial of degree %i is ==> P_%d(x)= ",N, N);
    for (int i = 0; i <= N; i++) {
        if (b_i[i] != 0) {
            if (i > 0 && b_i[i] > 0) {
                printf("+ ");
            }
            printf("%lf*(%s) ", b_i[i], print_my_chebychev_c(i));
        }
    }
    printf("\n");
    printf("********* Chebyshev Polynomial Evaluation *********\n");

    double x;
    double LowerBound_x;
    double UpperBound_x;
   
    while (1) {
        printf("Enter the Bounds of x example [-1.0, 1.0]: \n");
        printf("Enter the lower bound of x: \n");
        scanf("%lf", &LowerBound_x);
        printf("Enter the upper bound of x: \n");
        scanf("%lf", &UpperBound_x);

        if (LowerBound_x >= UpperBound_x) {
            printf("Invalid input. Lower bound must be less than upper bound.\n");
        } else {
            break;
        }
    }

    printf("You Entered the Bounds of x: [%.2lf, %.2lf]\n", LowerBound_x, UpperBound_x);

    int num_points=-1;
    while (num_points <= 0) {
        printf("Enter the no. of points to evaluate: \n");
        scanf("%d", &num_points);
        if (num_points <= 0) {
            printf("Invalid input. Please enter a positive integer.\n");
        }
    }

    double step = (UpperBound_x - LowerBound_x) / (num_points - 1);
    //Evaluate the PN(x) at the specified points and print the results
    FILE *file = fopen("chebyshev_output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    for (int i = 0; i < num_points; i++) {
        x = LowerBound_x + i * step;
        double PN_x = 0;
        for (int j = 0; j <= N; j++) {
            PN_x += b_i[j] * my_chebychev_c(x, j);
    }
        fprintf(file, "%.2lf %.4lf\n", x, PN_x);
    }
    fclose(file);

    // Call Python script to plot the results
    system("python3 plot_Chebyshev.py");

        return 1;
    };
    

    

   
    
