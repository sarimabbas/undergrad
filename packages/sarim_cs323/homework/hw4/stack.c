#include "stack.h"

// TODO: stack size is upper bounded by the number of elements in tthe hashtable
// so maybe you could transition to array-based stack?

stack* sCreate(void) {
    stack* s = malloc(sizeof(stack));
    s->head = NULL;
    s->size = 0;
    return s;
}

void sDestroy(stack* s) {
    node* curr = s->head;
    while(curr != NULL) {
        node* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    free(s);
}

void sDump(stack* s) {
    node* curr = s->head;
    fprintf(stderr, "%10s", "HEAD: [");
    while(curr != NULL) {
        if (curr->next != NULL) {
            fprintf(stderr, "%i ", curr->data);
        } else {
            fprintf(stderr, "%i", curr->data);
        }
        curr = curr->next;
    }
    fprintf(stderr, "]\n");
}

void sDumpAsChar(stack* s) {
    node* curr = s->head;
    printf("HEAD: [");
    while (curr != NULL) {
        printf("%c ", curr->data);
        curr = curr->next;
    }
    printf("]\n");
}

void sPush(stack* s, STACK_DATA d) {
    node* new = malloc(sizeof(node));
    new->data = d;
    new->next = s->head;
    s->head = new;
    (s->size)++;
}

node* sPop(stack* s) {
    node* pop = s->head;
    s->head = s->head->next;
    (s->size)--;
    return pop;  
}
