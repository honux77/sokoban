#include	<stdio.h>
#include	<stdlib.h>
#include	"soukodef.h"
#include	"extern.h"

move()
{ 
  int mb, i, j, number;
  struct {
    int n;
    int v;
    int step;
  }mb_sub[MAXBAGS*4];

  int tx, ty, tv, p_mx, p_my, p_step, temp;
  
  set_bags();
  paint(man_x,man_y);
  count_distance(man_x,man_y);
  mb = find_movable_bags();
  if ( mb == 0 ) return;

  /* by Y.Murase */
  if( mb > MAXBAGS*4 )
    error("Crazy error in move()");
  /* end by */

  for( j = 0; j < mb; j++ ){
    mb_sub[j].n = movable_bags[j].n;
    mb_sub[j].v = movable_bags[j].v;
    mb_sub[j].step = movable_bags[j].step;
  }

  for ( number = 0; number < mb; number++){
    tx = baggage[mb_sub[number].n].x;
    ty = baggage[mb_sub[number].n].y;
    tv = mb_sub[number].v;
    p_mx = man_x; p_my = man_y;
    temp = bags[tx+vx[tv]][ty+vy[tv]];
    bags[tx+vx[tv]][ty+vy[tv]] = BAGGAGE;
    baggage[mb_sub[number].n].x+=vx[tv];
    baggage[mb_sub[number].n].y+=vy[tv];
    bags[tx][ty] = CAN_MOVE;
    man_x = tx; man_y = ty;
    p_step = step;
    step += mb_sub[number].step;

    if (!is_in_close() && !is_in_open() 
	&& !blind_array(tx+vx[tv],ty+vy[tv], tv)){
      open_in();
    }

    step = p_step;		
    bags[tx+vx[tv]][ty+vy[tv]] = temp;
    baggage[mb_sub[number].n].x = tx;
    baggage[mb_sub[number].n].y = ty;
    bags[tx][ty] = BAGGAGE;
    man_x = p_mx; man_y = p_my;
    set_bags();
  }
}

distance_sub(x,y,n)
     int	x, y, n;
{
  int	i;
  
  if (maze[x][y] == BLOCK || bags[x][y] == BAGGAGE 
      || distance[x][y] >= 0 && distance[x][y] <= n)
    return;
  distance[x][y] = n;
  for (i = 0; i < 4; i++)
    distance_sub(x+vx[i],y+vy[i],n+1);
}

count_distance(sx,sy)
     int	sx, sy;
{
  static unsigned int buf[MAXSIZE*MAXSIZE*3];
  int	bh = 0, bt = 3;
  int	i, x, y, st, tx, ty;
  
  for(x = 0; x < size_x;x++)
    for (y = 0; y <size_y; y++)
      distance[x][y] = -1;
  
  buf[0] = sx; buf[1] = sy; buf[2] = 0;
  distance[sx][sy] = 0;
  while (bh != bt){
    x = buf[bh++];
    y = buf[bh++];
    st = buf[bh++]+1;
    for (i = 0; i < 4; i++){
      tx = x+vx[i];
      ty = y+vy[i];
      if (maze[tx][ty] != BLOCK && bags[tx][ty] != BAGGAGE &&
	  distance[tx][ty] == -1){
	distance[tx][ty] = st;
	buf[bt++] = tx;
	buf[bt++] = ty;
	buf[bt++] = st;
      }
    }
  }
}
