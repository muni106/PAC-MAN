#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "display.h"
#include "arena.h"



int main(int argc, char *argv[]) {
	unsigned int stat, input;
	char *arena_file = "arena.txt";
	char *positions_file = "positions.txt";
	
	if(argc != 3 && argc != 1) {
		fprintf(stderr,"Usage: pacman [arena] [positions]\n");
		return 1;
	}

	if(argc == 3) {
		arena_file = argv[1];
		positions_file = argv[2];
	}

	do {
		stat = CONTINUE;
	 	arena_setup(arena_file,positions_file);

		display_open();

		display_start_arena();

		do {
			display_arena();

			input = arena_pacman_direction();

			switch(display_read()) {
				case PAUSE_KEY   : display_pause();  break;
				case RESTART_KEY : stat = RESTART;   break;
				case QUIT_KEY    : stat = QUIT;      break;
				case LEFT_KEY    : input = LEFT;     break;
				case RIGHT_KEY   : input = RIGHT;    break;
				case DOWN_KEY    : input = DOWN;     break;
				case UP_KEY      : input = UP;       break;
				default          : input = arena_pacman_direction(); break;
			}
	
			if(stat == CONTINUE) {
				if((stat = arena_pacman_move(input)) == CONTINUE || stat == GHOST_EAT) {
					if(stat == GHOST_EAT)
						display_ghost_score();
					if((stat = arena_ghosts_move()) == CONTINUE || stat == GHOST_EAT) {
						if(stat == GHOST_EAT)
							display_ghost_score();
						stat = CONTINUE;
					}
				}
			}
		} while(stat == CONTINUE);

		if(stat != RESTART) {
			if(stat==LOSE)
				display_lose_arena();
			else if(stat == WIN)
				display_win_arena();

			do {
				switch(display_read()) {
					case RESTART_KEY : stat = RESTART; break;
					case QUIT_KEY    : stat = QUIT;    break;
					default:           stat = UNK_GAME_STATUS;
				}
			} while(stat == UNK_GAME_STATUS);
		}

		display_close();
		arena_destroy();
	} while(stat == RESTART);

  return 0;
}
