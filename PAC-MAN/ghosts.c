#define GHOSTS_STUD
#ifdef GHOSTS_STUD


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ghosts.h"
#include "pacman.h"
#include "matrix.h"

/* VARIABILI GLOBALI */

static const struct position UNK_POSITION = {0,0};

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
    struct ghosts* g = (struct ghosts*)malloc(sizeof(struct ghosts));
    if (g != NULL) {
        g->amount = num_ghosts;
        g->ghost_array = (struct ghost*)calloc(num_ghosts, sizeof(struct ghost));
        unsigned int i = 0;
        if (g->ghost_array != NULL) {
            while (i < (g->amount)) {
                g->ghost_array[i].ID = i;
                g->ghost_array[i].pos = UNK_POSITION;
                g->ghost_array[i].direction = UNK_DIRECTION;
                g->ghost_array[i].status = NORMAL;
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
    if (G != NULL) return G->amount;
    return 0;
}

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    return G != NULL ? G->ghost_array[id].pos : UNK_POSITION;

}

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    return G != NULL ? G->ghost_array[id].status : UNK_GHOST_STATUS;
}








#define PPOS P->pos



//cambia direzione in modo casualeù
int new_direction(int dir) {
    while (dir < 4) {
        dir++;
        return dir;
    }
    return 0;
}

////DA MODIFICARE MOLTO 
//static int update_ghost_position(struct ghosts* G, struct pacman* P, unsigned int id) {
//    struct position nuova_pos = new_pos(G->ghost_array[id].pos, G->ghost_array->direction, G->nrows, G->ncols);
//    struct position pacman_pos = pacman_get_position(P);
//    int g_status = ghosts_get_status(G, id);
//
//    struct pacman* pac = P;
//    //NORMAL
//    if (g_status == NORMAL) {
//        if (!IS_WALL(G->ARENA, nuova_pos) && !IS_GHOST(G->ARENA, nuova_pos)) {
//            G->ghost_array[id].pos = nuova_pos;
//            G->ghost_array->direction = new_direction(G->ghost_array[id].direction);
//            return 0;
//        }
//        else return 1;
//
//    }
//
//    //SCARED
//    if (g_status == SCARED_BLINKING || g_status == SCARED_NORMAL) {
//        if (!IS_WALL(G->ARENA, nuova_pos) && !IS_GHOST(G->ARENA, nuova_pos)
//            && (G->ghost_array[id].pos.j == pacman_pos.j && G->ghost_array[id].pos.i == pacman_pos.i)) {
//            G->ghost_array[id].pos = nuova_pos;
//            G->ghost_array->direction = new_direction(G->ghost_array[id].direction);
//            return 0;
//        }
//        else return 1;
//
//    }
//
//    //EYES
//    if (g_status == EYES) {
//        G->ghost_array[id].pos = nuova_pos;
//        G->ghost_array[id].direction = HOME_SYM;
//    }
//}




/* Move the ghost id (according to its status). Returns the new position */
//DA MODIFICARE
struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    struct position pacman_pos = pacman_get_position(P);
    struct position ghost_pos = G->ghost_array[id].pos;
    int dir = G->ghost_array[id].direction;
    if (dir == UNK_DIRECTION) dir = UP;
    int new_direction;

    switch (G->ghost_array[id].status) {
    case NORMAL:
        new_direction = 
    }
}




//inverti direzione 
int inversione(int dir) {
    switch (dir) {
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
        default: return -1; 
        break;
    }
}

//condizioni fantasma normale
int normal_ghost(struct position pos, struct ghosts* G) {
    int i;
    //vede se è un fantasma
    for (i = 0; i < G->amount; i++) {
        if(G->ghost_array[i].pos.i == pos.i && G->ghost_array[i].pos.j == pos.j)
        return 1;
    }
    return (IS_WALL(G->ARENA, pos));
}

//condizioni fantasma spaventato
int scared_ghost(struct position pos, struct ghosts* G, struct position pacman_pos) {
    if ((pacman_pos.i == pos.i && pacman_pos.j == pos.j) || normal_ghost(pos, G)) return 1;
    else return 0;
    
}


//Nuova posizione del fantasma
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

//Ritorna 1 se i fantasmi sono nella stessa posizione
int stessa_posizione(struct position pos_1, struct position pos_2) {
    return (pos_1.j == pos_2.j && pos_1.i == pos_2.i);
}

//Fa tornare il fantasmino mangiato a casa
int ritornoFantasma(struct position ghost_pos,struct ghosts* G) {
    char casella = G->ARENA[ghost_pos.i][ghost_pos.j];
    switch (casella) {
        case LEFT_SYM: return LEFT;
        case UP_SYM: return UP;
        case RIGHT_SYM: return RIGHT;
        case DOWN_SYM: return DOWN;
        default: return UNK_DIRECTION;
    }
}

#endif