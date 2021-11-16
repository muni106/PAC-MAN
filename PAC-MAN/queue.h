#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include "global.h"

struct queue {
	struct position *q;
	unsigned int start;
	unsigned int end;
	size_t size;
};


/* Alloc queue of size capacity */
struct queue *queue_alloc(size_t size);

/* Free queue */
void queue_free(struct queue *Q);

/* True if the queue is empty */
int empty_queue(struct queue *Q);

/* True if the queue is full */
int full_queue(struct queue *Q);

/* Adds position p to the queue */
void enqueue(struct queue *Q, struct position p);

/* Removes and returns the first element in the queue */
struct position dequeue(struct queue *Q);

#endif
