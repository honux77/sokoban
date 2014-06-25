#include	<stdio.h>
#include	<stdlib.h>
#include	"soukodef.h"
#include	"extern.h"

char temp2[MAXSIZE][MAXSIZE]; /*used in find_wall_cap() & sub()*/

preprocess()
{
  find_corner();
  find_avoid_area();
  find_wall_cap();
  get_goal_p();
  calc_max_value(); /* by Y.Murase */
  find_all_avoid();
}

find_corner()
{
  int i, j;
  
  for ( i = 1; i < size_x-1; i++ )
    for ( j = 1; j < size_y-1; j++ )
      if (  maze[i][j] == 0
	  && !( maze[i-1][j] != BLOCK && maze[i+1][j] != BLOCK
	       || maze[i][j-1] != BLOCK && maze[i][j+1] != BLOCK))
	maze[i][j] = AVOID;
}


find_avoid_area()
{ 
  int i, j, k, l, s, t, sn, st, wall,brk;
  static int vx[2]={1,0},vy[2]={0,1};
  
  for ( j = 1; j < size_y-1; j++ ){
    for ( i = 1; i < size_x-1; i++ ){
      if ( maze[i][j] == AVOID ){
        for (k = 0; k < 2; k++){
          st = 1; sn = 0; brk = 0; wall=1;
          while(brk==0){
	    switch (maze[i+vx[k]*st][j+vy[k]*st]){
	    case SPACE:sn++;st++;
	      break;
	    case GOAL:
	    case BLOCK:brk = 1;break;
	    case AVOID:if (sn > 0) {
	      for (l=1;l<=sn;l++){
		if (maze[i+vx[k]*l+vx[1-k]][j+vy[k]*l+vy[1-k]] < BLOCK
		    && maze[i+vx[k]*l-vx[1-k]][j+vy[k]*l-vy[1-k]] < BLOCK)
		  wall=0;
	      }
	      if (wall)
		for (l=1;l<=sn;l++)
		  maze[i+vx[k]*l][j+vy[k]*l]=AVOID;
	    }
	      brk = 1;
	      break;
	    default:break;
	    }
          }
        }
      }
    }
  }
}

find_wall_cap()
{
  int i, j, k, l, s, t, sn, st, spn,ii,jj;
  static int vx[2]={1,0},vy[2]={0,1};
  int wall1, wall2, brk;
  int	tempgoal;
  char	temp[MAXSIZE][MAXSIZE];
  
  for ( j = 0; j < size_y; j++ )
    for ( i = 0; i < size_x; i++ ){
      wallx[i][j] = 0;
      wally[i][j] = 0;
      wallu[i][j] = 0;
      if (maze[i][j] == BLOCK)
	temp[i][j] = BLOCK;
      else
	temp[i][j] = SPACE;
    }
  
  for ( j = 1; j < size_y-1; j++ )
    for ( i = 1; i < size_x-1; i++ )
      if (  temp[i][j] == 0 &&
	  !( temp[i-1][j] != BLOCK && temp[i+1][j] != BLOCK ||
	    temp[i][j-1] != BLOCK && temp[i][j+1] != BLOCK ))
	temp[i][j] = AVOID;
  
  for ( j = 1; j < size_y-1; j++ ){
    for ( i = 1; i < size_x-1; i++ )
      if ( temp[i][j] == AVOID ){
	for (k = 0; k < 2; k++){
	  st = 1; sn = 0; brk = 0; wall1=1; wall2=1;spn=0;
	  tempgoal = 0;
	  if (maze[i+vx[k]][j+vy[k]] == AVOID)
	    continue;
	  if (maze[i][j] == GOAL)
	    tempgoal++;
	  while (brk == 0){
	    switch (maze[i+vx[k]*st][j+vy[k]*st]){
	    case SPACE:
	      sn++;st++;
	      spn++;
	      break;
	    case GOAL:
	      if (temp[i+vx[k]*st][j+vy[k]*st] != AVOID){
		st++;
		tempgoal++;
		sn++;
	      }
	      break;
	    case AVOID:
	    case BLOCK:brk = 1;
	      break;
	    default:
	      break;
	    }
	    if (temp[i+vx[k]*st][j+vy[k]*st] == AVOID){
	      for (l=1;l<=sn;l++){
		if (maze[i+vx[k]*l+vx[1-k]][j+vy[k]*l+vy[1-k]] < 9)
		  wall1=0;
		if (maze[i+vx[k]*l-vx[1-k]][j+vy[k]*l-vy[1-k]] < 9)
		  wall2=0;
	      }
	      if (maze[i+vx[k]*st][j+vy[k]*st]== GOAL)
		tempgoal++;
	      if (wall1+wall2 >=1 && tempgoal > 0 && spn > 1){
		n_wall++;
		for (l=0;l<=sn+1;l++){
		  if (k == 0)
		    wallx[i+vx[k]*l][j+vy[k]*l] = n_wall;
		  else 
		    wally[i+vx[k]*l][j+vy[k]*l] = n_wall;
		}
		wallcap[n_wall] = tempgoal; 
	      }
	      brk = 1;
	    }
	  }
	}
      }
  }
  for ( j = 0; j < size_y; j++ )
    for ( i = 0; i < size_x; i++ ){
      if (maze[i][j]!=GOAL && temp[i][j] == AVOID){
	wallx[i][j] = 0;
	wally[i][j] = 0;
      }
      temp2[i][j] = (wallx[i][j]+wally[i][j] > 0)?1:0; 
    }
  
  for ( j = 0; j < size_y; j++ ){
    for ( i = 0; i < size_x; i++ ){
      if (temp[i][j] == AVOID && maze[i][j] == GOAL && wallx[i][j] > 0 &&
	  wally[i][j] > 0 && temp2[i][j] == 1){
	find_wall_cap_sub(i,j);
	tempgoal = 0;
	for ( jj = 0; jj < size_y; jj++ )
	  for ( ii = 0; ii < size_x; ii++ )
	    if (temp2[ii][jj] == 2 && maze[ii][jj] == GOAL)
	      tempgoal++;
	n_wall++;
	for ( jj = 0; jj < size_y; jj++ )
	  for ( ii = 0; ii < size_x; ii++ )
	    if (temp2[ii][jj] == 2)
	      wallu[ii][jj] =  n_wall;
	wallcap[n_wall] = tempgoal;
	for ( jj = 0; jj < size_y; jj++ )
	  for ( ii = 0; ii < size_x; ii++ )
	    if (temp2[ii][jj] == 2)
	      temp[ii][jj] = 3;
      }
    }
  }
}

find_wall_cap_sub(x, y)
     int	x, y;
{
  int	i;

  if (temp2[x][y] != 1)
    return;
  temp2[x][y] = 2;
  for (i = 0; i < 4; i++)
    find_wall_cap_sub(x+vx[i],y+vy[i]);
}
