#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

struct queue_node
{
    int key;
    queue_node* next;
};

struct queue
{
    queue_node* front;
    queue_node* rear;
};

/**
 * private function to create a new linked list node.
 * @param  k key to create a node with
 * @return   pointer to the node
 */
queue_node* newNode(int k);

queue* createQueue()
{
    queue* q = malloc(sizeof(queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

bool addToBack(queue* q, int k)
{
    // Create a new queue node
    queue_node* temp = newNode(k);

    // If queue is empty, then new node is front and rear both
    if(q->rear == NULL)
    {
       q->front = temp;
       q->rear = temp;
       return true;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;

    return true;
}

bool removeFromFront(queue *q)
{
    // If queue is empty, return false.
    if(q->front == NULL)
    {
        return false;
    }

    // Store previous front and move front one node ahead
    queue_node* temp = q->front;
    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if(q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);

    // the node was removed and return true
    return true;
}

queue_node* newNode(int k)
{
    queue_node* temp = malloc(sizeof(queue_node));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

int getFront(queue* q)
{
    return q->front->key;
}

int getRear(queue* q)
{
    return q->rear->key;
}

void nodePrint(queue_node* node)
{
    if(node == NULL)
    {
        printf("NODE IS NULL\n");
    }
    else
    {
        printf("NODE KEY: %i\n", node->key);
    }
}

bool isQueueEmpty(queue* q)
{
    if(q->front == NULL && q->rear == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool destroyQueue(queue* q)
{
    queue_node* current = q->front;
    while(current != NULL)
    {
        queue_node* temp = current;
        free(temp);
        current = current->next;
    }

    free(q);
    return true;
}
