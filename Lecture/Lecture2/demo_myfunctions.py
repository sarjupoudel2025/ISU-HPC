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


def my_exponential(x,):

    """ Compute the exponential of x using Taylor series expansion. """
    ''' e^x = 1 + x/1! + x^2/2! + x^3/3! + ... + x^n/n! + xi^(n+1)/(n+1)! '''
    ''' e^x = 1 + (1)*x/(1) + (1 * x)/(1) * (x)/(2) + (1 * x)/(1) * (x)/(2) * (x)/(3) + ... '''
    tolerance = 1e-10

    #first term of the exponential series
    term = 1.0

    #Initialize result with the first term
    result = term

    #Initialize order of the term
    n = 1
    
    #Checks if the absolute value of the term is small enough to stop
    #As loops proceeds and terms get smaller, if the computed terms is 
    #less than the tolerance, we stop the loop
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


def my_sqrt(x):
    
    """ compute the square root of x using Newton's iteration method. """
    ''' sqrt(x) = y  such that y^2 = x '''

    if x < 0:
        raise ValueError("Square root is not defined for negative numbers.")
    
    tolerance = 1e-12

    # Initial guess for sqrt(x)
    s = x / 2.0 if x != 0 else 0.0

    while True:
        # Newton's iteration formula
        s_new = 0.5 * (s + x / s) if s != 0 else 0.0

        # Check for convergence
        if abs(s_new - s) < tolerance:
            break

        s = s_new
    
    result = s
    return result

def my_abs(x):
    """Compute the absolute value of x."""
    if x < 0:
        return -x
    else:
        return x

"""
=======================================================================================
                                Test the functions
=======================================================================================
"""

if __name__ == "__main__":
    


    print("\n***************************************************************\nSample Test Cases for Custom Math Functions\n")

    # Test the my_factorial function with a sample input
    test_value = 5
    print(f"Calculating factorial for: {test_value}")
    factorial_result = my_factorial(test_value)
    print(f"Factorial of {test_value} = {factorial_result}")
   
    # Test the my_exponential function with a sample input
    test_value_exp = 2.0
    print(f"Calculating exponential for: {test_value_exp}")
    exponential_result = my_exponential(test_value_exp)
    print(f"Exponential of {test_value_exp} = {exponential_result}")

    # Test the my_logarithm function with a sample input
    test_value_log = exponential_result#2.718281828459045
    print(f"Calculating logarithm for: {test_value_log}")
    logarithm_result = my_logarithm(test_value_log)
    print(f"Natural logarithm of {test_value_log} = {logarithm_result}")

    # Test the my_sqrt function with a sample input
    test_value_sqrt = 16.0
    print(f"Calculating square root for: {test_value_sqrt}")
    sqrt_result = my_sqrt(test_value_sqrt)
    print(f"Square root of {test_value_sqrt} = {sqrt_result}")

    print("\n***************************************************************\n\n")

    customTest=input("Want to continue with custom tests? (y/n):")
    while customTest.lower()=='y':
        print("\nSelect the function to test:")
        print("1 for Factorial")
        print("2 for Exponential")
        print("3 for Logarithm")
        print("4 for Square Root")
        function_choice=input("Enter your choice (1/2/3/4):")
        if function_choice=='1':
            x=int(input("\nEnter a non-negative integer for factorial calculation:"))
            try:
                print(f"Factorial of {x} = {my_factorial(x)}\n")
            except ValueError as e:
                print(e)
        elif function_choice=='2':
            x=float(input("\nEnter a number for exponential calculation:"))
            print(f"Exponential of {x} = {my_exponential(x)}\n")
        elif function_choice=='3':
            x=float(input("\nEnter a positive number for logarithm calculation:"))
            try:
                print(f"Natural logarithm of {x} = {my_logarithm(x)}\n")
            except ValueError as e:
                print(e)
        elif function_choice=='4':
            x=float(input("\nEnter a non-negative number for square root calculation:"))
            try:
                print(f"Square root of {x} = {my_sqrt(x)}\n")
            except ValueError as e:
                print(e)
        else:
            print("Invalid choice. Please select a valid option.\n")
        customTest=input("\nWant to continue with custom tests? (y/n):")


