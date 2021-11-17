#include <stdlib.h>
#include "display.h"


#define SLEEP_TIME 0.5
#define BLINK
#define WIN

#ifdef WIN
#include <time.h>
#else
#include "sys/timeb.h"
#endif

unsigned int START_ROW, START_COL;

static int PACMAN_FLAG = 0;

enum {
	COLOR_LIGHT_BLUE = COLOR_CYAN,
	COLOR_PINK       = COLOR_MAGENTA,
  COLOR_ORANGE     = COLOR_GREEN,
  COLOR_DOOR       = COLOR_RED,
  COLOR_PACMAN     = COLOR_YELLOW,
  COLOR_BKGD       = COLOR_BLACK
};

enum pairs {
	PACMAN_PAIR,
	BLINKY_PAIR,
	PINKY_PAIR,
	INKY_PAIR,
	CLYDE_PAIR,
	FRIGHT_PAIR,
	FRIGHT_FLASH_PAIR,
	EYES_PAIR,	
	PILL_PAIR,
	DOOR_PAIR,
	DARK_WALL_PAIR,
	GAME_OVER_TEXT_PAIR
};

enum pairs CH_PAIR[] = {PACMAN_PAIR, BLINKY_PAIR, PINKY_PAIR, INKY_PAIR, CLYDE_PAIR};

static void mysleep(double secs) {
	double goal = secs+1.0*clock()/CLOCKS_PER_SEC;
	while(goal > 1.0*clock()/CLOCKS_PER_SEC);
}

void display_open(void) {
	unsigned int nrow = arena_rows();
	unsigned int ncol = (arena_cols()-1)*3+1;

	initscr();
	cbreak();
	noecho();               
	keypad(stdscr, TRUE); // make keys work
	curs_set(0);          // hide cursor
	timeout(TIMEOUT);     // Sets the waiting time for getch()
	
	start_color();

  init_pair(DARK_WALL_PAIR,      COLOR_BLUE,   COLOR_BKGD);
  init_pair(DOOR_PAIR,           COLOR_DOOR,   COLOR_BKGD);
  init_pair(PACMAN_PAIR,         COLOR_YELLOW, COLOR_BKGD);
  init_pair(GAME_OVER_TEXT_PAIR, COLOR_RED,    COLOR_BKGD);
  init_pair(BLINKY_PAIR,         COLOR_WHITE,  COLOR_RED);
  init_pair(INKY_PAIR,           COLOR_WHITE,  COLOR_LIGHT_BLUE);
  init_pair(PINKY_PAIR,          COLOR_WHITE,  COLOR_PINK);
  init_pair(CLYDE_PAIR,          COLOR_WHITE,  COLOR_ORANGE);
  init_pair(FRIGHT_PAIR,         COLOR_WHITE,  COLOR_BLUE);
  init_pair(FRIGHT_FLASH_PAIR,   COLOR_BLUE,   COLOR_WHITE);
  init_pair(EYES_PAIR,           COLOR_WHITE,  COLOR_BKGD);
	init_pair(PILL_PAIR,           COLOR_YELLOW, COLOR_BKGD);

	START_ROW = LINES < nrow ? nrow/2 : (LINES - nrow)/2;
	START_COL = COLS  < ncol ? ncol/2 : (COLS  - ncol)/2;

	PACMAN_FLAG = 0;
}

void display_close() {
	endwin();
}


static int select_border(char **A, unsigned int nrow, unsigned int ncol, unsigned int i, unsigned int j) {
	if(A[i][j] == ULCORN_SYM) {
		if(i < nrow - 1 && (A[i+1][j]==VWALL_SYM || A[i+1][j]==URCORN_SYM) && j < ncol-1 && (A[i][j+1]==HWALL_SYM || A[i][j+1]==URCORN_SYM)) return ACS_ULCORNER;

		if(i > 0        && (A[i-1][j]==VWALL_SYM || A[i-1][j]==URCORN_SYM) && j > 0      && (A[i][j-1]==HWALL_SYM || A[i][j-1]==URCORN_SYM)) return ACS_LRCORNER;
		
	} else if (A[i][j] == URCORN_SYM) {
		if(i < nrow - 1 && (A[i+1][j]==VWALL_SYM || A[i+1][j]==ULCORN_SYM) && j > 0      && (A[i][j-1]==HWALL_SYM || A[i][j-1]==ULCORN_SYM)) return ACS_URCORNER;

		if(i > 0        && (A[i-1][j]==VWALL_SYM || A[i-1][j]==ULCORN_SYM) && j < ncol-1 && (A[i][j+1]==HWALL_SYM || A[i][j+1]==ULCORN_SYM)) return ACS_LLCORNER;
	} 
	return A[i][j];
}

