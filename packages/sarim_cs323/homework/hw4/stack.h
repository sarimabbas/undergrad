#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int STACK_DATA;

typedef struct node {
    STACK_DATA data;
    struct node* next;
} node;

typedef struct stack {
    node* head;
    int size;
} stack;

stack* sCreate(void);
node* sPop(stack* s);
void sPush(stack* s, STACK_DATA d);
void sDump(stack* s);
void sDumpAsChar(stack* s);
void sDestroy(stack* s);
