CFLAG = -Wall -pedantic -Wno-comment -O3 -std=c89
 
all: pacman 

queue.o: global.h queue.h queue.c
	gcc $(CFLAG) -c queue.c

matrix.o: matrix.h matrix.c
	gcc $(CFLAG) -c matrix.c

arena.o: global.h arena.h arena.c
	gcc $(CFLAG) -c arena.c

pacman.o: global.h pacman.h pacman.c
	gcc $(CFLAG) -c pacman.c

ghosts.o: ghosts.h ghosts.c
	gcc $(CFLAG) -c ghosts.c

display.o: global.h display.h display.c
	gcc $(CFLAG) -c display.c

pacman: queue.o matrix.o arena.o pacman.o ghosts.o display.o main.c
	gcc $(CFLAG) -lm -lcurses queue.o matrix.o arena.o ghosts.o display.o pacman.o main.c -o pacman 

clean:
	rm -rf *.o pacman