static void display_board(char **A, unsigned int nrow, unsigned int ncol) {
	unsigned int i,j;

	for(i = 0; i < nrow; i++)
		for(j = 0; j < ncol; j++) {
			switch(A[i][j]) {
				case VWALL_SYM :attron(COLOR_PAIR(DARK_WALL_PAIR));
									 			mvaddch(START_ROW+i, START_COL+j,ACS_VLINE);
									 			attroff(COLOR_PAIR(DARK_WALL_PAIR));
									 			break;
				case HWALL_SYM :attron(COLOR_PAIR(DARK_WALL_PAIR));
									 			mvaddch(START_ROW+i, START_COL+j,ACS_HLINE);
									 			attroff(COLOR_PAIR(DARK_WALL_PAIR));
									 			break;
				case ULCORN_SYM:
				case URCORN_SYM:attron(COLOR_PAIR(DARK_WALL_PAIR));
									 			mvaddch(START_ROW+i, START_COL+j,select_border(A,nrow,ncol,i,j));
									 			attroff(COLOR_PAIR(DARK_WALL_PAIR));
									 			break;
				case DOOR_SYM : attron(COLOR_PAIR(DOOR_PAIR));
									 			mvaddch(START_ROW+i, START_COL+j,'_');
									 			attroff(COLOR_PAIR(DOOR_PAIR));
									 			break;
				case PILL_SYM : 
												#ifdef BLINK
												attron(A_BLINK);
												#endif
									 			attron(COLOR_PAIR(PILL_PAIR));
									 			mvaddch(START_ROW+i, START_COL+j,ACS_DIAMOND);
											  attroff(COLOR_PAIR(PILL_PAIR));
											  attroff(A_BLINK);
									 			break;
				case FOOD_SYM : mvaddch(START_ROW+i, START_COL+j,'.');
									 			break;
				default      : 	mvaddch(START_ROW+i, START_COL+j,' '); break;
			}
		}
}

static void display_pacman_eat(struct position pos) {
	int i = pos.i, j = pos.j;		

	attron(COLOR_PAIR(PILL_PAIR));
	//attron(COLOR_PAIR(PACMAN_PAIR));
	mvaddch(START_ROW+i, START_COL+j,'O');
	//attroff(COLOR_PAIR(PACMAN_PAIR));
	attroff(COLOR_PAIR(PILL_PAIR));
}

static void display_pacman(struct position pos, enum direction dir) {	
	int i = pos.i, j = pos.j;

	attron(COLOR_PAIR(PILL_PAIR));
	//attron(COLOR_PAIR(PACMAN_PAIR));
	switch(dir) {
		case LEFT:  //mvaddch(START_ROW+i, START_COL+j,ACS_RARROW);
								mvaddch(START_ROW+i, START_COL+j,PACMAN_LSYM);
								break;
		case RIGHT: //mvaddch(START_ROW+i, START_COL+j,ACS_LARROW);
								mvaddch(START_ROW+i, START_COL+j,PACMAN_RSYM);
								break;
		case UP:    //mvaddch(START_ROW+i, START_COL+j,ACS_DARROW);
								mvaddch(START_ROW+i, START_COL+j,PACMAN_USYM);
								break;
		case DOWN:  //mvaddch(START_ROW+i, START_COL+j,ACS_UARROW);
								mvaddch(START_ROW+i, START_COL+j,PACMAN_DSYM);
								break;
		case UNK_DIRECTION: break;
	}
	//attroff(COLOR_PAIR(PACMAN_PAIR));
	attroff(COLOR_PAIR(PILL_PAIR));
}

static void display_ghost(unsigned int id, struct position pos, enum ghost_status stat) {
	int i = pos.i, j = pos.j;
	int col = stat == SCARED_NORMAL ? FRIGHT_PAIR : stat == EYES ? EYES_PAIR : stat == SCARED_BLINKING ? FRIGHT_FLASH_PAIR : CH_PAIR[(id % 4)+1];

	attron(COLOR_PAIR(col));
	#ifdef BLINK
	if(stat == SCARED_BLINKING) attron(A_BLINK);
	#endif
	mvaddch(START_ROW+i, START_COL+j,'m');
	if(stat != EYES) {
		mvaddch(START_ROW+i, START_COL+j-1,' ');
		mvaddch(START_ROW+i, START_COL+j+1,' ');
	}
	attroff(A_BLINK);
	attroff(COLOR_PAIR(col));
}

