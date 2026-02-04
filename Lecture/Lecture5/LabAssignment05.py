"Develop a function GaussElimination(A,b) for solving the linear system"
"Ax=b"
"by Gaussian Elimination, with A and b the input and x the output."
""
"Find the cubic polynomial p(x) = ax^3 +b x^2 + cx + d that interpolates f(x) = cos(x) at the following four points"
"(-0.1,cos(-0.1)), (-0.02,cos(-0.02)), (0.02,cos(0.02)), (0.1,cos(0.1))"
"by "
"1. Set up a 4x4 linear system for computing (a,b,c,d)."
"2. Solve the linear system using the above function GasussElimination(A,b)"
"Measure the maximum error between f and p at the four points"
"Develop a script demo_polynomial_interpolation.py for the above tasks."
"Update Git repository."
"Submit both codes and results (screenshots)"
import numpy as np
def GaussElimination(A,b):
    # No. of Equations in A = No. of rows of A
    numEqs = len(A)
    print("Number of Equations: ", numEqs)
    #Row Reduction method to convert A to upper triangular matrix
    for i in range(numEqs):
        #Find Maximum absolute value of element in the pivot column
        #This prevent division by small numbers and zero if pivot element is zero
        #np.abs(A[i:,i]) gives the absolute values of the elements in the pivot column from row i to the last row
        #np.argmax(...) gives the index of the maximum value in that array
        #Add i to get the index in the original matrix A
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
            #Print intermediate steps for debugging
            #New Matrix A and vector b after each elimination step
            print(f"After eliminating row {j} using row {i}:")
            print("Matrix A:")
            print(A)
            print("Vector b:")
            print(b)

        
    #Back Substitution to find solution vector x
    x = np.zeros(numEqs)
    for i in range(numEqs-1, -1, -1):
        x[i] = (b[i] - np.dot(A[i,i+1:], x[i+1:])) / A[i][i]
    print("The solution vector x is: ", x)
    return x

#Test code for GaussElimination
#Sample Input A
A = np.array([[2.0, 1.0, 3.5 ],
              [-3.0, -1.0, 1.2]
              ,[1.0, 2.0, -4.0]])
#Sample Input b
b = np.array([8.0, -11.0, -3.0])
GaussElimination(A,b)