#ifndef LINKEDLISTESSENTIALS_H
#define LINKEDLISTESSENTIALS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int coefficient;
    int power;
    struct Node* next;
} Node;

Node* createNode(int coefficient, int power);
void insertNode(Node** head, int coefficient, int power);
void printPolynomial(Node* head);
Node* addPolynomials(Node* poly1, Node* poly2);
double evaluatePolynomial(Node* head, double x);
void freePolynomial(Node** head);

#endif