static void display_score(unsigned int score) {
	mvprintw(START_ROW - 1, (COLS-17)/2,"S C O R E %7u",score);
}

static void display_menu(unsigned int nrows) {
	mvprintw(START_ROW + nrows+1,START_COL,"[Q]uit  [P]ause [R]estart");
}

static void display_gameover(char *str) {
	#ifdef BLINK
	attron(A_BLINK);
	#endif
	attron(COLOR_PAIR(GAME_OVER_TEXT_PAIR));
	mvprintw(START_ROW+17,(COLS-18)/2,str);
	attroff(COLOR_PAIR(GAME_OVER_TEXT_PAIR));
	attroff(A_BLINK);
}

static void display_start() {
	int i;
	for(i = 3; i > 0; i--) {
		#ifdef BLINK
	  attron(A_BLINK);
		#endif
		attron(COLOR_PAIR(GAME_OVER_TEXT_PAIR));
	  mvprintw(START_ROW+17,(COLS-10)/2,"S T A R T   %d",i);
		attroff(COLOR_PAIR(GAME_OVER_TEXT_PAIR));
	  attroff(A_BLINK);
		refresh();
		mysleep(1);
	}
}

void display_arena(void) {
	unsigned int nrow = arena_rows();
	unsigned int ncol = (arena_cols()-1)*3+1;
	static struct position old_pos = {0,0};
	char **M = arena_matrix();

	if(M != NULL) {
		int g, ghost_num = arena_ghost_number();
		struct position pos;
		enum direction dir;

		erase();
		display_board(M,nrow,ncol);

		// Print ghosts
		for(g = 0; g < ghost_num; g++) {
				pos = arena_ghost_position(g); pos.j *= 3;
				display_ghost(g,pos,arena_ghost_status(g));
		}
		
		// Print pacman
		pos = arena_pacman_position(); pos.j *= 3;
		dir = arena_pacman_direction();
		if((PACMAN_FLAG = ((PACMAN_FLAG + 1) % 2)) || (pos.i == old_pos.i && pos.j == old_pos.j))
			display_pacman(pos,dir);
		else
			display_pacman_eat(pos);
		old_pos = pos;

		display_score(arena_score());
		display_menu(nrow);
		refresh();
	}
}

void display_start_arena(void) {
	display_arena();
	display_start();
}

int display_read(void) {
	#ifdef WIN
	return getch();
	#else
	struct timeval tval_start, tval_end, tval_res;
	int delay = TIMEOUT, input = ERR, input_save = ERR;
	
	while(delay > 0) {
		gettimeofday(&tval_start, NULL);
		timeout(delay);
		if((input = getch()) == PAUSE_KEY || input == QUIT_KEY || input == RESTART_KEY) {
			delay = 0;
		} else {
			gettimeofday(&tval_end, NULL);
			timersub(&tval_end, &tval_start, &tval_res);
			delay -= tval_res.tv_sec * 1000.0 + tval_res.tv_usec/1000.0;
		}
		input_save = input != ERR ? input : input_save;
	}
	return input_save;
	#endif
}

int display_pause(void) {
	int input;
	timeout(-1);
	input = getch();
	timeout(TIMEOUT);
	return input;
}

void display_ghost_score() {
	struct position pos = arena_pacman_position();
	display_arena();
	mvprintw(START_ROW + pos.i, START_COL + (int)pos.j*3,"%d",arena_ghost_score());
	refresh();
	mysleep(1);
	display_arena();
	refresh();
} 

void display_lose_arena(void) {
	unsigned int nrow = arena_rows();
  unsigned int ncol = (arena_cols()-1)*3+1;
  char **M = arena_matrix();

  if(M != NULL) {
    struct position pos;

    erase();
    display_board(M,nrow,ncol);
		display_score(arena_score());
		display_menu(nrow);
    pos = arena_pacman_position(); pos.j *= 3;
		display_pacman(pos, LEFT);
		refresh(); mysleep(SLEEP_TIME);
		display_pacman(pos, DOWN);
		refresh(); mysleep(SLEEP_TIME);
		display_pacman(pos, RIGHT);
		refresh(); mysleep(SLEEP_TIME);
		display_pacman(pos, UP);
		refresh(); mysleep(SLEEP_TIME);
		
		display_gameover("G A M E   O V E R!");
	
    refresh();
  }
}

void display_win_arena(void) {
	display_arena();
	display_gameover("G O O D   G A M E!");
	refresh();
}