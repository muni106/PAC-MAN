#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "queue.h"
#include "arena.h"
#include "ghosts.h"
#include "pacman.h"

static const struct position UNK_POSITION = {-1,-1}; // Set unknown position to (UINT_MAX,UINT_MAX)

enum pacman_action {EAT_PILL, EAT_FOOD, TOUCH_GHOST, PACMAN_MOVE};
enum ghost_action {TOUCH_PACMAN, BACK_HOME, GHOST_MOVE};

struct arena {
  char **arena;
	char **path;
  unsigned int nrow;
  unsigned int ncol;
  unsigned int food;
  unsigned int score;
	unsigned int ghost_score;
	unsigned int timer;
	struct pacman *pacman;
  struct ghosts *ghosts;
} *A = NULL;


/* Count dots in the char matrix */
static unsigned int count_food(char **M, unsigned int nrow, unsigned int ncol);

/* Find pacman position */
static struct position pacman_position(char **M, unsigned int nrow, unsigned int ncol);

/* Setup pacman position and direction */
static struct pacman *setup_pacman(char **M, char **N, unsigned int nrow, unsigned int ncol);

/* Count number of ghosts in the char matrix */
static unsigned int count_ghosts(char **M, unsigned int nrow, unsigned int ncol);

/* Setup ghosts position and direction */
static struct ghosts *setup_ghosts(char **M, char **N, unsigned int nrow, unsigned int ncol);

/* Updates arena data */
static void arena_update(void);

/* Recursive path finding algorithm */
static void set_path(char **H, unsigned int nrow, unsigned int ncol, struct position pos, struct queue *Q);

/* Setup path matrix */
static char **setup_homepath(char **M, unsigned int nrow, unsigned int ncol);

int arena_setup(char *arena_file, char *positions_file) {
	unsigned int nrow, ncol, m, n;
	char **Amat = matrix_read(arena_file,&nrow,&ncol);
	char **Pmat = matrix_read(positions_file,&m,&n);

	A = (struct arena *)calloc(1,sizeof(struct arena));

	if(A == NULL || Amat == NULL || Pmat == NULL || nrow != m) {
		matrix_free(Amat);
		matrix_free(Pmat);
		free(A);
		return 1;	
	} else {
		A->arena       = Amat;
		A->path        = setup_homepath(Pmat,nrow,n);
		A->nrow        = nrow;
		A->ncol        = n;
		A->food        = count_food(Amat,nrow,ncol);				
		A->score       = 0;
		A->ghost_score = GHOST_SCORE;
		A->timer       = 0;
		A->pacman      = setup_pacman(Pmat,A->path,nrow,n);	
		A->ghosts      = setup_ghosts(Pmat,A->path,nrow,n);	

		matrix_free(Pmat);

		if(A->pacman == NULL || A->ghosts == NULL) {
			arena_destroy();
			return 1;
		}
		else {
			return 0;
		}
	}
}

void arena_destroy(void) {
	if(A != NULL) {
		pacman_destroy(A->pacman);
		ghosts_destroy(A->ghosts);
		matrix_free(A->arena);
		matrix_free(A->path);
		free(A);		
		A = NULL;
	}
}


unsigned int arena_rows(void) {
	return A == NULL ? 0 : A->nrow;
}

unsigned int arena_cols(void) {
	return A == NULL ? 0 : A->ncol;
}


char **arena_matrix(void) {
	return A == NULL ? NULL : A->arena;
}

unsigned int arena_score(void) {
	return A->score;
}

unsigned int arena_ghost_score(void) {
	return A->ghost_score;
}

unsigned int arena_food(void) {
	return A->food;
}

struct position arena_pacman_position(void) {
	return A == NULL ? UNK_POSITION : pacman_get_position(A->pacman);
}

enum direction arena_pacman_direction(void) {
	return A == NULL ? UNK_DIRECTION : pacman_get_direction(A->pacman);
}

enum pacman_status arena_pacman_status(void) {
	return A == NULL ? UNK_PACMAN_STATUS : pacman_get_status(A->pacman);
}

struct position arena_ghost_position(unsigned int id) {
	return A == NULL ? UNK_POSITION : ghosts_get_position(A->ghosts,id);
}

enum ghost_status arena_ghost_status(unsigned int id) {
	return A == NULL ? UNK_GHOST_STATUS : ghosts_get_status(A->ghosts,id);
}

unsigned int arena_ghost_number(void) {
	return A == NULL ? 0 : ghosts_get_number(A->ghosts);
}


