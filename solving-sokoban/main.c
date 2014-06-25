/*                                                                   */
/*                                                                   */
/*        Solving Sokoban                                            */
/*                                                                   */
/*        by A. Ueno, K. Nakayama and T. Hikita                      */
/*        Dept. of Computer Science, Meiji University                */
/*        1997                                                       */
/*                                                                   */
/*        cf.  A. Ueno, K. Nakayama and T. Hikita :                  */
/*        Sokoban (in Japanese), in "Game Programming",              */
/*        bit (special issue), pp.158-172, Kyoritsu Shuppan, 1997.   */
/*                                                                   */
/*                                                                   */

/*
 *	Patch Level 5 (1998.02.06)
 *	Modefied by Y.Murase, K.Takahashi, and M.hiramatsu.
 */

#include        <stdio.h>
#include        <stdlib.h>
#include        "soukodef.h"
 
int bags[MAXSIZE][MAXSIZE];
int maze[MAXSIZE][MAXSIZE];
int size_x, size_y;
int man_x, man_y;
int n_bags, n_goal;
unsigned int step, source;
int distance[MAXSIZE][MAXSIZE];
unsigned char buffer[BUFFERSIZE];
int n_open=0;
int cell_ptr=0;
int data_length;
unsigned char *close_p[CLOSESIZE];
int n_close;
int	_max_val=0, max_val=0;
int	wallx[MAXSIZE][MAXSIZE], wally[MAXSIZE][MAXSIZE]; 
int	wallu[MAXSIZE][MAXSIZE];
int	wallcap[MAXWALL];
int	n_wall = 0;
 
struct LOCATION {
	int x;
        int y;
} baggage[MAXBAGS], entrance[MAXENT], goal[MAXBAGS];
 
struct CELL {
	unsigned char	*d;
     	struct CELL 	*next;
} cell[HASHSIZE];

struct CELL *hash_head[HASH];
unsigned int hp = 0;

int	vx[5] = {0,1,0,-1,0}, vy[5] = {-1,0,1,0,-1};
int	wx[9] = {0,1,1,1,0,-1,-1,-1,0}, wy[9] = {-1,-1,0,1,1,1,0,-1,-1};

struct{
  int n;
  int v; /* 0: up; 1: right; 2: down; 3:left; */
  int step;
} movable_bags[MAXBAGS*4];


unsigned char* open_out();
FILE	*ofp;

main(argc, argv) 
     int argc; char *argv[]; 
{ 
  FILE	*fp;
  unsigned char 	*p;
  int	i, ss = 0;
  char	*fni,*fno,fnc = 0;
  
  ofp = stderr;
  
  for (i = 1;i < argc;i++){
    if (*argv[i] == '-'){
      if (*(argv[i]+1) == 's')
	ss = 1;
      else
	error("unknown option");
    } else {
      switch (fnc){
      case 0:
	fni = argv[i];		
	break;
      case 1:
	fno = argv[i];
	break;	
      default:
	break;	
      }
      fnc++;
    }
  }
  if (fnc == 0 || fnc > 2)
    error("Mismatch");
  if (( fp = fopen(fni, "r")) == NULL)
    error("Can't open data file");
  if (fnc == 2){
    if (( ofp = fopen(fno, "w")) == NULL)
      error("Can't open out_file");
  } 
  
  initialize();
  read_data(fp);
  preprocess();
  open_in();
  for (;;){
    if ( n_open == 0 ){
      fprintf(ofp, "n_close=%d, open_in=%d\n", n_close, cell_ptr);
      error("No solution");
    }
    p = open_out();
    
    close_in(p);
    if (is_final()){
      
      if (ss)
	print_simple_solution();
      else
	print_solution();
      if( _max_val != MAXPOINT )	error("Crazy error about value");
      break;
    }
    source = n_close;
    move();
  }
}
  
initialize()
{
  int	i, j;
  
  size_x = 0;	size_y = 0;
  for ( i = 0; i < MAXSIZE; i++ ){
    for ( j = 0; j < MAXSIZE; j++ ){
      bags[i][j] = 0;
      maze[i][j] = BLOCK;
    }
  }
  source = 0;
  step = 0;
  for ( i = 0; i < HASH; i++)
    hash_head[i] = NULL;
  init_open_hash();
}

int is_final()
{
  int i;
  
  for ( i = 0; i < n_bags; i++)
    if ( maze[baggage[i].x][baggage[i].y] != GOAL )
      return(0);

  return(1);
}
