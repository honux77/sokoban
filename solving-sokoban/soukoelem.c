#include	<stdio.h>
#include	<stdlib.h>
#include	"soukodef.h"
#include	"extern.h"

clear_bags()
{ 
  int i, j;
  
  for (i = 0; i < size_x; i++)
    for (j = 0; j < size_y; j++)
      bags[i][j] = SPACE;
}

set_bags(){
  int	i;
  
  clear_bags();
  for (i = 0; i < n_bags; i++)
    bags[baggage[i].x][baggage[i].y] = BAGGAGE;
}