static enum pacman_action pacman_action(unsigned int *ghost_id) {
  struct position p = pacman_get_position(A->pacman);
  unsigned int id, num = ghosts_get_number(A->ghosts);

  for(id = 0; id < num; id++) {
    struct position g = ghosts_get_position(A->ghosts,id);
    if(p.i == g.i && p.j == g.j) {
			*ghost_id = id;
			return TOUCH_GHOST;
		}
  }

  if(A->arena[p.i][p.j*3] == PILL_SYM) {
    return EAT_PILL;
  } else if (A->arena[p.i][p.j*3] == FOOD_SYM) {
    return EAT_FOOD;
  } else {
    return PACMAN_MOVE;
  }
}

enum game_status arena_pacman_move(enum direction dir) {
	enum game_status stat = CONTINUE;
	struct position pos;
	unsigned int id, num = ghosts_get_number(A->ghosts);
	enum pacman_action act;

  pos = pacman_move(A->pacman,dir);

  switch((act = pacman_action(&id))) {
    case EAT_PILL:    A->timer = 0;
											A->score += PILL_SCORE;
											A->arena[pos.i][pos.j*3] = SPACE_SYM;
											pacman_set_status(A->pacman,POWERED);
                      for(id = 0; id < num; id++)
                        if(ghosts_get_status(A->ghosts,id) != EYES)
													ghosts_set_status(A->ghosts,id,SCARED_NORMAL);
                      break;
    case EAT_FOOD:    A->score += FOOD_SCORE;
										  A->arena[pos.i][pos.j*3] = SPACE_SYM;	
                      if(--A->food == 0)
                        stat = WIN;
                      break;
    case TOUCH_GHOST: if(ghosts_get_status(A->ghosts,id) != EYES) {
                        if(pacman_get_status(A->pacman) == POWERED) {
													ghosts_set_status(A->ghosts,id,EYES);
													A->ghost_score *= 2;
													A->score += A->ghost_score;
													stat = GHOST_EAT;
                        } else {
                          stat = LOSE;
                        }
                      }
                      break;
    default:          break;
  }
	arena_update();
  return stat;
}


static enum ghost_action ghost_action(unsigned int id) {
  struct position p = pacman_get_position(A->pacman);
	struct position g = ghosts_get_position(A->ghosts,id);

	if(p.i == g.i && p.j == g.j)
		return TOUCH_PACMAN;
  else if(A->path[g.i][g.j] == HOME_SYM)
		return BACK_HOME;
	else
		return GHOST_MOVE;
}


enum game_status arena_ghosts_move(void) {
	enum game_status stat = CONTINUE;
  unsigned int id, num = ghosts_get_number(A->ghosts);
	enum ghost_action act;
	
	for(id = 0; id < num; id++) {
		ghosts_move(A->ghosts,A->pacman,id);
		switch((act = ghost_action(id))) {
			case TOUCH_PACMAN: if(ghosts_get_status(A->ghosts,id) != EYES) {
														if(pacman_get_status(A->pacman) == POWERED) {
															ghosts_set_status(A->ghosts,id,EYES);
															A->ghost_score *= 2;
															A->score += A->ghost_score;
															stat = GHOST_EAT;
														} else {
															stat = LOSE;
														}
													}
													break;
			case BACK_HOME: if(ghosts_get_status(A->ghosts,id) == EYES) {
												if(pacman_get_status(A->pacman) == POWERED)
													ghosts_set_status(A->ghosts,id,SCARED_NORMAL);
												else 
													ghosts_set_status(A->ghosts,id,NORMAL);
											} 	
											break;
			case GHOST_MOVE: stat = CONTINUE;
		}

		if(stat != CONTINUE) {
			arena_update();
			return stat;
		}
	}
	arena_update();
	return stat;
}

static void arena_update(void) {
  unsigned int id, num = ghosts_get_number(A->ghosts);

	A->timer++;
  if(A->timer >= POWER_TIMEOUT) {
    A->ghost_score    = GHOST_SCORE;
		pacman_set_status(A->pacman,WEAK);
    for(id = 0; id < num; id++)
      if(ghosts_get_status(A->ghosts,id) != EYES)
        ghosts_set_status(A->ghosts,id,NORMAL);
  } else if(A->timer < POWER_TIMEOUT && A->timer >= POWER_TIMEOUT_WARNING) {
		for(id = 0; id < num; id++)
			if(ghosts_get_status(A->ghosts,id) == SCARED_NORMAL)
				ghosts_set_status(A->ghosts,id,SCARED_BLINKING);
  }
}



