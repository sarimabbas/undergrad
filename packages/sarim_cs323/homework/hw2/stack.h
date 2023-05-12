
// C program for linked list implementation of stack
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// A structure to represent a stack
typedef struct stackNode {
    int data;
    struct stackNode* next;
} stackNode;

stackNode* stackNewNode(int data);

int stackIsEmpty(stackNode* root);

void stackPush(stackNode** root, int data);

int stackPop(stackNode** root);

int stackPeek(stackNode* root);
