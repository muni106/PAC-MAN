/* INCLUSIVE & DEFINES */

#define GHOSTS_STUD
#ifdef GHOSTS_STUD

#include <stdlib.h>
#include "ghosts.h"
#include "pacman.h"
#include "matrix.h"

/* GLOBAL VARIABLES */

static const struct position UNK_POSITION = { 0, 0 };
typedef struct {
    unsigned id;
    enum direction dir;
    struct position pos;
    enum ghost_status status;
} ghost;
struct ghosts {
    unsigned int num_ghost;
    ghost* arr;
};
static char** arena;
static unsigned rows, cols;

/* EXTRA FUNCTIONS PROTOTYPES */

static enum direction returnHome(struct position ghostPos);
static enum direction oppositeDirection(enum direction dir);
static int isIllegal(struct position pos, struct ghosts* G);
static int equalsPositions(struct position p1, struct position p2);
static struct position newPosition(struct position pos, enum direction dir);
static enum direction* otheAxisDirections(enum direction dir, enum direction* compDir);
static int isIllegalForScared(struct position pos, struct ghosts* G, struct position pacmanPos);
static enum direction* getDirectionToFollow(struct position g, struct position p, enum direction* r, int scared);
static enum direction chase(struct position pacmanPos, struct position ghostPos, enum direction dir, struct ghosts* G, int scared);

/* MAIN FUNCTIONS */

/* Create the ghosts data structure and assign it some defaults values */
struct ghosts* ghosts_setup(unsigned int num_ghosts) {
    struct ghosts* g = malloc(sizeof(struct ghosts));
    if (g == NULL) return NULL;
    g->arr = calloc(num_ghosts, sizeof(ghost));
    g->num_ghost = num_ghosts;
    if (g->arr == NULL) return NULL;
    unsigned int i;
    for (i = 0; i < g->num_ghost; i++) {
        struct position p = { 0, 0 };
        g->arr[i].id = i;
        g->arr[i].dir = UNK_DIRECTION;
        g->arr[i].pos = p;
        g->arr[i].status = NORMAL;
    }
    return g;
}

/* Destroy the ghost data structure freeing it*/
void ghosts_destroy(struct ghosts* G) {
    free(G->arr);
    free(G);
}

/* Set the arena (A) matrix */
void ghosts_set_arena(struct ghosts* G, char** A, unsigned int nrow, unsigned int ncol) {
    rows = nrow;
    cols = ncol;
    if (A == NULL) return;
    unsigned int i;
    for (i = 0; i < nrow; ++i)
        if (A[i] == NULL)
            return;
    arena = A;
}

/* Set the position of the ghost id. */
void ghosts_set_position(struct ghosts* G, unsigned int id, struct position pos) {
    if (G == NULL) return;
    G->arr[id].pos = pos;
}

/* Set the status of the ghost id. */
void ghosts_set_status(struct ghosts* G, unsigned int id, enum ghost_status status) {
    if (G == NULL) return;
    G->arr[id].status = status;
}

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts* G) {
    if (G != NULL) return G->num_ghost;
    return 0;
}

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    if (G == NULL) return UNK_POSITION;
    return G->arr[id].pos;
}

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    if (G == NULL) return -1;
    return G->arr[id].status;
}




/* Move the ghost id (according to its status). Returns the new position */
struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    /* set the pacman and ghosts positions in 2 variables, in another one save the direction */
    struct position ghostPos = G->arr[id].pos;
    struct position pacmanPos = pacman_get_position(P);
    enum direction dir = G->arr[id].dir;
    if (dir == UNK_DIRECTION) dir = UP;
    enum direction newDir;
    /* start checking the status of the ghost, changing direction and updating his position if legal */
    switch (G->arr[id].status) {
    case NORMAL:
        newDir = chase(pacmanPos, ghostPos, dir, G, 0);
        if (!isIllegal(newPosition(ghostPos, newDir), G))
            ghostPos = newPosition(ghostPos, newDir);
        break;
    case SCARED_NORMAL: case SCARED_BLINKING:
        newDir = chase(pacmanPos, ghostPos, dir, G, 1);
        if (!isIllegalForScared(newPosition(ghostPos, newDir), G, pacmanPos))
            ghostPos = newPosition(ghostPos, newDir);
        break;
    case EYES:
        newDir = returnHome(ghostPos);
        enum direction compDir[2];
        otheAxisDirections(newDir, compDir);
        if (!isIllegalForScared(newPosition(ghostPos, newDir), G, pacmanPos))
            ghostPos = newPosition(ghostPos, newDir);
        else if (!isIllegalForScared(newPosition(ghostPos, oppositeDirection(newDir)), G, pacmanPos))
            ghostPos = newPosition(ghostPos, oppositeDirection(newDir));
        else if (!isIllegalForScared(newPosition(ghostPos, compDir[0]), G, pacmanPos))
            ghostPos = newPosition(ghostPos, compDir[0]);
        else if (!isIllegalForScared(newPosition(ghostPos, compDir[1]), G, pacmanPos))
            ghostPos = newPosition(ghostPos, compDir[1]);
        break;
    default:
        newDir = UNK_DIRECTION;
        break;
    }
    /* assigning the temporary variables to the ghost attributes */
    G->arr[id].dir = newDir;
    G->arr[id].pos = ghostPos;
    return ghostPos;
}