static unsigned int count_food(char **M, unsigned int nrow, unsigned int ncol) {
  unsigned int i, j, dots = 0;

  for(i = 0; i < nrow; i++)
    for(j = 0; j < ncol; j++)
      dots += M[i][j] == FOOD_SYM;

  return dots;
}

static unsigned int count_ghosts(char **M, unsigned int nrow, unsigned int ncol) {
	unsigned int num_ghosts = 0;
	struct position pos;
	
	for(pos.i = 0; pos.i < nrow; pos.i++)
		for(pos.j = 0; pos.j < ncol; pos.j++)
			num_ghosts += IS_GHOST(M,pos);
	return num_ghosts;
}

static struct position pacman_position(char **M, unsigned int nrow, unsigned int ncol) {
	struct position pos;

	for(pos.i = 0; pos.i < nrow; pos.i++)
		for(pos.j = 0; pos.j < ncol; pos.j++)
			if(IS_PACMAN(M,pos))
				return pos;
	return UNK_POSITION;
}

static struct pacman *setup_pacman(char **M, char **N, unsigned int nrow, unsigned int ncol) {
	struct pacman *P    = pacman_setup();
	struct position pos = pacman_position(M,nrow,ncol);	
	enum direction dir  = UNK_DIRECTION;
	
	if(pos.i == UNK_POSITION.i && pos.j == UNK_POSITION.j) {
		pacman_destroy(P);
		return NULL;
	} else {
		switch(M[pos.i][pos.j]) {
			case PACMAN_LSYM: dir = LEFT;  break;
			case PACMAN_RSYM: dir = RIGHT; break;
			case PACMAN_USYM: dir = UP;    break;
			case PACMAN_DSYM: dir = DOWN;  break;
		}
		pacman_set_position(P,pos);
		pacman_set_direction(P,dir);
		pacman_set_status(P,WEAK);
		pacman_set_arena(P,N,nrow,ncol);
	}
	return P;
}

static struct ghosts *setup_ghosts(char **M, char **N, unsigned int nrow, unsigned int ncol) {
	unsigned int id = 0, num_ghosts = count_ghosts(M,nrow,ncol);
	struct ghosts *G = ghosts_setup(num_ghosts);
	struct position pos;
	
	for(pos.i = 0; pos.i < nrow; pos.i++)
		for(pos.j = 0; pos.j < ncol; pos.j++) {
			if(IS_GHOST(M,pos)) {
				ghosts_set_position(G,id,pos);
				ghosts_set_status(G,id,NORMAL);
				id++;
			}
		}

	ghosts_set_arena(G,N,nrow,ncol);

	return G;
}

static char **setup_homepath(char **M, unsigned int nrow, unsigned int ncol) {
	char **H = matrix_copy(M,nrow,ncol);
	unsigned int i, j;
	struct queue *Q = queue_alloc(nrow*ncol);
	struct position pos;

	for(i = 0; i < nrow; i++)
		for(j = 0; j < ncol; j++)
			if(H[i][j] == HOME_SYM) {
				pos.i = i; pos.j = j;
				enqueue(Q,pos);
			}
				
	while(!empty_queue(Q)) 
		set_path(H,nrow,ncol,dequeue(Q),Q);

	queue_free(Q);

	return H;
}

static void set_path(char **H, unsigned int nrow, unsigned int ncol, struct position pos, struct queue *Q) {
	struct position new;

	if(IS_PATH(H,pos)) {
		if(pos.i > 0) {
			new.i = pos.i-1;  new.j = pos.j;
			if(!IS_WALL(H,new) && !IS_PATH(H,new)) {
				H[new.i][new.j] = DOWN_SYM;
				enqueue(Q,new);
			}
		}
		if(pos.i+1 < nrow) {
			new.i = pos.i+1;  new.j = pos.j;
			if(!IS_WALL(H,new) && !IS_PATH(H,new)) {
				H[new.i][new.j] = UP_SYM;
				enqueue(Q,new);
			}
		}
		if(pos.j > 0) {
			new.i = pos.i;  new.j = pos.j-1;
			if(!IS_WALL(H,new) && !IS_PATH(H,new)) {
				H[new.i][new.j] = RIGHT_SYM;
				enqueue(Q,new);
			}
		}
		if(pos.j+1 < ncol) {
			new.i = pos.i;  new.j = pos.j+1;
			if(!IS_WALL(H,new) && !IS_PATH(H,new)) {
				H[new.i][new.j] = LEFT_SYM;
				enqueue(Q,new);
			}
		}
	}
}

