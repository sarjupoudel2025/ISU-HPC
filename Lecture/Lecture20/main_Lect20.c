/*
Computing 1D Electrostatic Potential

V(x0) = ∫[a to b] rho(x) / |x - x0| dx
  = ∫[a to b] (rho(x) - rho(x0)) / |x - x0| dx
    + rho(x0) * ∫[a to b] 1 / |x - x0| dx
  = ∫[a to b] (rho(x) - rho(x0)) / |x - x0| dx
    + rho(x0) * log((b - x0) / (x0 - a))

for x0 in (a, b)

Choose:
    rho(x) = exp(-x^2)
    interval [a, b] = [-1, 1]

Task:
    Plot V(x0)
------------------------------------------------------------
Compute the Bessel function:

J0(x) = (1 / pi) * ∫[0 to pi] cos(x * sin(theta)) dtheta

for x in [0, 50]

Task:
    Plot J0(x)
------------------------------------------------------------
Submit source code, screenshots, and update Git.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define M_PI 3.14159265358979323846


// ---------- FUNCTION DECLARATIONS ----------
double AdaptiveInt(const double a, const double b, const double TOL,
                   double x0, int mode);

double Q(const double a, const double b,
         double x0, int mode);

double f(const double x, double x0, int mode);

// ---------- MAIN ----------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <num_threads> <TOL>\n", argv[0]);
        return 1;
    }

    int thread_count = strtol(argv[1], NULL, 10);
    double TOL = strtod(argv[2], NULL);

#ifdef _OPENMP
    omp_set_num_threads(thread_count);
#endif

    double start_time, end_time;

    // ==============================
    // PART 1: Compute V(x0)
    // ==============================
    int NV = 201;
    double x0_array[NV];
    double V_array[NV];

    start_time = omp_get_wtime();  // start timer for V

#pragma omp parallel for
    for (int i = 0; i < NV; i++)
    {
        double x0 = -0.999 + i * (1.998 / (NV - 1));
        double eps = 1e-6;

        double integral =
            AdaptiveInt(-1.0, x0 - eps, TOL / 2.0, x0, 0) +
            AdaptiveInt(x0 + eps, 1.0, TOL / 2.0, x0, 0);

        double rho0 = exp(-x0 * x0);
        double log_term = rho0 * log((1.0 - x0) / (x0 + 1.0));

        x0_array[i] = x0;
        V_array[i] = integral + log_term;
    }

    end_time = omp_get_wtime();  // end timer for V
    printf("Wall time for V(x0) computation = %f seconds\n", end_time - start_time);

    // Write to file AFTER computation
    FILE* fileV = fopen("Vx0.dat", "w");
    for (int i = 0; i < NV; i++)
        fprintf(fileV, "%lf %lf\n", x0_array[i], V_array[i]);
    fclose(fileV);


    // ==============================
    // PART 2: Compute J0(x)
    // ==============================
    int NJ = 501;
    double x_array[NJ];
    double J_array[NJ];

    start_time = omp_get_wtime();  // start timer for J0

#pragma omp parallel for
    for (int i = 0; i < NJ; i++)
    {
        double x = i * (50.0 / (NJ - 1));
        double integral = AdaptiveInt(0.0, M_PI, TOL, x, 1);

        x_array[i] = x;
        J_array[i] = integral / M_PI;
    }

    end_time = omp_get_wtime();  // end timer for J0
    printf("Wall time for J0(x) computation = %f seconds\n", end_time - start_time);

    // Write to file AFTER computation
    FILE* fileJ = fopen("J0.dat", "w");
    for (int i = 0; i < NJ; i++)
        fprintf(fileJ, "%lf %lf\n", x_array[i], J_array[i]);
    fclose(fileJ);

    printf("Output files written: Vx0.dat, J0.dat\n");

    return 0;
}
// ---------- ADAPTIVE INTEGRATION ----------
double AdaptiveInt(const double a, const double b, const double TOL,
                   double x0, int mode)
{
    double Qab = Q(a, b, x0, mode);
    double c = 0.5 * (a + b);
    double Qac = Q(a, c, x0, mode);
    double Qcb = Q(c, b, x0, mode);

    double error_est = fabs(Qac + Qcb - Qab) / 15.0;

    if (error_est < TOL)
    {
        return Qac + Qcb;
    }
    else
    {
        return AdaptiveInt(a, c, TOL / 2.0, x0, mode) +
               AdaptiveInt(c, b, TOL / 2.0, x0, mode);
    }
}

// ---------- SIMPSON RULE ----------
double Q(const double a, const double b,
         double x0, int mode)
{
    double c = 0.5 * (a + b);
    return (b - a) / 6.0 *
           (f(a, x0, mode) +
            4.0 * f(c, x0, mode) +
            f(b, x0, mode));
}

// ---------- INTEGRAND ----------
double f(const double x, double x0, int mode)
{
    if (mode == 0)
    {
        // Electrostatic potential
        double rho = exp(-x * x);
        double rho0 = exp(-x0 * x0);

        double dx = x - x0;

        if (fabs(dx) < 1e-8)
            return 0.0;

        return (rho - rho0) / fabs(dx);
    }
    else
    {
        // Bessel function
        return cos(x0 * sin(x));
    }
}