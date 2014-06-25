#include	<stdlib.h>
#include	"soukodef.h"
#include	"extern.h"

extern int	cell_ptr;

int	buffer_p = 0;
int	maxpoint = 0;
struct	CELL open_cell[HASHSIZE];
struct	CELL *open_hash_head[MAXPOINT+1];
int	temp_dist[MAXBAGS][MAXSIZE][MAXSIZE][4];
int	goal_p[MAXBAGS][MAXSIZE][MAXSIZE][4],g_temp[MAXSIZE][MAXSIZE][4];
int	o_goal[MAXSIZE][MAXSIZE];
int	o_goal_table[MAXBAGS][MAXBAGS],n_o_goal_table[MAXBAGS];
int	max_p = 0;
int	can_move[MAXSIZE][MAXSIZE];
int	max_diff[MAXBAGS];
int 	temp_map[MAXSIZE][MAXSIZE];

/*
 *	`calc_max_value()' is made by Y.Murase.
 *	According to the thesis, the end position has the highest value.
 *	The value depends on `max_p' and the number of the order of goals.
 *	`max_p' dependes on the longest distance for a bag from a goal to
 *	a space.
 */
calc_max_value()
{
  int i, tx, ty;

  max_val = 0;

  for (i = 0; i < n_goal; i++){
    tx = goal[i].x; ty = goal[i].y;
    max_val = max_val + max_p + max_p * o_goal[tx][ty];
  }
}

init_open_hash()
{
	int	i;

	for (i = 0; i <= MAXPOINT; i++)
		open_hash_head[i] = NULL;
}

open_in()
{
	int	point, h, temp;
	struct CELL *cp,*pp;
	unsigned char	*p;

	/* by K.Takahashi */
	if (cell_ptr == HASHSIZE) error("Full of open_cell");

	point = value();
	if (point > maxpoint) maxpoint = point;

	cp  = open_hash_head[point];
	if (cp != NULL) {
		p = cp->d;
		p += 2;
		temp = *p++*256;
		temp += *p;
	}
	if (cp == NULL || step <= temp){
		open_cell[cell_ptr].next = open_hash_head[point];
		open_cell[cell_ptr].d = &buffer[buffer_p];
		open_hash_head[point] = &open_cell[cell_ptr];
		open_in_sub();
		cell_ptr++;
		n_open++;
	} else {
		pp = open_hash_head[point];
		while (1){
			cp = pp->next;
			if (cp != NULL) {
				p = cp->d;
				p += 2;
				temp = *p++*256;
				temp += *p;
			}
        		if (cp == NULL || step <= temp){
               		 	open_cell[cell_ptr].next = cp;
                		open_cell[cell_ptr].d = &buffer[buffer_p];
                		pp->next = &open_cell[cell_ptr];
                		open_in_sub();
                		cell_ptr++;
                		n_open++;
				break;
			}
			pp = cp;
		}
	}
}

open_in_sub()
{
	int     i, temp;

	if (buffer_p+data_length >= BUFFERSIZE) error("Full of OPEN");
	buffer[buffer_p++] = man_x;
	buffer[buffer_p++] = man_y;
	buffer[buffer_p++] = step / 256;
	buffer[buffer_p++] = step % 256;
	temp = source / 65536;
	buffer[buffer_p++] = temp;
	temp = source - temp*65536;
	buffer[buffer_p++] = temp / 256;
	buffer[buffer_p++] = temp % 256;
	for (i = 0; i < n_bags; i++){
		buffer[buffer_p++] = baggage[i].x;
		buffer[buffer_p++] = baggage[i].y;
	}
}

unsigned char* open_out()
{
	int	i, h;
	struct CELL	*cp,*pp;
	unsigned char	*p;

	i = maxpoint;
	while (open_hash_head[i] == NULL){
		i--;
		if (i < 0) error("OPEN EMPTY");
	}
	p = open_hash_head[i]->d;
	open_out_sub(p);
	open_hash_head[i] = open_hash_head[i]->next;
	while (open_hash_head[i] == NULL)
		i--;
	maxpoint = i;
	n_open--;
	return(p);
}

