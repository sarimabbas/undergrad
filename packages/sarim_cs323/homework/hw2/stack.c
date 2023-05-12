#include <stack.h>

stackNode* stackNewNode(int data) {
    stackNode* stackNode = malloc(sizeof(stackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

int stackIsEmpty(stackNode* root) { return !root; }

void stackPush(stackNode** root, int data) {
    stackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
    printf("%d pushed to stack\n", data);
}

int stackPop(stackNode** root) {
    if (isEmpty(*root)) return INT_MIN;
    stackNode* temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    free(temp);

    return popped;
}

int stackPeek(stackNode* root) {
    if (isEmpty(root)) return INT_MIN;
    return root->data;
}
