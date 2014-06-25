#include	"soukodef.h"
#include	"extern.h"

int p_space[MAXSIZE][MAXSIZE];
struct LOCATION stopper[MAXBAGS];
int n_stopper;
int p_cnt;

int p_temp[3][MAXSIZE][MAXSIZE];
int p_cnt_temp[3];
int n_search;
int around;

int	temp[MAXSIZE][MAXSIZE];

init_stopper(){ 
  int i, j;
  
  n_stopper = 0;
  for ( i = 0; i < size_x; i++)
    for ( j = 0; j < size_y; j++)
      p_space[i][j] = 0;
}

int check(x,y,v) 
     int x, y, v;
{ 
  
  return( four_elems(x, y, v)||
	 fixed_elems(x,y)||
	 check_wall_cap());
}

int four_elems(x, y, v)
     int	x, y, v;
{
  int	i, j, k;
  int	bl_cnt, bg_cnt, g_cnt;
  
  for ( k = 0; k < 2; k++){
    i = (v+3+k)%4;
    bl_cnt = 0;
    bg_cnt = 0;
    g_cnt = 0;
    if ( maze[x][y] == GOAL )
      g_cnt++;
    bg_cnt++;
    for (j = i*2;j < i*2+3; j++){
      if (maze[x+wx[j]][y+wy[j]] == BLOCK) bl_cnt++; 
      if (bags[x+wx[j]][y+wy[j]] == BAGGAGE) bg_cnt++;
      if (maze[x+wx[j]][y+wy[j]] == GOAL) g_cnt++;
    }
    if ((bl_cnt+bg_cnt)== 4 && g_cnt != bg_cnt){
/*kyakka("********** 4 tugumi *************");*/
      return(1);
    }
  }
  return(0);
}

blind_array(x, y, v)
     int	x, y, v;
{
  int	i;
  int	cnt, vec, tx, ty, vwx;
  cnt = 0;
  n_search = 0;
  vec = ((v+3)%4)*2;
  for ( i = 0; i < 5; i++){
    tx = x+wx[(vec+i+7)%8];
    ty = y+wy[(vec+i+7)%8];
    if (i == 0 || i > 0 
	&& (maze[tx][ty] == BLOCK || bags[tx][ty] == BAGGAGE)){
      tx = x+wx[(vec+i)%8];
      ty = y+wy[(vec+i)%8];
      if ( bags[tx][ty] != BAGGAGE && maze[tx][ty] != BLOCK ){ 
	stopper[0].x = x;
	stopper[0].y = y;
	if ( closure(tx,ty,v)){
/*kyakka("%%%%%%%%%%% fukuro %%%%%%%%%%%%%%");*/
	  return(1); 
	}
      }
    }
  }
  return(0);
}



int closure(x,y,v) 
int x, y, v;
{ 
  int i, j, k, brk, tx, ty, b, bad, cnt, side;
  
  cnt = 0;
  p_cnt = 0;
  bad = 0;
  set_bags();
  paint(man_x,man_y);
  init_stopper();
  paint2(x,y);
  if ( p_cnt == 0 ) return(0);
  find_stopper();

#if 0
  if ( n_stopper == 0 ) return(0);
#endif

  /* by K.Takahashi */
  if ( n_stopper == 0 || n_stopper > n_bags / 2) return(0);

  if ( n_search != 0 ){
    for ( k = 0; k < n_search; k++){
      if ( p_cnt_temp[k] == p_cnt ){
	brk = 0;
	for( i = 0; i < size_x; i++){
	  for( j = 0; j < size_y; j++)
	    if( p_space[i][j] != p_temp[k][i][j] ){
	      brk = 1;
	      break;
	    }
	  if (brk) break;
	}
	if ( !brk )
	  return(0);
      }
    }
  }

  for( i = 0; i < size_x; i++)
    for( j = 0; j < size_y; j++)
      p_temp[n_search][i][j] = p_space[i][j];

  p_cnt_temp[n_search] = p_cnt;
  n_search++;

  for ( i = 0; i < size_x; i++)
    for ( j = 0; j < size_y; j++)
      if ( maze[i][j] == GOAL && p_space[i][j] == SHUT ) 
	return(0);

  if ( n_bags == n_stopper ) return(0);
  if ( is_cave(p_cnt) ) return(0);

  return(1);
}

