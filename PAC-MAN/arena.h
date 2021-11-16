#ifndef ARENA_H 
#define ARENA_H

#include "global.h"

/* Inizialize an arena struct. Reads the arena matrix from "file".*/
int arena_setup(char *arena_file, char *postions_file);

/* Destroy an arena structure. */
void arena_destroy(void);

/* Return the number of rows in the arena matrix. */
unsigned int arena_rows(void);

/* Return the number of columns in the arena matrix. */
unsigned int arena_cols(void);

/* Return the arena matrix. */
char **arena_matrix(void);

/* Return the current global score of the game. */
unsigned int arena_score(void);

/* Return the current amount of food (dots) in the arena. */
unsigned int arena_food(void);

/* Return the current score gained eating a ghost */ 
unsigned int arena_ghost_score(void);

/* Return pacman position */ 
struct position arena_pacman_position(void);

/* Return pacman direction */ 
enum direction arena_pacman_direction(void);

/* Return pacman status */
enum pacman_status arena_pacman_status(void);

/* Return the position of the ghost id. */
struct position arena_ghost_position(unsigned int id);

/* Return the status of the ghost id. */
enum ghost_status arena_ghost_status(unsigned int id);

/* Return the number of ghosts */
unsigned int arena_ghost_number(void);

/* Move pacman in direction dir */
enum game_status arena_pacman_move(enum direction dir);

/* Move all the ghosts */
enum game_status arena_ghosts_move(void);

#endif
