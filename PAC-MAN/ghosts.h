#ifndef GHOSTS_H 
#define GHOSTS_H

#include "global.h"
#include "pacman.h"

struct ghosts; /* Opaque declation of the ghosts data structure */

/* Create the ghosts data structure */
struct ghosts *ghosts_setup(unsigned int num_ghosts);

/* Destroy the ghost data structure */
void ghosts_destroy(struct ghosts *G);

/* Set the arena (A) matrix */
void ghosts_set_arena(struct ghosts *G, char **A, unsigned int nrow, 
                                                      unsigned int ncol);

/* Set the position of the ghost id. */
void ghosts_set_position(struct ghosts *G, unsigned int id, struct position pos);

/* Set the status of the ghost id. */
void ghosts_set_status(struct ghosts *G, unsigned int id, enum ghost_status status);

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts *G);

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts *G, unsigned int id);

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts *G, unsigned int id);

/* Move the ghost id (according to its status). Returns the new position */
struct position ghosts_move(struct ghosts *G, struct pacman *P, unsigned int id);

#endif
