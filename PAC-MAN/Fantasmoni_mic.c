#ifndef GHOSTS_H
#define GHOSTS_H

#include "global.h"
#include "pacman.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

static const struct position UNK_POSITION;

struct ghost {
    struct position pos;
    enum direction dir;
    int id;
    enum ghost_status status;
};

struct ghosts {
    char** A;
    unsigned int nrow;
    unsigned int ncol;
    unsigned int num_ghosts;
    struct ghost* ghosts_arr;
};

/*
 * Calcola la distanza tra due posizioni.
 */
static double distance(struct position pos1, struct position pos2);

/*
 * Calcola la direzione ottimale da seguire per il fantasma.
 * closest == 0 trova la direzione più lontana da pacman,
 * closest == 1 trova la direzione più vicina.
 */
static enum direction relative_direction(struct ghosts* G, struct pacman* P, struct ghost* ghost, int closest);

/*
 * Converte la direzione in una position relativa con i e j tra -1 e 1.
 */
static struct position dir_to_relative_pos(enum direction dir);

/*
 * Converte la position relativa in direzione.
 */
static enum direction relative_pos_to_dir(struct position rel);

/*
 * Calcola la nuova posizione in base alla direzione.
 */
static struct position new_position(struct position pos, enum direction dir, unsigned int nrow, unsigned int ncol);

/*
 * Determina se la posizione è libera (pacman escluso).
 */
static int is_free(struct position pos, struct ghosts* G, struct pacman* P);

/*
 * Determina se la posizione è libera (pacman incluso).
 */
static int is_free_other(struct position pos, struct ghosts* G, struct pacman* P);

/*
 * Determina se il fantasma può muoversi lateralmente.
 */
static int can_move_side(struct ghost* ghost, struct ghosts* G, struct pacman* P);

/*
 * Trova la direzione da seguire per EYES.
 */
static enum direction eyes_suggested_direction(char c);

/* Create the ghosts data structure */
struct ghosts* ghosts_setup(unsigned int num_ghosts) {
    struct ghosts* G = (struct ghosts*)malloc(sizeof(struct ghosts));
    if (G != NULL) {
        G->num_ghosts = num_ghosts;
        G->ghosts_arr = (struct ghost*)calloc(num_ghosts, sizeof(struct ghost));

    }
    int i;
    for (i = 0; i < num_ghosts; i++) {
        struct ghost new_ghost;
        new_ghost.pos = UNK_POSITION;
        new_ghost.dir = UNK_DIRECTION;
        new_ghost.id = i;
        new_ghost.status = UNK_GHOST_STATUS;
        G->ghosts_arr[i] = new_ghost;
    }
    return G;
}

/* Destroy the ghost data structure */
void ghosts_destroy(struct ghosts* G) {
    free(G);
}

/* Set the arena ( A ) matrix */
void ghosts_set_arena(struct ghosts* G, char** A, unsigned int nrow, unsigned int ncol) {
    if (G != NULL) {
        G->A = A;
        G->nrow = nrow;
        G->ncol = ncol;
    }
}

/* Set the position of the ghost id . */
void ghosts_set_position(struct ghosts* G, unsigned int id, struct position pos) {
    if (G != NULL) G->ghosts_arr[id].pos = pos;
}

/* Set the status of the ghost id . */
void ghosts_set_status(struct ghosts* G, unsigned int id, enum ghost_status status) {
    if (G != NULL) G->ghosts_arr[id].status = status;
}

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts* G) {
    return G->num_ghosts;
}

/* Return the position of the ghost id . */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    return (G != NULL) ? G->ghosts_arr[id].pos : UNK_POSITION;
}

/* Return the status of the ghost id . */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    return (G != NULL) ? G->ghosts_arr[id].status : UNK_GHOST_STATUS;
}

struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    struct ghost* g = &G->ghosts_arr[id];
    if (g->status == EYES) {
        struct position eyes_pos = g->pos;
        char c = G->A[g->pos.i][g->pos.j];
        g->dir = eyes_suggested_direction(c);
        eyes_pos = new_position(g->pos, g->dir, G->nrow, G->ncol);
        if (!is_free_other(eyes_pos, G, P)) {
            struct position left_pos = new_position(g->pos, LEFT, G->nrow, G->ncol),
                right_pos = new_position(g->pos, RIGHT, G->nrow, G->ncol),
                up_pos = new_position(g->pos, UP, G->nrow, G->ncol),
                down_pos = new_position(g->pos, DOWN, G->nrow, G->ncol);
            if (is_free_other(left_pos, G, P)) g->dir = LEFT;
            else if (is_free_other(right_pos, G, P)) g->dir = RIGHT;
            else if (is_free_other(up_pos, G, P)) g->dir = UP;
            else if (is_free_other(down_pos, G, P)) g->dir = DOWN;
            else g->dir = UNK_DIRECTION;
        }
        g->pos = new_position(g->pos, g->dir, G->nrow, G->ncol);
    }
    else {
        struct position new = new_position(g->pos, g->dir, G->nrow, G->ncol);
        if (g->status == NORMAL) {
            if (!is_free(new, G, P) || can_move_side(g, G, P)) {
                g->dir = relative_direction(G, P, g, 1);
            }
        }
        else {
            if (!is_free_other(new, G, P) || can_move_side(g, G, P)) {
                g->dir = relative_direction(G, P, g, 0);
            }
        }
        g->pos = new_position(g->pos, g->dir, G->nrow, G->ncol);
    }

    return g->pos;
}