open_out_sub(p)
unsigned char	*p;
{
	int     i;

	man_x = *p++;
	man_y = *p++;
	step  = *p++*256;
	step += *p++;
	source = *p++*65536;
	source += *p++*256;
	source += *p++;
	for (i = 0; i < n_bags; i++){
	       baggage[i].x = *p++;
	       baggage[i].y = *p++;
	}
	set_bags();
}

is_in_open()
{
	int	point;
	struct CELL * cp;

	point = value();
	cp = open_hash_head[point];
	while (cp != NULL){
		if (compare_buffer(cp->d) == 1)
			return(1);
		cp = cp -> next;
	}
	return(0);
}

compare_buffer(p)
unsigned char	*p;
{
	unsigned char	*bp;
	int	i, brk = 0;

	bp = p+DATAHEAD;
	for (i = 0;i < n_bags; i++){
		if (bags[*bp][*(bp+1)] != BAGGAGE) {
			brk = 1;
			break;
		}
		bp+=2;
	}
	set_bags();
	if (!brk){
        	paint(man_x,man_y);
		if (bags[*p][*(p+1)] ==  CAN_MOVE || *p == man_x && *(p+1) == man_y)
			return(1);
	}
	return(0);
}

int value()
{
	int	val, val_new;
	int	g_bagsx=0,g_bagsy=0;
	int	temp;
	int	i, j, k, tx,ty,v,bx,by;
	int 	index;
	int	max,tmax;
	int	diff = 0;
	int	cnt = 0;
	double	tmp_double;

	index = 0;
	for (i = 0; i < n_goal; i++){
		tx = goal[i].x; ty = goal[i].y;
		if (bags[tx][ty] != BAGGAGE)
			if (o_goal[tx][ty] > index )
				index = o_goal[tx][ty];
	}
	val = 0;
	for (i = 0; i < n_bags; i++){
		bx = baggage[i].x;
		by = baggage[i].y;

		if (o_goal[bx][by] >= index){
			val += max_p+max_p*(o_goal[bx][by]);
		} else {	
			for (tx = 0; tx < size_x; tx++)
	        	    for (ty = 0; ty < size_y; ty++)
				can_move[tx][ty] = SPACE;
			can_move[bx][by] = BAGGAGE;
			paint_can_move(man_x,man_y);
                	for (v = 0; v < 4; v++)
			    if (can_move[bx+vx[v]][by+vy[v]] == CAN_MOVE)
				break;
			cnt++;
			tmax = 0;
			max = 0;
			temp = 0;
			for (j = 0; j < n_o_goal_table[index]; j++){
			    k = o_goal_table[index][j];
			    if (bags[goal[k].x][goal[k].y] == BAGGAGE)
				continue;
			    temp = goal_p[k][bx][by][v];
			    if (temp > tmax)
				tmax = temp;
			}
			val += tmax;
			if (max - tmax > diff)
				diff = (max - tmax);
		}
	}
	val += diff*0;

	if (val < 0) val = 0;

	/* if (val > MAXPOINT) val = MAXPOINT; */
/* by Y.Murase */
	if( val > max_val )	error("Crazy error in value()");

	tmp_double = ((double)val*MAXPOINT / (double)max_val);
	val_new = (int)tmp_double;

	if( val_new > _max_val )	  _max_val = val_new;
/* */
	return (val_new);
}

