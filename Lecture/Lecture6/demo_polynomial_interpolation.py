"""
This is a demo script for polynomial interpolation for assignment Lecture 05.
It uses Gaussian elimination to solve for the coefficients of a cubic polynomial
that interpolates the function f(x) = cos(x) at four specified points.
(-0.1, cos(-0.1)), (-0.02, cos(-0.02)), (0.02, cos(0.02)), (0.1, cos(0.1)).
P(x) = ax^3 + bx^2 + cx + d
The script sets up the linear system, solves it using GaussElimination,
and measures the maximum error between f and p at the four points.
"""
#Form the equations for interpolation
"""
a.(-0.1)^3 + b.(-0.1)^2 + c.(-0.1) + d = cos(-0.1)
a.(-0.02)^3 + b.(-0.02)^2 + c.(-0.02) + d = cos(-0.02)
a.(0.02)^3 + b.(0.02)^2 + c.(0.02) + d = cos(0.02)
a.(0.1)^3 + b.(0.1)^2 + c.(0.1) + d = cos(0.1)

Hence the matrix A and vector b are formed as follows:
A = [[(-0.1)^3, (-0.1)^2, (-0.1), 1],
     [(-0.02)^3, (-0.02)^2, (-0.02), 1],
     [(0.02)^3, (0.02)^2, (0.02), 1],
     [(0.1)^3, (0.1)^2, (0.1), 1]]
b = [cos(-0.1), cos(-0.02), cos(0.02), cos(0.1)
x = [a, b, c, d]"""

import numpy as np
#Given Points for interpolation
points_x = [-0.1, -0.02, 0.02, 0.1]
#Function to interpolate
def f(x):
    return np.cos(x)

#Construct matrix A and vector b
A = np.array([[x**3, x**2, x, 1] for x in points_x])
b = np.array([f(x) for x in points_x])
#Function for Gauss Elimination
def GaussElimination(A,b):
    # No. of Equations in A = No. of rows of A
    numEqs = len(A)
    print("Number of Equations: ", numEqs)
    #Row Reduction method to convert A to upper triangular matrix
    for i in range(numEqs):
       

        #Find Maximum absolute value of element in the pivot column
        #This prevent division by small numbers and zero if pivot element is zero
        maxRow = i + np.argmax(np.abs(A[i:,i]))

        #If the pivot element is not maximum, swap the rows, both row in A and b
        if i != maxRow:
            A[[i,maxRow]] = A[[maxRow,i]]
            b[[i,maxRow]] = b[[maxRow,i]]
        #Elimination
        for j in range(i+1, numEqs):
            factor = A[j][i] / A[i][i]
            A[j] = A[j] - factor * A[i]
            b[j] = b[j] - factor * b[i]

        
    #Back Substitution to find solution vector x
    x = np.zeros(numEqs)
    for i in range(numEqs-1, -1, -1):
        x[i] = (b[i] - np.dot(A[i,i+1:], x[i+1:])) / A[i][i]
    return x

#Solve for coefficients [a, b, c, d]
coefficients = GaussElimination(A.copy(), b.copy())
a, b, c, d = coefficients
print(f"Coefficients of the interpolating polynomial: a={a}, b={b}, c={c}, d={d}\n")

#Define the interpolating polynomial
def p(x):
    return a*x**3 + b*x**2 + c*x + d

#Measure maximum error at the four points
errors = [abs(f(x) - p(x)) for x in points_x]
max_error = max(errors)
print("Errors at the points:", errors)
print(f"\nMaximum error between f and p at the four points: {max_error}")