static struct position new_position(struct position pos, enum direction dir, unsigned int nrow, unsigned int ncol) {
    struct position new = pos;
    switch (dir) {
    case LEFT:
        new.j = (pos.j + (ncol - 1)) % ncol;
        break;
    case RIGHT:
        new.j = (pos.j + 1) % ncol;
        break;
    case UP:
        new.i = (pos.i + (nrow - 1)) % nrow;
        break;
    case DOWN:
        new.i = (pos.i + 1) % nrow;
        break;
    case UNK_DIRECTION:
        break;
    }
    return new;
}

static int is_free(struct position pos, struct ghosts* G, struct pacman* P) {
    if (pos.i >= G->nrow || pos.i < 0) return 0;
    if (pos.j < 0) {
        pos.j = 0;
        return is_free(pos, G, P);
    }
    if (pos.j >= G->ncol) {
        pos.j = G->ncol - 1;
        return is_free(pos, G, P);
    }
    if (IS_WALL(G->A, pos)) return 0;
    if (IS_GHOST(G->A, pos)) return 0;
    int i;
    for (i = 0; i < G->num_ghosts; i++) {
        if ((pos.i == G->ghosts_arr[i].pos.i) && (pos.j == G->ghosts_arr[i].pos.j))
            return 0;
    }
    return 1;
}

static int is_free_other(struct position pos, struct ghosts* G, struct pacman* P) {
    struct position pacman_pos = pacman_get_position(P);
    enum direction pacman_dir = pacman_get_direction(P);
    if (pos.i == pacman_pos.i && pos.j == pacman_pos.j)
        return 0;
    struct position new_pacman_pos = new_position(pacman_pos, pacman_dir, G->nrow, G->ncol);
    if (pos.i == new_pacman_pos.i && pos.j == new_pacman_pos.j)
        return 0;
    return is_free(pos, G, P);
}

static enum direction eyes_suggested_direction(char c) {
    switch (c) {
    case UP_SYM:
        return UP;
    case DOWN_SYM:
        return DOWN;
    case LEFT_SYM:
        return LEFT;
    case RIGHT_SYM:
        return RIGHT;
    }
    return UNK_DIRECTION;
}

static double distance(struct position pos1, struct position pos2) {
    unsigned int distance_x = (pos1.j - pos2.j) * (pos1.j - pos2.j);
    unsigned int distance_y = (pos1.i - pos2.i) * (pos1.i - pos2.i);
    return sqrt(distance_x + distance_y);
}

static struct position dir_to_relative_pos(enum direction dir) {
    struct position rel;
    rel.i = 0;
    rel.j = 0;

    switch (dir) {
    case LEFT:
        rel.j = -1;
        break;
    case RIGHT:
        rel.j = 1;
        break;
    case UP:
        rel.i = -1;
        break;
    case DOWN:
        rel.i = 1;
        break;
    case UNK_DIRECTION:
        break;
    }
    return rel;
}

static enum direction relative_pos_to_dir(struct position rel) {
    if (rel.j == -1) return LEFT;
    if (rel.j == 1) return RIGHT;
    if (rel.i == -1) return UP;
    if (rel.i == 1) return DOWN;
    return UNK_DIRECTION;
}

static enum direction relative_direction(struct ghosts* G, struct pacman* P, struct ghost* ghost, int closest) {
    struct position pacman_pos = pacman_get_position(P);
    struct position ghost_pos = ghost->pos;
    struct position ghost_dir = dir_to_relative_pos(ghost->dir);

    struct position new = ghost_pos;

    double dis = closest ? DBL_MAX : 0;

    int dir_x = 0;
    int dir_y = 0;

    /*
     * !ghost_dir.i/j permette di alternare movimenti verticali ad orizzontali
     * per evitare situazioni di stallo.
     */

    int x, y;
    for (x = -1; x <= 1; x++) {
        for (y = -1; y <= 1; y++) {
            new.i = ghost_pos.i + y;
            new.j = ghost_pos.j + x;
            if (((x && !ghost_dir.j) || (y && !ghost_dir.i)) && !(x && y) && is_free_other(new, G, P)) {
                double dist = distance(pacman_pos, new);
                if (dis > dist == closest) {
                    dis = dist;
                    dir_x = x;
                    dir_y = y;
                }
            }
        }
    }

    struct position direction;
    direction.i = dir_y;
    direction.j = dir_x;
    return relative_pos_to_dir(direction);
}

static int can_move_side(struct ghost* ghost, struct ghosts* G, struct pacman* P) {
    if (ghost->dir == LEFT || ghost->dir == RIGHT) {
        struct position up = ghost->pos;
        up.i--;
        struct position down = ghost->pos;
        down.i++;
        return is_free_other(up, G, P) || is_free_other(down, G, P);
    }
    if (ghost->dir == UP || ghost->dir == DOWN) {
        struct position left = ghost->pos;
        left.j--;
        struct position right = ghost->pos;
        right.j++;
        return is_free_other(left, G, P) || is_free_other(right, G, P);
    }
    return 0;
}

#endif