/* invoked only by preprocess() */
get_goal_p()
{

	int	i,j,x, y,v,tx,ty,min[MAXBAGS],max = 0;
	int	max_dist = 0;
	for (i = 0; i < n_goal; i++) 
	    for (y = 0; y < size_y; y++)
		for (x = 0;x < size_x; x++)
		    for (v = 0; v < 4; v++)
		        goal_p[i][x][y][v] = 0;

	for (i = 0; i < n_goal; i++){
		x = goal[i].x;
		y = goal[i].y;
		for (ty = 0; ty < size_y; ty++)
		    for (tx = 0;tx < size_x; tx++)
			for (v = 0; v < 4; v++)
			    g_temp[tx][ty][v] = -1;
		for (v = 0; v < 4; v++)
			get_goal_p_sub(x,y,0,v);
		for (tx = 0; tx < size_x; tx++){
		    for (ty = 0; ty < size_y; ty++)
			for (v = 0; v < 4; v++){
				temp_dist[i][tx][ty][v] = g_temp[tx][ty][v];	
				if (temp_dist[i][tx][ty][v] > max_dist)
					max_dist = temp_dist[i][tx][ty][v];
			}
		}

	}
	max_p = (max_dist+1)*(max_dist+1);

	for (i = 0; i < n_goal; i++)
	    for (ty = 0; ty < size_y; ty++)
		for (tx = 0; tx < size_x; tx++){
		    for (v = 0; v < 4; v++)
			if (temp_dist[i][tx][ty][v] != -1){
				goal_p[i][tx][ty][v] = (max_dist-temp_dist[i][tx][ty][v]+1)*(max_dist-temp_dist[i][tx][ty][v]+1);
			      }
	}

	find_goal_order();
}

find_goal_order()
{
	int	i, j, x, y, v;
	int	tx,ty;
	int	odr=0,g_cnt,cnt,brk,mz1,mz2;
	int	to_goal[MAXSIZE][MAXSIZE];
	int	failf = 0;
	int	sub_bags[MAXSIZE][MAXSIZE];

	for (y = 0; y < size_y;y++)
		for (x = 0;x < size_x; x++){
			o_goal[x][y] = 0;
			to_goal[x][y] = 0;
			sub_bags[x][y] = bags[x][y];
	}
	for (i = 0;i < n_goal; i++)
		if (bags[goal[i].x][goal[i].y] == BAGGAGE)
			bags[goal[i].x][goal[i].y] = SPACE;

	odr = 1;
	g_cnt = 0;
	while (g_cnt < n_goal){
		cnt = 0;
		for (i = 0; i < n_goal; i++){
			for (j = 0; j < n_goal; j++){
				tx = goal[j].x;
				ty = goal[j].y;
				maze[tx][ty] = (to_goal[tx][ty] == 2) ? GOAL : BLOCK;
			}
			tx = goal[i].x;
			ty = goal[i].y;

			if (to_goal[tx][ty] != 2){
				maze[tx][ty] = GOAL;

				brk = 0;
				for (v = 0; v < 4; v++)
					if(is_to_start(tx,ty,v)){
						brk = 1;
						break;
					}
				if (brk) {
					o_goal[tx][ty] = odr;
					to_goal[tx][ty] = 1;
					o_goal_table[odr][cnt] = i;
					g_cnt++;
					cnt++;
				}
			}
		}
		if (cnt == 0){
			failf = 1;	
			break;
		}
		n_o_goal_table[odr] = cnt;
		for (y = 0; y < size_y;y++)
			for (x = 0;x < size_x; x++)
				if (to_goal[x][y] == 1)
					to_goal[x][y] = 2;
		odr++;
	}

	for (i = 0; i < n_goal; i++)
		maze[goal[i].x][goal[i].y] = GOAL; 
	for (i = 0; i < size_x; i++)
		for (j = 0; j < size_y; j++)
			bags[i][j] = sub_bags[i][j];
	


/*	if (failf == 0){
printf("1st Process succeed !\n");
		 return;
	}
*/
/*
printf("1st Process failed !\n");
*/
/* 2nd way */
/*	for (y = 0; y < size_y;y++)
		for (x = 0;x < size_x; x++){
			o_goal[x][y] = 0;
			to_goal[x][y] = 0;
	}*/
/*	odr = 1;
	g_cnt = 0;
	cnt = 0;*/

	do {
		cnt = 0;
		for (i = 0; i < n_goal; i++){
			tx = goal[i].x;
			ty = goal[i].y;
			if (to_goal[tx][ty] != 2){
				brk = 0;
				for (j = 0; j < 4; j++){
					mz1 = maze[tx+vx[j]][ty+vy[j]];
					mz2 = maze[tx+2*vx[j]][ty+2*vy[j]];
					if (((mz1 < BLOCK && mz1 != GOAL) || (mz1 == GOAL && to_goal[tx+vx[j]][ty+vy[j]] == 2)) &&
					 ((mz2 < BLOCK  && mz2 != GOAL) || (mz2 == GOAL && to_goal[tx+2*vx[j]][ty+2*vy[j]] == 2))){
						brk = 1;
						break;
					}
				}
				if (brk) {
					o_goal[tx][ty] = odr;		
					to_goal[tx][ty] = 1;
					o_goal_table[odr][cnt] = i;
					g_cnt++;
					cnt++;
				}
			}
		}
		n_o_goal_table[odr] = cnt;
		for (y = 0; y < size_y;y++)
			for (x = 0;x < size_x; x++)
				if (to_goal[x][y] == 1)
					to_goal[x][y] = 2;
		odr++;
	}while (cnt > 0);	

}