paint(sx,sy) 
int sx, sy;
{ 
  static unsigned char	buf[MAXSIZE*MAXSIZE*2];
  
  int	bh = 0, bt = 2;
  int 	i, x, y, tx, ty;
  
  bags[sx][sy] = CAN_MOVE;
  buf[0] = sx; buf[1] = sy;
  while (bh != bt){
    x = buf[bh++];
    y = buf[bh++];
    for (i = 0; i < 4; i++){
      tx = x+vx[i];
      ty = y+vy[i];
      if (maze[tx][ty] != BLOCK &&
	  bags[tx][ty] == SPACE){
	bags[tx][ty] = CAN_MOVE;
	buf[bt++] = tx;
	buf[bt++] = ty;
      }
    }
  }
}

paint2(x,y) 
int x, y;
{ 
  int i, brk=0;
  
  if ( maze[x][y] == BLOCK || bags[x][y] == CAN_MOVE 
      || p_space[x][y] == SHUT || bags[x][y] == BAGGAGE )
    return;
  p_space[x][y] = SHUT;
  p_cnt++;
  for ( i = 0; i < 4; i++)
    paint2(x+vx[i],y+vy[i]);
}

find_stopper()
{ 
  int i, j, brk, k, brk2;

  around = 0;

  for( i = 0; i < n_bags; i++ )
    for( j = 0; j < 4; j++ )
      if( p_space[baggage[i].x+vx[j]][baggage[i].y+vy[j]] == SHUT ){
	stopper[n_stopper] = baggage[i];
	n_stopper++;
	around++;
	break;
      }
  for( i = 0; i < n_bags; i++ )
    for( j = 1; j < 8; j+=2 )
      if( p_space[baggage[i].x+wx[j]][baggage[i].y+wy[j]] == SHUT ){
	brk2 = 0;
	for( k = 0; k < n_stopper; k++ )
	  if( baggage[i].x == stopper[k].x 
	     && baggage[i].y == stopper[k].y )
	    brk2 = 1;
	if( !brk2 ){
	  stopper[n_stopper] = baggage[i];
	  n_stopper++;
	  around++;
	  break;
	}
      }

  brk = 0;
  for( i = 0; i < n_stopper; i++ )
    if( maze[stopper[i].x][stopper[i].y] == GOAL )
      brk++;
  if( n_stopper == brk ){
    n_stopper = 0;
    return;
  }
  /*find_more();*/
}

find_more()
{ 
  int i, j, k, tx, ty, brk, mb;
  struct BAGINFO mb_sub[MAXBAGS];
  set_bags();
  paint(man_x,man_y);
  mb = find_movable_bags();
  for( i = 0; i < mb; i++ )
    mb_sub[i] = movable_bags[i];
  /*subete no un-movable_bags wo nokosu version*/
  for( i = 0; i < n_bags; i++ ){
    brk = 0;
    for( j = 0; j < mb; j++ ){
      if( i == mb_sub[j].n ){
	brk = 1;
	break;
      }
    }
    for( j = 0; j < n_stopper; j++ ){
      if( baggage[i].x == stopper[j].x
	 && baggage[i].y == stopper[j].y ){
	brk = 1;
	break;
      }
    }
    if( brk == 0 ){
      stopper[n_stopper] = baggage[i];
      n_stopper++;
    }
  }
  /**/
#if 0
  /* nimotsu ni sessiteiru nimotu de un-movable wo nokosu version */
  for( i = 0; i < n_stopper; i++ ){
    for( j = 0; j < 8; j++ ){
      tx = stopper[i].x+wx[j];
      ty = stopper[i].y+wy[j];
      brk = 0;
      if( bags[tx][ty] == BAGGAGE ){
	for( k = 0; k < n_stopper; k++ ){
	  if( tx == stopper[k].x && ty == stopper[k].y )
	    brk = 1;
	}
	for( k = 0; k < mb; k++ ){
	  if( tx == baggage[mb_sub[k].n].x && ty == baggage[mb_sub[k].n].y )
	    brk = 1;
	}
	if ( brk == 0 ){
	  stopper[n_stopper].x = tx;
	  stopper[n_stopper].y = ty;
	  n_stopper++;
	}
      }
    }
  }
#endif
}

