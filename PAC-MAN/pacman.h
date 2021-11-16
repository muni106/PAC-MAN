#ifndef PACMAN_H
#define PACMAN_H

#include "global.h"

struct pacman; /* Opaque declaration of the pacman datastructure */

/* Create the pacman data structure */
struct pacman *pacman_setup(void);

/* Destroy the pacman data structure */
void pacman_destroy(struct pacman *P);

/* Set the arena (A) matrix */
void pacman_set_arena(struct pacman *P, char **A, unsigned int nrow, unsigned int ncol);

/* Set pacman position. */
void pacman_set_position(struct pacman *P, struct position pos);

/* Set pacman direction */
void pacman_set_direction(struct pacman *P, enum direction dir);

/* Set pacman status */
void pacman_set_status(struct pacman *P, enum pacman_status status);

/* Return pacman position */
struct position pacman_get_position(struct pacman *P);

/* Return pacman direction */
enum direction pacman_get_direction(struct pacman *P);

/* Return pacman status */
enum pacman_status pacman_get_status(struct pacman *P);

/* Move pacman. If the move towards direction dir is not possible (i.e. a wall
   or a ghost in eyes form) tries to move pacman towards its current direction.
   Returns the new position.
 */
struct position pacman_move(struct pacman *P, enum direction dir);

#endif
