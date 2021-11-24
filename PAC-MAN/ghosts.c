
#define GHOSTS_STUD

#ifdef GHOSTS_STUD


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ghosts.h"
#include "pacman.h"
#include "matrix.h"

#include "global.h"


static const struct position UNK_POSITION = { -1,-1 }; // Set unknown position to (UINT_MAX,UINT_MAX)

static struct ghost
{
    unsigned int id;
    enum ghost_status status;
    enum direction direction;
    struct position ghost_position;

};

static struct ghosts {
    unsigned int amount;
    struct ghost* ghost_arr;
    unsigned int arena_rowS, arena_colS;
    char** arena_definE;
};
/*
static unsigned int arena_rows, arena_cols;
static char** arena_define;
*/

/* Create the ghosts data structure */
struct ghosts* ghosts_setup(unsigned int num_ghosts) {
    struct ghosts* ghosts_punt;
    ghosts_punt = (struct ghosts*)malloc(sizeof(struct ghosts));
    if (ghosts_punt != NULL) {
        ghosts_punt->amount = num_ghosts;
        ghosts_punt->ghost_arr = (struct ghost*)calloc(num_ghosts, sizeof(struct ghost));

        int i;

        if (ghosts_punt->ghost_arr != NULL) {
            for (i = 0; i < ghosts_punt->amount; i++) {
                ghosts_punt->ghost_arr[i].id = i;
                ghosts_punt->ghost_arr[i].ghost_position = UNK_POSITION;
                ghosts_punt->ghost_arr[i].direction = UP;
            }
        }
    }
    return ghosts_punt;
}

/* Destroy the ghost data structure */
void ghosts_destroy(struct ghosts* G) {
    if (G != NULL) {
        free(G->ghost_arr);
        free(G);
    }
    return;
}

/* Set the arena (A) matrix */
void ghosts_set_arena(struct ghosts* G, char** A, unsigned int nrow, unsigned int ncol) {
    if (G != NULL) {
        G->arena_definE = A;
        G->arena_colS = ncol;
        G->arena_rowS = nrow;
    }
    return;
}

/* Set the position of the ghost id. */
void ghosts_set_position(struct ghosts* G, unsigned int id, struct position pos) {
    if ((G != NULL) && (id < G->amount)) {
        int i;
        for (i = 0; i < G->amount; i++) {
            if (G->ghost_arr[i].id == id) {
                G->ghost_arr[i].ghost_position = pos;
            }
        }
    }
    return;
}

/* Set the status of the ghost id. */
void ghosts_set_status(struct ghosts* G, unsigned int id, enum ghost_status status) {
    if ((G != NULL) && (id < G->amount)) {
        int i;
        for (i = 0; i < G->amount; i++) {
            if (G->ghost_arr[id].id == id)
                G->ghost_arr[id].status = status;
        }
    }
    return;
}

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts* G) {
    return G->amount;
}

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    int i;
    for (i = 0; i < G->amount; i++) {
        if (G->ghost_arr[i].id == id) return G->ghost_arr[i].ghost_position;
    }
}

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    int i;
    for (i = 0; i < G->amount; i++) {
        if (G->ghost_arr[i].id == id) return G->ghost_arr[id].status;
    }
}



struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    return { 0,0 };
}

#endif
