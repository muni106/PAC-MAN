#include <stdlib.h>
#include "pacman.h"
#include "ghosts.h"

static const struct position UNK_POSITION = {-1,-1}; // Set unknown position to (UINT_MAX,UINT_MAX)

struct pacman {
	int status;
	int dir;
	struct position pos;
	char **A;
	unsigned int nrow;
	unsigned int ncol;
};


/* Compute the new position toward direction dir in a toroidal world of size nrow*ncol */
static struct position new_position(struct position pos, enum direction dir, unsigned int nrow, unsigned int ncol);

/* Move pacman towards direction dir if the new position is not a wall.
   Returns 0 if the update is successful, 1 otherwise.
 */ 
static int update_pacman_position(struct pacman *P, enum direction dir);


struct pacman *pacman_setup(void) {
	struct pacman *pac = (struct pacman *)calloc(1,sizeof(struct pacman));
	pac->status = UNK_PACMAN_STATUS;
	pac->dir    = UNK_DIRECTION;
	pac->pos    = UNK_POSITION;
	pac->A      = NULL;
	pac->nrow   = 0;
	pac->ncol   = 0;
	return pac;
}

void pacman_destroy(struct pacman *P) {
	free(P);
}

void pacman_set_arena(struct pacman *P, char **A, unsigned int nrow, unsigned int ncol) {
	if(P != NULL) {
		P->A = A;
		P->nrow = nrow;
		P->ncol = ncol;
	}
}

void pacman_set_position(struct pacman *P, struct position pos) {
	if(P != NULL) P->pos = pos;
}

void pacman_set_direction(struct pacman *P, enum direction dir) {
	if(P != NULL) P->dir = dir;
}

void pacman_set_status(struct pacman *P, enum pacman_status status) {
	if(P != NULL) P->status = status;
}

struct position pacman_get_position(struct pacman *P) {
	return P != NULL ? P->pos : UNK_POSITION;
}

enum direction pacman_get_direction(struct pacman *P) {
	return P != NULL ? P->dir : UNK_DIRECTION;
}

enum pacman_status pacman_get_status(struct pacman *P) {
	return P != NULL ? P->status : UNK_PACMAN_STATUS;
}

struct position pacman_move(struct pacman *P, enum direction dir) {
	if(update_pacman_position(P,dir) != 0)
		if(P->dir != dir) // try to move in the current direction
		update_pacman_position(P,P->dir);
			
	return P->pos;
}

static struct position new_position(struct position pos, enum direction dir, unsigned int nrow, unsigned int ncol) {
	struct position new = pos;
	switch (dir) {
		case LEFT:  new.j = (pos.j+(ncol-1)) % ncol; break;
		case RIGHT: new.j = (pos.j+1)        % ncol; break;
		case UP:    new.i = (pos.i+(nrow-1)) % nrow; break;
		case DOWN:  new.i = (pos.i+1)        % nrow; break;
		case UNK_DIRECTION: break;
	}
	return new;
}

static int update_pacman_position(struct pacman *P, enum direction dir) {
	struct position new = new_position(P->pos,dir,P->nrow,P->ncol);
	if(!IS_WALL(P->A,new)) {
		P->pos = new;
		P->dir = dir;
		return 0;
	} else {
		return 1;
	}
}
