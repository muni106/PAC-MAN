#define GHOSTS_STUD
#ifdef GHOSTS_STUD


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ghosts.h"
#include "pacman.h"
#include "matrix.h"

static const struct position UNK_POSITION = { -1,-1 };

//Struttura fantasma singolo
struct ghost {
    unsigned int ID;
    int status;
    int direction;
    struct position pos;
    
};
//Struttura per tutti i fantasmi
struct ghosts {
    unsigned int amount;
    struct ghost* ghost_array;
    char** ARENA;
    unsigned int nrows;
    unsigned int ncols;
    
};



/* Create the ghosts data structure */
struct ghosts* ghosts_setup(unsigned int num_ghosts) {
    struct ghosts* g = (struct ghosts *)calloc(1,sizeof(struct ghosts));
    if (g != NULL) {
        g->amount = num_ghosts;
        g->ghost_array = (struct ghost*)calloc(num_ghosts, sizeof(struct ghost));
        unsigned int i = 0;
        if (g->ghost_array != NULL) {
            while (i < (g->amount)) {
                g->ghost_array[i].ID = i;
                g->ghost_array[i].pos = UNK_POSITION;
                g->ghost_array[i].direction = UP;
                i++;
            }
        }
    }    
    return g;
}

/* Destroy the ghost data structure */
void ghosts_destroy(struct ghosts* G) {
    if (G != NULL) {
    free(G->ghost_array);
    free(G);
    
    }

}

/* Set the arena (A) matrix */
void ghosts_set_arena(struct ghosts* G, char** A, unsigned int nrow,
    unsigned int ncol) {
    if (G != NULL) {
        G->ARENA = A;
        G->nrows = nrow;
        G->ncols = ncol;
    }
}

/* Set the position of the ghost id. */
void ghosts_set_position(struct ghosts* G, unsigned int id, struct position pos) {
    if (G != NULL) G->ghost_array[id].pos = pos;
}

/* Set the status of the ghost id. */
void ghosts_set_status(struct ghosts* G, unsigned int id, enum ghost_status status) {
    if (G != NULL) G->ghost_array[id].status = status;
}

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts* G) {
    return G->amount;
}

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    return G !=NULL ? G->ghost_array[id].pos : UNK_POSITION;

}

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    return G!= NULL ? G->ghost_array[id].status : UNK_GHOST_STATUS;
}



/* Move the ghost id (according to its status). Returns the new position */
//DA MODIFICARE
struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    if (update_ghost_posiotion(G, G->ghost_array->direction) != 0) {
        if (G->ghost_array->direction != /*something*/)
            update_ghost_posiotion(G->ghost_array, G->ghost_array->direction);
    }
    return G->ghost_array->pos;
}


//dovrebbe essere buono
static struct position new_pos(struct position pos, enum direction dir, unsigned int nrow, unsigned int ncol) {
        struct position nuova_pos = pos;
        switch (dir) {
        case LEFT: nuova_pos.j = (pos.j + (ncol - 1)) % ncol; 
            break;
        case RIGHT: nuova_pos.j = (pos.j + 1) % ncol; 
            break;
        case UP:    nuova_pos.i = (pos.i + (nrow - 1)) % nrow;
            break;
        case DOWN:  nuova_pos.i = (pos.i + 1) % nrow; 
            break;
        case UNK_DIRECTION: break;
        }
        return nuova_pos;

}

//DA MODIFICARE MOLTO 
static int update_ghost_posiotion(struct ghosts *G, enum direction dir) {
    struct position nuova_pos = new_pos(G->ghost_array[0].pos, dir, G->nrows, G->ncols);
    if (!IS_WALL(G->ARENA, nuova_pos) && ) {
        G->ghost_array[0].pos = nuova_pos;
        G->ghost_array->direction = dir;
        return 0;
    }
    else {
        return 1;
    }
}

#endif


