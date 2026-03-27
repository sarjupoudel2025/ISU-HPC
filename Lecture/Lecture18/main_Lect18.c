#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif


#define N 10000000  // must be even

const double MathPI = 3.14159265358979323846;
// Parameters
const double S0 = 100.0;
const double K  = 100.0;
const double r  = 0.05;
const double T  = 1.0;
const double sigma = 0.2;

// Probability density function p(S)
double pdf(double S) {
    double mu = log(S0) + (r - 0.5 * sigma * sigma) * T;
    double denom = S * sigma * sqrt(2.0 * MathPI * T);
    double exponent = -pow(log(S) - mu, 2) / (2.0 * sigma * sigma * T);
    return exp(exponent) / denom;
}

// Integrand f(S) = (S-K)*p(S)
double f(double S) {
    return (S - K) * pdf(S);
}

// Simpson’s Rule with OpenMP
double simpson_parallel(int n, int threads) {
    double Smax = 5.0 * S0;
    double a = K;
    double b = Smax;
    double h = (b - a) / n;

    double sum = 0.0;

    omp_set_num_threads(threads);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < n; i++) {
        double S = a + i * h;
        if (i % 2 == 0)
            sum += 2.0 * f(S);
        else
            sum += 4.0 * f(S);
    }

    double result = (f(a) + f(b) + sum) * h / 3.0;
    return exp(-r * T) * result;
}

// Black-Scholes formula
double normal_cdf(double x) {
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

double black_scholes() {
    double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T)
                / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    return S0 * normal_cdf(d1) - K * exp(-r * T) * normal_cdf(d2);
}

void usage(const char* prog_name) {
    printf("Usage: %s <thread_count>\n", prog_name);
    printf("thread_count must be >= 1\n");
    exit(0);
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        usage(argv[0]);
    }

    const int thread_count = strtol(argv[1], NULL, 10);

    if (thread_count < 1) {
        usage(argv[0]);
    }

    printf("Black-Scholes Price: %.10f\n\n", black_scholes());

    double time1 = omp_get_wtime();
    double price = simpson_parallel(N, thread_count);
    double time2 = omp_get_wtime();

    printf("Threads: %d | Price: %.10f | Time: %f sec\n",
           thread_count, price, time2 - time1);

    return 0;
}


