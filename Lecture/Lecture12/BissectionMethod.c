#include "BissectionMethod.h"
double function(double x) {
    // Example function: f(x) = x^3 - x - 2
    return pow(x, 3) - x - 2;
}

void push(Node** head, double a, double b) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->a = a;
    newNode->b = b;
    newNode->mid = (a + b) / 2.0;  // Midpoint of the interval
    newNode->next = *head;
    *head = newNode;
}

void pop(Node** head) {
    if (*head == NULL) return;
    Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}

void printStack(Node* head) {
    printf("Stack contents:\n");
    while (head != NULL) {
    
        printf("a: %lf, b: %lf, mid: %lf\n", head->a, head->b, head->mid);
        head = head->next;
    }
}

void freeStack(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

void bisectionMethod(double a, double b, double tol, int maxIter)
{
    if (function(a) * function(b) >= 0) {
        printf("The function has same signs at a and b.\n");
        return;
    }

    Node* stack = NULL;

    // push first interval
    push(&stack, a, b);

    for (int i = 0; i < maxIter; i++) {

        Node* current = stack;   
        double mid = current->mid;

        if (fabs(function(mid)) < tol) {
            printf("Root found in %d iterations: %.10lf\n", i, mid);
            break;
        }


        if (function(current->a) * function(mid) < 0)
            //Root is in the left subinterval [a, mid]
            push(&stack, current->a, mid);
        else
            //Root is in the right subinterval [mid, b]
            push(&stack, mid, current->b);
    }

    //Print the elements in the Stack after the convergence
    printStack(stack);

    freeStack(stack);
}