/*
 *	Just setting g_temp[]. Nothing else.
 */
get_goal_p_sub(sx,sy,sn,sv)
int	sx, sy, sn, sv;
{
	int	i,tx,ty;
	int	x, y, n, v;
	static	int  	buf[MAXSIZE*MAXSIZE*4*4];
	int	bh = 0, bt = 4;
	int	g_temp_length = MAXSIZE*MAXSIZE*4*4;

	int	cm[4];

/*	if (maze[x][y] == BLOCK || maze[x][y] == AVOID ||
	    g_temp[x][y][v] >= 0 && g_temp[x][y][v] <= n) 
		return;*/
	buf[0] = sx; buf[1] = sy; buf[2] = sn; buf[3] = sv;

	while(bh != bt){
		x = buf[bh++]; y = buf[bh++];
		n = buf[bh++]; v = buf[bh++];

		for (tx = 0; tx < size_x; tx++)
			for (ty = 0; ty < size_y; ty++)
				can_move[tx][ty] = SPACE;
		can_move[x][y] = BAGGAGE;	
		paint_can_move(x+vx[v],y+vy[v]);

/*for (ty = 0; ty < size_y; ty++){
  for (tx = 0; tx < size_x; tx++)
    printf("%d",can_move[tx][ty]);
  printf("\n");
}*/
		for (i = 0; i < 4; i++){
			tx = x+vx[i]; ty = y+vy[i];
			if (can_move[tx][ty] == CAN_MOVE){
				g_temp[x][y][i] = n;
				if (maze[tx][ty] != BLOCK &&
				maze[tx+vx[i]][ty+vy[i]] != BLOCK &&
				g_temp[tx][ty][i] < 0){
					buf[bt++] = tx;
					buf[bt++] = ty;
					buf[bt++] = n+1;
					buf[bt++] = i;
	/* by Y. Murase */
	g_temp[tx][ty][i] = n+1;
	if( bt > g_temp_length-4 )
	error("Crazy error in get_goal_p()");
	/* */				
				}
			}
		}
	}
}

