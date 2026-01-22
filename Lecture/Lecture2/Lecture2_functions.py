"This is a lab assigment for Math 5250-01 Numerical Analysis of High Performance Computing Spring 2026"
"Three functions defined here are factorial, exponential, and logarithm."


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


def my_exponential(x):

    """ Compute the exponential of x using Taylor series expansion. """
    ''' e^x = 1 + x/1! + x^2/2! + x^3/3! + ... + x^n/n! + xi^(n+1)/(n+1)! '''
    tolerance = 1e-10

    #first term of the exponential series
    term = 1.0

    #Initialize result with the first term
    result = term

    #Initialize order of the term
    n = 1
    
    while abs(term) > tolerance:

        # Compute the next term in the series
        term *= x / n  

        # Add the term to the result
        result += term

        # Increment n for the next term
        n += 1

   
    return result


def my_logarithm(x):

    """ compute the natural logarithm of x using Newton's iteration method. """
    ''' ln(x) = y  such that e^y = x '''

    if x <= 0:
        raise ValueError("Logarithm is not defined for zero or negative numbers.")
    
    tolerance = 1e-12

    # Initial guess for ln(x)
    y = x - 1.0

    while True:
        # Compute e^y using the my_exponential function
        exp_y = my_exponential(y)

        # Newton's iteration formula
        y_new = y - (exp_y - x) / exp_y

        # Check for convergence
        if abs(y_new - y) < tolerance:
            break

        y = y_new
    
    result = y
    return result