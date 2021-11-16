#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>
#include "global.h"
#include "arena.h"


/*
 * Opens the graphical display.
 */
void display_open(void);

/*
 * Closes the graphical display.
 */
void display_close(void);

/*
 * Plots the current arena.
 */
void display_arena();

/* 
 * Read from keyboard.
 */
int display_read(void);

/*
 * Pauses the game. 
 */
int display_pause(void);

/*
 * Plots a score.
 */
void display_ghost_score();

/*
 * Plots the starting arena into the graphical display.
 */
void display_start_arena(void);

/*
 * Plots the gameover arena.
 */
void display_win_arena(void);

/*
 * Plots the gameover arena.
 */
void display_lose_arena(void);

#endif
