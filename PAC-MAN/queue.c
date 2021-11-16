#include <stdlib.h>
#include "queue.h"

struct queue *queue_alloc(size_t size) {
	struct queue *Q = (struct queue *)malloc(sizeof(struct queue));
	
	Q->q     = (struct position *)malloc(size*sizeof(struct position));
	Q->start = 
	Q->end   = -1;
	Q->size  = size;

	return Q;
}

void queue_free(struct queue *Q) {
	if(Q != NULL) {
		free(Q->q);
		free(Q);
	}
} 

int empty_queue(struct queue *Q) {
	return Q == NULL || Q->start == -1;
}

int full_queue(struct queue *Q) {
	return !empty_queue(Q) && (Q->end + 1) % Q->size == Q->start;
}

void enqueue(struct queue *Q, struct position p) {
	if(empty_queue(Q)) 
		Q->start = Q->end = 0;

	if(!full_queue(Q)) {
		Q->q[Q->end] = p;
		Q->end = (Q->end+1) % Q->size;
	}
}

struct position dequeue(struct queue *Q) {
	struct position pos = {-1,-1};
	if(!empty_queue(Q)) {
		pos = Q->q[Q->start];
		Q->start = (Q->start + 1) % Q->size;
		if(Q->start == Q->end)
			Q->start = Q->end = -1;
	}
	return pos;	
}
