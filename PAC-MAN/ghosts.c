#define GHOSTS_STUD
#ifdef GHOSTS_STUD


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ghosts.h"
#include "pacman.h"
#include "matrix.h"


/* Create the ghosts data structure */
struct ghosts* ghosts_setup(unsigned int num_ghosts) {
    struct ghosts* g = NULL;
    return g;
}

/* Destroy the ghost data structure */
void ghosts_destroy(struct ghosts* G) {
    return;

}

/* Set the arena (A) matrix */
void ghosts_set_arena(struct ghosts* G, char** A, unsigned int nrow,
    unsigned int ncol) {
    return;
}

/* Set the position of the ghost id. */
void ghosts_set_position(struct ghosts* G, unsigned int id, struct position pos) {
    return;
}

/* Set the status of the ghost id. */
void ghosts_set_status(struct ghosts* G, unsigned int id, enum ghost_status status) {
    return;
}

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts* G) {
    return 0;
}

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    struct position p;
    return p;

}

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    return 0;
}

/* Move the ghost id (according to its status). Returns the new position */
struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    struct position p;
    return p;
}

#endif


