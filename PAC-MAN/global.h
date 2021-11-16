#ifndef GLOBAL_H
#define GLOBAL_H

/* Arena txt symbols */
#define HOME_SYM    'X'
#define SPACE_SYM   ' '
#define FOOD_SYM    '.'
#define DOOR_SYM    '_'
#define PILL_SYM    '*'
#define GHOST_SYM   'G'
/* Pacman symbols */
#define PACMAN_LSYM '>'
#define PACMAN_RSYM '<'
#define PACMAN_USYM 'v'
#define PACMAN_DSYM '^'
/* Wall symbols */
#define HWALL_SYM   '-'
#define VWALL_SYM   '|'
#define XWALL_SYM   'x'
#define ULCORN_SYM  '/'
#define URCORN_SYM  '\\'
/* Path symbols */
#define UP_SYM      'U'
#define LEFT_SYM    'L'
#define RIGHT_SYM   'R'
#define DOWN_SYM    'D'

/* True if arena (A) position pos=(i,j) is a wall */
#define IS_WALL(A,pos) (A[pos.i][pos.j] == URCORN_SYM || A[pos.i][pos.j] == ULCORN_SYM || \
                        A[pos.i][pos.j] == VWALL_SYM  || A[pos.i][pos.j] == HWALL_SYM  || \
                        A[pos.i][pos.j] == XWALL_SYM)

/* True if arena (A) position pos=(i,j) contains pacman */
#define IS_PACMAN(A,pos) (A[pos.i][pos.j] == PACMAN_LSYM || A[pos.i][pos.j] == PACMAN_RSYM || \
                          A[pos.i][pos.j] == PACMAN_USYM || A[pos.i][pos.j] == PACMAN_DSYM)

#define IS_PATH(A,pos) (A[pos.i][pos.j] == UP_SYM    || A[pos.i][pos.j] == LEFT_SYM || \
                        A[pos.i][pos.j] == RIGHT_SYM || A[pos.i][pos.j] == DOWN_SYM || A[pos.i][pos.j] == HOME_SYM)

/* True if arena (A) position pos=(i,j) contains a ghost */
#define IS_GHOST(A,pos)  (A[pos.i][pos.j] == GHOST_SYM)


/* Game scores */
#define FOOD_SCORE  10
#define PILL_SCORE  100
#define GHOST_SCORE 100 

/* Game timeouts */
#define POWER_TIMEOUT 200 // 100 pacman moves and 100 ghosts moves 
#define POWER_TIMEOUT_WARNING 160 
#define TIMEOUT  150 // getch() timeotut (ms)

/* Game keys */
#define QUIT_KEY    'q'
#define PAUSE_KEY   'p'
#define RESTART_KEY 'r'
#define LEFT_KEY     KEY_LEFT
#define RIGHT_KEY    KEY_RIGHT
#define DOWN_KEY     KEY_DOWN
#define UP_KEY       KEY_UP

/* Ghost status */
enum ghost_status {NORMAL, SCARED_NORMAL, SCARED_BLINKING, EYES, UNK_GHOST_STATUS};

/* Pacman status */
enum pacman_status {WEAK, POWERED, UNK_PACMAN_STATUS};

/* Game status */
enum game_status {CONTINUE, RESTART, WIN, LOSE, QUIT, GHOST_EAT, UNK_GAME_STATUS};

/* Directions */
enum direction {LEFT, UP, RIGHT, DOWN, UNK_DIRECTION};


struct position {
  unsigned int i;
  unsigned int j;
};

#endif
