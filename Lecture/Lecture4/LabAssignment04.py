"""
This moducle contains functions for Lab Assignment 04 of Lecture 4.
Develop your own version of sqrt function using the Newton's Method
Start simple and add complexity in stages
Illustrate some Python programming
Illustrate use of git to track out development
Develop your own exponential function as well using own factorial function
then use it to find P(t) =  K / (1+ (K-P0)/P0 * e^(-rt))
Numerial method for dP/dt = rP (1-P/K)
Numerical solution compare with exact solution at K/2
for Numerical solution since exact K/3 is not possible use close.
Check accuracy of your numerical solution by reducing step size against exact solution
Choose N:
set h = 20/N. 
for n=0, . . .N:
y=yn + h* f(tn, yn)
y-P(tn)
(y-K/2)<  tol (tolerance; constant term.) print(tn) break
yn =y
told =0 
yold = y0
for n = 1, . . .N:

y = yold + h *f(yold, told)
y-P(told+h) < check
check |y -K/2| < C2 h
print(told+h)
yold = y
told = told + h
"""


def my_abs(x):
    """Compute the absolute value of x."""
    if x < 0:
        return -x
    else:
        return x

def my_factorial(n):
    """Compute the factorial of a non-negative integer n."""
    if n < 0:
        raise ValueError("Factorial is not defined for negative numbers.")
    
    # Default result if only 0! or 1!
    result = 1

    #Compute factorial iteratively if n > 1
    for i in range(2, n + 1):
        result *= i

    return result


def my_exponential(x, max_iterations=1000,tolerance=1e-10):

    """ Compute the exponential of x using Taylor series expansion. """
    ''' e^x = 1 + x/1! + x^2/2! + x^3/3! + ... + x^n/n! + xi^(n+1)/(n+1)! '''
    ''' e^x = 1 + (1)*x/(1) + (1 * x)/(1) * (x)/(2) + (1 * x)/(1) * (x)/(2) * (x)/(3) + ... '''
   

    #first term of the exponential series
    term = 1.0

    #Initialize result with the first term
    result = term

    #Initialize order of the term
    n = 1
    
    #Checks if the absolute value of the term is small enough to stop
    #As loops proceeds and terms get smaller, if the computed terms is 
    #less than the tolerance, we stop the loop
    for iteration in range(max_iterations):
        if my_abs(term) <= tolerance:
            break

        # Compute the next term in the series
        term *= x / n  

        # Add the term to the result
        result += term

        # Increment n for the next term
        n += 1
        if iteration == max_iterations - 1:
            print(f"Warning: Maximum iterations {max_iterations} for exponential reached without convergence.")
   
    return result


def my_logarithm(x, tolerance=1e-12, max_iterations=1000):

    """ compute the natural logarithm of x using Newton's iteration method. """
    ''' ln(x) = y  such that e^y = x '''

    if x <= 0:
        raise ValueError("Logarithm is not defined for zero or negative numbers.")
    
    # Initial guess for ln(x)
    y = x - 1.0

    for iteration in range(max_iterations):
        # Compute e^y using the my_exponential function
        exp_y = my_exponential(y)

        # Newton's iteration formula
        y_new = y - (exp_y - x) / exp_y

        # Check for convergence
        if my_abs(y_new - y) < tolerance:
            break

        y = y_new
        if iteration == max_iterations - 1:
            print(f"Warning: Maximum iterations {max_iterations} for logarithm reached without convergence.")
    result = y
    return result


def my_sqrt(x, initial_guess=1.0, max_iterations=100, tolerance=1e-12):
    
    """ compute the square root of x using Newton's iteration method. """
    ''' sqrt(x) = y  such that y^2 = x '''
    iteration = 0
    if x < 0:
        raise ValueError("Square root is not defined for negative numbers.")
    
    if x==0:
        #Returing 0 for sqrt(0) to prevent starting loop
        print(f"Converged (tolerance: {tolerance}) in {iteration} iterations.")
        s0=0.0
        return s0
    elif x==1:
        #Returning 1 for sqrt(1) to prevent starting loop
        print(f"Converged (tolerance: {tolerance}) in {iteration} iterations.")
        s0=1.0
        return s0
    else:
        # Allowing only positive square roots so eliminating negative initial guesses
        s0 = initial_guess if initial_guess > 0 else my_abs(x) / 2.0

   
    for iteration in range(max_iterations):
        # Newton's iteration formula

        # Checking if s is zero to avoid division by zero
        s_new = s0 - (s0**2 - x) / (2 * s0)

        # Check for convergence
        if my_abs(s_new - s0)/my_abs(s0) < tolerance:
            print(f"Converged (tolerance: {tolerance}) in {iteration} iterations.")
            break

        s0 = s_new
        if iteration == max_iterations - 1:
            print(f"Warning: Maximum iterations {max_iterations} for square root reached without convergence.")
    result = s_new
    return result


"""=======================================================================================
                                 Test the functions
======================================================================================="""

if __name__ == "__main__":
    # Test my_sqrt function
    test_values = [0, 1, 4, 9, 9E58, 0.25]
    for val in test_values:
        sqrt_val = my_sqrt(val, initial_guess=-1)
        print(f"my_sqrt({val}) = {sqrt_val}, check: {sqrt_val**2}\n")
    
    print("\n************************************************************************************")

    #Test my_exponential function
    test_values_exp = [0, 1, -1, 10, -10, 20]
    for val in test_values_exp:
        exp_val = my_exponential(val)
        print(f"my_exponential({val}) = {exp_val}, check: {exp_val/my_exponential(0)}\n")
    
    #Test my_logarithm function
    print("\n************************************************************************************")
    test_values_log = [1, 2.718281828459045, 7.38905609893065, 20, 0.5]
    for val in test_values_log:
        log_val = my_logarithm(val)
        print(f"my_logarithm({val}) = {log_val}, check: {my_exponential(log_val)}\n")
