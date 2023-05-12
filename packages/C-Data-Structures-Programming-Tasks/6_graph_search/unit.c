#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "ldigraph.h"
#include "queue.h"

int main(int argc, char **argv)
{
    printf("HELLOO WORLD\n");

    printf("Testing Queue\n");
    queue* q = createQueue();
    addToBack(q, 13);
    addToBack(q, 23);
    addToBack(q, 33);
    addToBack(q, 43);
    addToBack(q, 53);
    int front = getFront(q);
    printf("START FRONT NODE: %i\n", front);

    while(!isQueueEmpty(q))
    {
        front = getFront(q);
        printf("FRONT NODE: %i\n", front);
        printf("REMOVE SUCCESS?: %i\n", removeFromFront(q));
    }

    if(isQueueEmpty(q))
    {
        printf("queue is now empty\n");
    }

    destroyQueue(q);
}