/* HELPER FUNCTIONS */

/* function to check if 2 positions are equals */
static int equalsPositions(struct position p1, struct position p2) {
    return (p1.i == p2.i && p1.j == p2.j);
}

/* function that return the opposite direction of the one in input */
static enum direction oppositeDirection(enum direction dir) {
    switch (dir) {
    case UP: return DOWN; break;
    case DOWN: return UP; break;
    case LEFT: return RIGHT; break;
    case RIGHT: return LEFT; break;
    default: return -1; break;
    }
}

/* get the direction that the ghost has to follow (vertical axis [0] and horizontal axis [1]) */
static enum direction* getDirectionToFollow(struct position g, struct position p, enum direction* r, int scared) {
    r[0] = -1; r[1] = -1;
    if (g.i > p.i) r[0] = UP;
    else if (g.i < p.i) r[0] = DOWN;
    if (g.j > p.j)  r[1] = LEFT;
    else if (g.j < p.j)  r[1] = RIGHT;
    if (r[0] == -1) r[0] = r[1];
    if (r[1] == -1) r[1] = r[0];
    if (scared) {
        r[0] = oppositeDirection(r[0]);
        r[1] = oppositeDirection(r[1]);
    }
    return r;
}

/* return the new position of the object in input */
static struct position newPosition(struct position pos, enum direction dir) {
    switch (dir) {
    case UP: pos.i = (pos.i + (rows - 1)) % rows; break;
    case DOWN: pos.i = (pos.i + 1) % rows; break;
    case RIGHT: pos.j = (pos.j + 1) % cols; break;
    case LEFT: pos.j = (pos.j + (cols - 1)) % cols; break;
    default: break;
    }
    return pos;
}

/* check if the position where the struct is going is illegal or not */
static int isIllegal(struct position pos, struct ghosts* G) {
    unsigned int i;
    /* checking if is a ghost */
    for (i = 0; i < G->num_ghost; ++i)
        if (G->arr[i].pos.i == pos.i && G->arr[i].pos.j == pos.j)
            return 1;
    return (IS_WALL(arena, pos));
}

/* same of isIllegal() but check also if the new position will go into pacman */
static int isIllegalForScared(struct position pos, struct ghosts* G, struct position pacmanPos) {
    return isIllegal(pos, G) || equalsPositions(pos, pacmanPos);
}

/* return the complementary directions of a direction */
static enum direction* otheAxisDirections(enum direction dir, enum direction* compDir) {
    switch (dir) {
    case UP: case DOWN:
        compDir[0] = RIGHT;
        compDir[1] = LEFT;
        break;
    case RIGHT: case LEFT:
        compDir[0] = UP;
        compDir[1] = DOWN;
        break;
    default: break;
    }
    return compDir;
}

/* function that returns the direction the ghost has to follow to chase pacman */
static enum direction chase(struct position pacmanPos, struct position ghostPos, enum direction dir, struct ghosts* G, int scared) {
    enum direction newDir;
    enum direction possibleDirections[2];
    getDirectionToFollow(ghostPos, pacmanPos, possibleDirections, scared);
    /* choosing the best way to follow pacman (if on the vertical axis (default) or the horizontal) */
    unsigned int index = 0;
    if (abs((int)ghostPos.j - (int)pacmanPos.j) > abs((int)ghostPos.i - (int)pacmanPos.i)) index = 1;
    newDir = possibleDirections[index];
    if (isIllegal(newPosition(ghostPos, newDir), G) || (!scared && newDir == oppositeDirection(dir))) {
        newDir = possibleDirections[!index];
        if (isIllegal(newPosition(ghostPos, newDir), G) || (!scared && newDir == oppositeDirection(dir))) {
            newDir = dir;
            if (isIllegal(newPosition(ghostPos, newDir), G)) {
                enum direction compDir[2];
                otheAxisDirections(newDir, compDir);
                index = 0;
                if (abs((int)ghostPos.j - (int)pacmanPos.j) > abs((int)ghostPos.i - (int)pacmanPos.i)) index = 1;
                newDir = compDir[index];
                if (isIllegal(newPosition(ghostPos, newDir), G)) {
                    newDir = compDir[!index];
                    if (isIllegal(newPosition(ghostPos, newDir), G))
                        newDir = oppositeDirection(dir);
                }
            }
        }
    }
    return newDir;
}

/* function that return the direction the ghost has to follow to come back home */
static enum direction returnHome(struct position ghostPos) {
    char c = arena[ghostPos.i][ghostPos.j];
    switch (c) {
    case UP_SYM: return UP;
    case RIGHT_SYM: return RIGHT;
    case DOWN_SYM: return DOWN;
    case LEFT_SYM: return LEFT;
    default: return UNK_DIRECTION;
    }
}

#endif