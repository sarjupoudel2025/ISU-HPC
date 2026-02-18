'''
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
'''
def plot_chebyshev():
    import matplotlib.pyplot as plt
    import numpy as np

    # Read data from the file
    data = np.loadtxt('chebyshev_output.txt')
    x = data[:, 0]
    PN_x = data[:, 1]

    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(x, PN_x, label='PN(x)', color='blue')
    plt.title('Chebyshev Polynomial PN(x)')
    plt.xlabel('x')
    plt.ylabel('PN(x)')
    plt.grid()
    plt.legend()
    plt.savefig('chebyshev_plot.png')  # Save the figure
    plt.show()

if __name__ == "__main__":
    plot_chebyshev()    