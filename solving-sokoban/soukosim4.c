#include	"soukodef.h"
#include	"extern.h"

extern struct LOCATION stopper[MAXBAGS];
extern int n_stopper, around;
extern int p_space[MAXSIZE][MAXSIZE];
int depth = 0;

int is_cave(p_cnt) 
int p_cnt;
{ 
  int i, j, unreachable=0, mb, cnt;
  int sub_bags[MAXSIZE][MAXSIZE], bags_local[MAXSIZE][MAXSIZE];
  int sub_mx, sub_my;
  int sub_n_bags,sub_step;
  
  struct LOCATION sub_baggage[MAXBAGS], baggage_local[MAXBAGS];
  
  struct BAGINFO movable_bags_local[MAXBAGS*2];
  int p_space_local[MAXSIZE][MAXSIZE];
  int n_bags_local;
  int mx_local, my_local;
  char sim_buf[SIMBUFSIZE];
  int sim_bt, sim_bh, d_length;
  int number, tx, ty, tv, p_mx, p_my, p_step;
  int bp, is_in_s_b, brk, temp;
  int k, ttx, tty, cnt_s, cnt_f, cnt_r, ttemp, kind;
  int around_local, brk2, kind_temp;
  
  sim_bt = 0; sim_bh = 0;

/*store_values*/
  for( i = 0; i < size_x; i++)
    for( j = 0; j < size_y; j++ )
      sub_bags[i][j] = bags[i][j]; 
  for( i = 0; i < MAXBAGS; i++ )
    sub_baggage[i] = baggage[i];
  sub_mx = man_x;
  sub_my = man_y;
  sub_n_bags = n_bags;
  sub_step = step;

/*end store_values*/
  for( i = 0; i < size_x; i++)
    for( j = 0; j < size_y; j++)
      p_space_local[i][j] = p_space[i][j];
  unreachable = p_cnt;
  n_bags = n_stopper;
  for( i = 0; i < n_bags; i++ )
    baggage[i] = stopper[i];
  set_bags();
  d_length = n_bags*2+2;

  /*sim_buffer_in*/
  for(  i = 0; i < n_bags; i++){
    sim_buf[sim_bt++] = baggage[i].x;
    sim_buf[sim_bt++] = baggage[i].y;
  }
  sim_buf[sim_bt++] = man_x;
  sim_buf[sim_bt++] = man_y;
  /*end sim_buffer_in*/
  
  /* start main search */
  for(;;){
    if( sim_bt == sim_bh ){
      /*restore_values*/
      n_bags = sub_n_bags;
      for( i = 0; i < MAXBAGS; i++ )
	baggage[i] = sub_baggage[i];
      set_bags();
      man_x = sub_mx;
      man_y = sub_my;
      step = sub_step;
      /*end restore_values*/
      
      return(0);
    }
    else{

      /*sim_buffer_out*/
      clear_bags();
      for(  i = 0; i < n_bags; i++){
	baggage[i].x = sim_buf[sim_bh++];
	baggage[i].y = sim_buf[sim_bh++];
      }
      for(  i = 0; i < n_bags; i++)
	bags[baggage[i].x][baggage[i].y] = BAGGAGE;
      man_x = sim_buf[sim_bh++];
      man_y = sim_buf[sim_bh++];
      /*end sim_buffer_out*/

      set_bags();
      paint(man_x,man_y);

      /*sim_pre_move();*/
      mb = find_movable_bags();
      around_local = 0;
      for( i = 0; i < mb; i++){
	brk = 0;
	for( j = 0; j < 8; j++){
	  if( bags[baggage[movable_bags[i].n].x+wx[j]]
     		  [baggage[movable_bags[i].n].y+wy[j]]
	     == BAGGAGE ){
	    brk = 1;
	    break;
	  }
	}
	if( brk == 0 ){
	  brk2 = 0;
	  kind =  maze[baggage[movable_bags[i].n].x+wx[0]]
	    	      [baggage[movable_bags[i].n].y+wy[0]]; 
	  if( kind != BLOCK ) kind = SPACE;
	  for( j = 0; j < 9; j++){
	    if( maze[baggage[movable_bags[i].n].x+wx[j]]
	       	    [baggage[movable_bags[i].n].y+wy[j]] != BLOCK ) 
	      kind_temp = SPACE;
	    else kind_temp = BLOCK;
	    if( kind_temp != kind ){
	      kind = kind_temp;
	      brk2++;
	    }
	  }
	  if( brk2 < 3 ){ around_local++;
			}
	}
	if( brk != 0 || brk2 > 2 )
	  movable_bags_local[i-around_local] = movable_bags[i];
      }
      mb -= around_local;
      for( number = 0; number < mb; number++ ){
	is_in_s_b = 0;

	/*sim_move(i);*/
	tx = baggage[movable_bags_local[number].n].x;
	ty = baggage[movable_bags_local[number].n].y;
	tv = movable_bags_local[number].v;
	p_mx = man_x; p_my = man_y;
	temp = bags[tx+vx[tv]][ty+vy[tv]];
	bags[tx+vx[tv]][ty+vy[tv]] = BAGGAGE;
	baggage[movable_bags_local[number].n].x+=vx[tv];
	baggage[movable_bags_local[number].n].y+=vy[tv];
	bags[tx][ty] = SPACE;
	man_x = tx; man_y = ty;

	/* store local values */
	for( i = 0; i < n_bags; i++)
	  baggage_local[i] = baggage[i]; 
	n_bags_local = n_bags;
	mx_local = man_x;
	my_local = man_y;
	/* end store local values */

	if( ! blind_array(tx+vx[tv],ty+vy[tv],tv)){

	  /* restore local values*/ 
	  n_bags = n_bags_local;
	  for( i = 0; i < n_bags; i++)
	    baggage[i] = baggage_local[i]; 
	  set_bags();
	  man_x = mx_local;
	  man_y = my_local;
	  /*end restore local values */

	  set_bags();
	  paint(man_x, man_y);

	  /*if( is_reachable() ) return(1)*/
	  cnt = 0;
	  for ( i = 0; i < size_x; i++)
	    for( j = 0; j < size_y; j++ )
	      if( p_space_local[i][j] == SHUT ){ 
		if( bags[i][j] != SPACE ) 
		  cnt++;
		else if( unreachable == 1 )
		  if( bags[i][j] == CAN_MOVE ) cnt++;  
	      }
	  if( cnt == unreachable ){

	    /*restore_values*/
	    n_bags = sub_n_bags;
	    for( i = 0; i < MAXBAGS; i++ )
	      baggage[i] = sub_baggage[i];
	    set_bags();
	    man_x = sub_mx;
	    man_y = sub_my;
	    step = sub_step;
	    /*end restore_values*/

	    return(1); 
	  }
	  /*end if( is_reachable() ) return(1)*/

	  /*make ( is_in_s_b )*/
	  for( j = 0; j < sim_bt/d_length; j++ ){
	    brk = 0;
	    bp = d_length*j;
	    for( i = 0; i < n_bags; i++){
	      if( bags[sim_buf[bp]][sim_buf[bp+1]] != BAGGAGE ){
		brk = 1;
		break;
	      }
	      bp+=2;
	      if( bp > SIMBUFSIZE ) error("Crazy error. by Y.Murase");
	    }
	    if (!brk){
	      set_bags();
	      paint(man_x, man_y);
	      if( bags[sim_buf[bp]][sim_buf[bp+1]] == CAN_MOVE
		 || sim_buf[bp] == man_x && sim_buf[bp+1] == man_y ){
		is_in_s_b = 1;
		break;
	      }
	    }
	  }
	  /*end make ( is_in_s_b )*/

	  if(!is_in_s_b ){

	    /* sim_buffer_in*/
	    if( sim_bt+d_length >= SIMBUFSIZE ) error("FULL of SIM BUFFER");
	    for(  i = 0; i < n_bags; i++){
	      sim_buf[sim_bt++] = baggage[i].x;
	      sim_buf[sim_bt++] = baggage[i].y;
	    }
	    sim_buf[sim_bt++] = man_x;
	    sim_buf[sim_bt++] = man_y;
	    /*end sim_buffer_in*/
	  }
	}
	else{

	  /*restore local values*/ 
	  n_bags = n_bags_local;
	  for( i = 0; i < n_bags; i++)
	    baggage[i] = baggage_local[i]; 
	  set_bags();
	  man_x = mx_local;
	  man_y = my_local;
	  /*end restore local values */
	}

	/*end sim_move(i);*/
	bags[tx+vx[tv]][ty+vy[tv]] = temp;
	baggage[movable_bags_local[number].n].x-=vx[tv];
	baggage[movable_bags_local[number].n].y-=vy[tv];
	bags[tx][ty] = BAGGAGE;
	man_x = p_mx; man_y = p_my;
	/*end sim_pre_move();*/

      } /*for( number = 0; number < mb; number++ )*/
    } /*else < if( sim_bt == sim_bh )>*/
  } /*for(;;)*/
}/*is_cave(p_cnt) */