int find_movable_bags()
{ 
  int a,b;
  int i, j, cnt = 0, tx, ty, tmp;

  for ( i = 0; i < n_bags; i++ ){
	  tx = baggage[i].x;
	  ty = baggage[i].y;
	  for ( j = 0; j < 4; j++ ){
	    if ( bags[tx-vx[j]][ty-vy[j]] == CAN_MOVE 
		&& maze[tx+vx[j]][ty+vy[j]] < AVOID 
		&& bags[tx+vx[j]][ty+vy[j]] != BAGGAGE ) {
	      tmp = bags[tx+vx[j]][ty+vy[j]];
	      bags[tx+vx[j]][ty+vy[j]] = BAGGAGE;
	      bags[tx][ty] = CAN_MOVE;
	      baggage[i].x += vx[j];
	      baggage[i].y += vy[j];
	      if( !check(tx+vx[j],ty+vy[j],j)){
		movable_bags[cnt].n = i;
		movable_bags[cnt].v = j;
		movable_bags[cnt].step = distance[tx-vx[j]][ty-vy[j]]+1; 
		cnt++;
	      }
	      bags[tx+vx[j]][ty+vy[j]] = tmp;
	      bags[tx][ty] = BAGGAGE;
	      baggage[i].x = tx;
	      baggage[i].y = ty;
	    }
	  }
  }
  return(cnt);
}

check_wall_cap()
{
  static temp[MAXWALL];
  int	i,tx,ty;
  if (n_wall==0) return(0);
  for (i = 1;i <= n_wall; i++)
    temp[i] = 0;
  for (i = 0; i < n_bags; i++){
    tx = baggage[i].x;
    ty = baggage[i].y;
    temp[wallx[tx][ty]]++;
    temp[wally[tx][ty]]++;
    temp[wallu[tx][ty]]++;
  }
  for (i = 1;i <= n_wall; i++)
    if (temp[i] > wallcap[i]){
/*kyakka(" ++++++++++ wall capa +++++++++++");*/
      return(1);
    }
  return(0);
}

fixed_elems(x, y)
int	x, y;
{
  int	i, j, n = 0,nn;
  int	tx, ty;
  int	brk,get;
  
  struct {int	x,y,f;} temp_baggage[MAXBAGS];
  
  /*	set_bags();*/
  for (i = 0; i < size_x; i++)
    for (j = 0; j < size_y; j++)
      temp[i][j] = 0;
  fixed_elems_sub(x, y);
  for (i = 0; i < n_bags; i++)
    if (temp[baggage[i].x][baggage[i].y] == 1){
      temp_baggage[n].x = baggage[i].x;
      temp_baggage[n].y = baggage[i].y;
      temp_baggage[n].f = 1;
      n++;
    }
  nn = n;
  for (i = 0; i < size_x; i++)
    for (j = 0; j < size_y; j++){
      temp[i][j] = maze[i][j];
      if (bags[i][j] == BAGGAGE)
	temp[i][j] = BLOCK;
    }
  brk = 0;
  do {
    get = 0;
    for (i = 0; i < nn; i++){
      if (temp_baggage[i].f == 1){
	tx = temp_baggage[i].x;
	ty = temp_baggage[i].y;
	if (!(temp[tx-1][ty] == BLOCK || temp[tx+1][ty] == BLOCK ||
	      temp[tx-1][ty] == AVOID && temp[tx+1][ty] == AVOID)
	    ||
	    !(temp[tx][ty-1] == BLOCK || temp[tx][ty+1] == BLOCK ||
	      temp[tx][ty-1] == AVOID && temp[tx][ty+1] == AVOID)){
	  temp_baggage[i].f = 0;
	  n--;
	  get = 1;
	  temp[tx][ty] = maze[tx][ty];
	  continue;
	}
      }
    }
  } while (get == 1);
  if (n == 0) return(0);
  for (i = 0; i < nn; i++)
    if (temp_baggage[i].f == 1 &&
	maze[temp_baggage[i].x][temp_baggage[i].y] != GOAL){
      /*kyakka(" ??????????? fixed elems ???????????");*/
      return(1);
    }
  return(0);
}

fixed_elems_sub(x, y)
int	x, y;
{
  int 	i;
  
  if (temp[x][y] == 1 || bags[x][y] != BAGGAGE) return;
  temp[x][y] = 1;
  for (i = 0; i < 4; i++)
    fixed_elems_sub(x+vx[i], y+vy[i]);
}
