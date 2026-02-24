/*Develop a program (with multiple files) for linked List with application for 
polynomial representation, addition and evaluation. Use Makefile.
Update Git Repository
Submit source codes and screenshots.*/

/*Stores terms with non zero coefficients
Order the nodes by descending power
Add two polynomials
Two nodes of same power may cancel out (zero coefficients) then no nodes for this.
Evaluate polynomials at some x points

function: inserts a node to a list
function: add two polynomials and return a new polynomial
function: evaluate a polynomial at a given x value.
function: free memory deleting all polynomials*/
#include "LinkedListEss.h"
int main() {
    Node* poly1 = NULL;
    Node* poly2 = NULL;

    // Create first polynomial: 3x^2 + 2x + 1
    insertNode(&poly1, 3, 5);
    insertNode(&poly1, 2, 2);
    insertNode(&poly1, 1, 0);

    // Create second polynomial: 5x^3 + 4x^2 + 3
    insertNode(&poly2, 5, 4);
    insertNode(&poly2, 4, 2);
    insertNode(&poly2, 3, 0);

    printf("Polynomial 1: ");
    printPolynomial(poly1);

    printf("Polynomial 2: ");
    printPolynomial(poly2);

    Node* sum = addPolynomials(poly1, poly2);
    printf("Sum of Polynomials: ");
    printPolynomial(sum);

    double x = 2;
    printf("Evaluation of Polynomial 1 at x=%f: %f\n", x, evaluatePolynomial(poly1, x));
    printf("Evaluation of Polynomial 2 at x=%f: %f\n", x, evaluatePolynomial(poly2, x));
    printf("Evaluation of Sum at x=%f: %f\n", x, evaluatePolynomial(sum, x));

    freePolynomial(&poly1);
    freePolynomial(&poly2);
    freePolynomial(&sum);

    return 0;
}
