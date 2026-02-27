#ifndef BISSECTIONMETHOD_H
#define BISSECTIONMETHOD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



double function(double x);

typedef struct Node {
    double a;
    double b;
    double mid;
    struct Node* next;
} Node;

void bisectionMethod(double a, double b, double tol, int maxIter);
void push(Node** head, double a, double b);
void pop(Node** head);
void printStack(Node* head);
void freeStack(Node* head);




#endif

