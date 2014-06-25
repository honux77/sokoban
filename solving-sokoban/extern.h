#include	<stdio.h>
#include	<stdlib.h>

extern int maze[MAXSIZE][MAXSIZE];
extern int bags[MAXSIZE][MAXSIZE];
extern int size_x, size_y;
extern int man_x, man_y;
extern int n_bags, n_goal;
extern unsigned int step, source;
extern int distance[MAXSIZE][MAXSIZE];
extern  unsigned char buffer[BUFFERSIZE];
extern int n_open;
extern int data_length;
extern unsigned char *close_p[CLOSESIZE];
extern int n_close;

extern int intensive;
extern int n_entrance;

extern int 	wallx[MAXSIZE][MAXSIZE], wally[MAXSIZE][MAXSIZE]; 
extern int	wallu[MAXSIZE][MAXSIZE];
extern int	wallcap[MAXWALL];
extern int	n_wall;

extern struct LOCATION{ 
	int x;
       	int y;
} baggage[MAXBAGS], entrance[MAXENT], goal[MAXBAGS];

extern struct CELL {
	unsigned char *d;
	struct CELL *next;} cell[HASHSIZE];
extern struct CELL *hash_head[HASH];

extern unsigned int hp;

extern int vx[5], vy[5];
extern int wx[9], wy[9];

extern struct BAGINFO{ 
	int n;
        int v; /* 0: up; 1: right; 2: down; 3:left; */
        int step;
} movable_bags[MAXBAGS];

extern FILE	*ofp;

extern int	_max_val, max_val;