is_to_start(sx,sy,sv)
int	sx, sy, sv;
{
        int     i,j,k,tx,ty;
        int     x, y, v;
        static  int     buf[MAXSIZE*MAXSIZE*3];
        int     bh = 0, bt = 3;
	int 	temp_maze[MAXSIZE][MAXSIZE][4];

        int     cm[4];

/*      if (maze[x][y] == BLOCK || maze[x][y] == AVOID ||
            g_temp[x][y][v] >= 0 && g_temp[x][y][v] <= n)
                return;*/

	for (i = 0; i < size_x; i++)
		for (j = 0; j < size_y; j++)
			for (k = 0; k < 4; k++)
			temp_maze[i][j][k] = 0;

        buf[0] = sx; buf[1] = sy; buf[2] = sv;

        while(bh != bt){
                x = buf[bh++]; y = buf[bh++]; v = buf[bh++];
		if (bags[x][y] == BAGGAGE)
			return(1);

                for (tx = 0; tx < size_x; tx++)
                        for (ty = 0; ty < size_y; ty++)
                                can_move[tx][ty] = SPACE;

                can_move[x][y] = BAGGAGE;
                paint_can_move(x+vx[v],y+vy[v]);

                for (i = 0; i < 4; i++){
                        tx = x+vx[i]; ty = y+vy[i];
                        if (can_move[tx][ty] == CAN_MOVE){
				temp_maze[x][y][i] = 1;
                                if (maze[tx][ty] != BLOCK &&
                                maze[tx+vx[i]][ty+vy[i]] != BLOCK &&
					temp_maze[tx][ty][i] != 1){
                                        buf[bt++] = tx;
                                        buf[bt++] = ty;
                                        buf[bt++] = i;
					/* by Y.Murase */
					temp_maze[tx][ty][i] = 1;
                                }
                        }
                }
        }
	return(0);
}

paint_can_move(sx,sy)
{
	static unsigned char buf[MAXSIZE*MAXSIZE*2];

	int	bh = 0,bt = 2;
	int	i, x, y, tx, ty;

if (maze[sx][sy] == BLOCK || can_move[sx][sy] != SPACE)
	return;
	can_move[sx][sy] = CAN_MOVE;
	buf[0] = sx; buf[1] = sy;
	while (bh != bt){
		x = buf[bh++];
		y = buf[bh++];
		for (i = 0; i < 4; i++){
			tx = x+vx[i];
			ty = y+vy[i];
			if (maze[tx][ty] != BLOCK &&
			    can_move[tx][ty] == SPACE){
			       can_move[tx][ty] = CAN_MOVE;
				buf[bt++] = tx;
				buf[bt++] = ty;
			}
		}
	}
}
		
close_in(p)
unsigned char	*p;
{
	int	h;

	/* by K.Takahashi */
	if (hp == HASHSIZE) error("Full of close_cell");

	h =hash_func();
	cell[hp].next = hash_head[h];
	cell[hp].d = p;
	hash_head[h] = &cell[hp];
	hp++;
	close_p[n_close] = p;
	n_close++;
}

int is_in_close()
{
	int         h;
	struct CELL *cp;

	h = hash_func();
	cp = hash_head[h];
	while (cp != NULL){
		if (compare_buffer(cp->d) == 1)
			return(1);
		cp = cp->next;
	}
	return(0);
}

int hash_func()
{
	int 	i;
	unsigned int	 res = 0;

	for (i = 0; i <  n_bags; i++)
		res += (baggage[i].x*(size_x+2)*3 ^ baggage[i].y*(size_y+3)*5);
		/*res += (baggage[i].x*(size_x+2)*3 ^ baggage[i].y);*/
	return(res % HASH);
}

find_all_avoid()
{
	int	x, y, i,brk, cnt = 0; 

	for (x = 0;x < size_x; x++)
 	    for (y = 0; y < size_y; y++){
		if (maze[x][y] != SPACE) continue;
		brk = 0;
		for (i = 0; i < n_goal; i++)
		    if (temp_dist[i][x][y][0] >= 0 ||
		        temp_dist[i][x][y][1] >= 0 ||
		    	temp_dist[i][x][y][2] >= 0 ||
		 	temp_dist[i][x][y][3] >= 0 ){
				brk = 1;
				break;
		    }	
		if (brk == 0){
			maze[x][y] = AVOID;
			cnt++;
		}
	    }
}
