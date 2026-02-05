def GaussElimination(A,b):
    import numpy as np
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

def LeastSquareApprox(x,f,n):
    import numpy as np
    m = len(x)
    mat_X = np.zeros((m, n+1))

    #Formulate matrix X and vector b
    for i in range(m):
        for j in range(n+1):
            mat_X[i][j] = x[i]**j
    
    #Formulate Matrix A with matrix X . A=X^T . X
    A = np.dot(mat_X.T, mat_X)
    #Formulate vector b with X^T . f(x)
    b = np.dot(mat_X.T, f(x))
    coeffs = GaussElimination(A.copy(), b.copy())
    return coeffs

def PolynomialEval(coeffs, x):
    import numpy as np
    n = len(coeffs)
    result = 0
    for i in range(n):
        result += coeffs[i] * (x ** i)
    return result

#Define the function to interpolate
def f(x):
    import numpy as np
    return np.cos(x)

#List of x points
#linspcae (-pi, pi ,51)
if __name__ == "__main__":
    import matplotlib.pyplot as plt
    import numpy as np
    x_points = np.linspace(-np.pi, np.pi, 51)

    #Degree of polynomial
    degree = 5

    #Get coefficients using Least Square Approximation
    coeffs = LeastSquareApprox(x_points, f, degree)
    print("Coefficients of the Least Squares Approximating Polynomial:")
    for i, coeff in enumerate(coeffs):
        print(f"a_{i} = {coeff}")
    
    #Evaluate polynomial at some point
    test_x = np.linspace(-np.pi, np.pi, 30)
    poly_values = PolynomialEval(coeffs, test_x)
    func_values = f(test_x)

    #Plot the Interpolating Polynomial and the function
    plt.figure(figsize=(8, 5))
    plt.plot(test_x, poly_values, 'o', label="Least Squares Polynomial(Pn) with degree "+str(degree))
    plt.plot(test_x, func_values, label="Original Function")
    plt.legend()

    #Plot error between polynomial and function
    plt.figure(figsize=(8, 5))
    error = np.abs(func_values - poly_values)
    plt.plot(test_x, error, label="Error |f(x)-Pn(x)| with degree "+str(degree))
    plt.legend()
    plt.xlabel("x")
    plt.ylabel("Error")
   


    #Other test
    other_test = True
    if other_test:
        x_test = np.linspace(-np.pi,np.pi, 100)
        degree_test = 10
        coeffs_test = LeastSquareApprox(x_test, f, degree_test)
        x4Pn = np.linspace(-np.pi, np.pi, 20)
        
        #Evaluate polynomial and function
        poly_values_test = PolynomialEval(coeffs_test, x4Pn)
        func_values_test = f(x_test)

        #Plot the Interpolating Polynomial and the function
        plt.figure(figsize=(8, 5))
        plt.plot(x4Pn, poly_values_test, 'o', label="Least Squares Polynomial(Pn) with degree "+str(degree_test))
        plt.plot(x_test, func_values_test, label="Original Function")
        plt.legend()

        #Plot error between polynomial and function
        plt.figure(figsize=(8, 5))
        error_test = np.abs(f(x4Pn) - poly_values_test)
        plt.plot(x4Pn, error_test, label="Error |f(x)-Pn(x)| with degree "+str(degree_test))
        plt.legend()
        plt.xlabel("x")
        plt.ylabel("Error")
    plt.show()