#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct queue_node queue_node;
typedef struct queue queue;

/**
 * function to create an empty queue
 * @return pointer to a queue
 */
queue* createQueue();

/**
 * function to destroy a queue
 * @param  q pointer to a queue
 * @return   true for success / false for failure
 */
bool destroyQueue(queue* q);

/**
 * function to add a key k to queue q
 * @param q pointer to a queue
 * @param k an integer key
 */
bool addToBack(queue* q, int k);

/**
 * function to free a key from given queue q
 * @param   q pointer to a queue
 */
bool removeFromFront(queue *q);

/**
 * function to get the current front of the queue
 * @param  q queue to access
 */
int getFront(queue* q);

/**
 * function to get the current rear of the queue
 * @param  q queue to access
 */
int getRear(queue* q);

/**
 * function to check if the queue is empty
 * @param  q the queue to check
 * @return   true if empty, false if non-empty
 */
bool isQueueEmpty(queue* q);

/**
 * utility function to print a queue node.
 * @param  node node to print
 */
void nodePrint(queue_node* node);


#endif
