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

double evaluatePolynomial(Node* head, double x) {
    double result = 0.0;
    Node* current = head;
    while (current != NULL) {
        result += current->coefficient * pow(x, current->power);
        current = current->next;
    }
    return result;
}

void printPolynomial(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%dx^%d ", current->coefficient, current->power);
        if (current->next != NULL && current->next->coefficient >= 0) {
            printf("+ ");
        }
        current = current->next;
    }
    printf("\n");
}

void freePolynomial(Node** head) {
    Node* current = *head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
void insertNode(Node** head, int coefficient, int power) {
    Node* newNode = createNode(coefficient, power);
    if (*head == NULL || (*head)->power < power) {
        newNode->next = *head;
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL && current->next->power > power) {
            current = current->next;
        }
        if (current->power == power) {
            current->coefficient += coefficient;
            free(newNode);
        } else {
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

Node* createNode(int coefficient, int power) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->coefficient = coefficient;
    newNode->power = power;
    newNode->next = NULL;
    return newNode;
}

Node* addPolynomials(Node* poly1, Node* poly2) {
    Node* result = NULL;
    Node* current1 = poly1;
    Node* current2 = poly2;

    while (current1 != NULL && current2 != NULL) {
        if (current1->power > current2->power) {
            insertNode(&result, current1->coefficient, current1->power);
            current1 = current1->next;
        } else if (current1->power < current2->power) {
            insertNode(&result, current2->coefficient, current2->power);
            current2 = current2->next;
        } else {
            int sumCoefficient = current1->coefficient + current2->coefficient;
            if (sumCoefficient != 0) {
                insertNode(&result, sumCoefficient, current1->power);
            }
            current1 = current1->next;
            current2 = current2->next;
        }
    }

    while (current1 != NULL) {
        insertNode(&result, current1->coefficient, current1->power);
        current1 = current1->next;
    }

    while (current2 != NULL) {
        insertNode(&result, current2->coefficient, current2->power);
        current2 = current2->next;
    }

    return result;
}

