#include	<stdio.h>
#include	<stdlib.h>
#include	"soukodef.h"
#include	"extern.h"

extern	int	cell_ptr;

read_data(fp)
     FILE *fp;
{
  int n_man;
  int i, j;
  int c;

  i = 0; j = 0;
  n_man = 0;
  n_bags = 0;
  n_goal = 0;
  c = fgetc(fp);
  
  while( c != EOF ){
    switch (c){
    case BLOCKCHR:
      maze[i][j] = BLOCK;
      break;
    case GOALCHR:
      maze[i][j] = GOAL;
      goal[n_goal].x = i;
      goal[n_goal].y = j;
      n_goal++;
      break;
    case BAGCHR:
      maze[i][j] = SPACE;
      baggage[n_bags].x = i;
      baggage[n_bags].y = j;
      n_bags++;
      break;
    case GOAL_INCHR:
      maze[i][j] = GOAL;
      baggage[n_bags].x = i;
      baggage[n_bags].y = j;
      n_bags++;
      goal[n_goal].x = i;
      goal[n_goal].y = j;
      n_goal++;
      break;
    case MANCHR:
      maze[i][j] = SPACE;
      man_x = i; man_y = j;
      n_man++;
      break;
    case SPACECHR:
      maze[i][j] = SPACE;
      break;
    case MAN_GOALCHR:
      maze[i][j] = GOAL;
      goal[n_goal].x = i;
      goal[n_goal].y = j;
      n_goal++;
      man_x =i; man_y = j;
      n_man++;
      break;
    case '\n':
      break;
    default:
      error("Wrong character");
      break;
    }
    i++;
    if ( c == '\n' ){
      if( size_x < i-1 )
	size_x = i-1;
      i = 0; j++;
    }
    c = fgetc(fp);
  }
  size_y = j;
  data_length = DATAHEAD + n_bags*2;

  if ( n_man > 1 )		error("Must be one player");
  if ( n_man == 0 )		error("No player");
  if ( n_bags > MAXBAGS )	error("Too many bags");
  if ( n_bags != n_goal )	error("Mismatch between bags and goal");
  
  set_bags();
}

print_simple_solution()
{
  int	cur,pre,src;
  int 	tmp,tmp2;
  int	i, j;
  int	pcn = 0;
  unsigned char 	*curp,*prep;
  unsigned char	*tcp,*tpp;
  int	sol_order[MAXANSWER];
  
  cur = n_close-1;
  i = 0;
  do {
    sol_order[i++] = cur;
    curp = close_p[cur];
    src = *(curp+4)*65536+*(curp+5)*256+*(curp+6)-1;
    cur = src;
  }while (src != -1);
  pcn = i-1;
  
  cur = sol_order[--i];
  curp = close_p[cur];
  print_buffer(curp);
  do {
    prep = curp;
    cur = sol_order[--i];
    curp = close_p[cur];
    fprintf(ofp,"%d:",cur);
    
    fprintf(ofp,"man(%d,%d):",*curp,*(curp+1));
    fprintf(ofp,"stp(%d):",*(curp+2)*256+*(curp+3));
    fprintf(ofp,"push(%d):",pcn-i);
    fprintf(ofp,"src(%d):",*(curp+4)*65536+*(curp+5)*256+*(curp+6)-1);
    
    for (j = 0; j < n_bags; j++){
      tcp = curp+DATAHEAD+j*2;
      tpp = prep+DATAHEAD+j*2;
      if (*tcp != *tpp || *(tcp+1) != *(tpp+1))
	fprintf(ofp,"bag%d (%d,%d)->(%d,%d):",j,*tpp,*(tpp+1),*tcp,*(tcp+1));
      tcp += 2;
      tpp += 2;
    }
    fprintf(ofp,"\n");
  } while (i > 0);
  print_buffer(curp);
  fprintf(ofp, "n_close=%d\n",n_close);
  fprintf(ofp, "open_in=%d\n",cell_ptr);
  fprintf(ofp, "push=%d\n",pcn);
  fprintf(ofp, "total steps=%d\n",step);
}

print_solution()
{
  int	cur,pre;
  int	i, j, pcn = 0;
  unsigned char 	*curp,*prep;
  unsigned char	*tcp,*tpp;
  int	step,src;
  int     sol_order[MAXANSWER];
  
  fprintf(ofp,"*** SOLUTION ***\n");
  cur = n_close-1;
  i = 0;
  do {
    sol_order[i++] = cur;
    curp = close_p[cur];
    src = *(curp+4)*65536+*(curp+5)*256+*(curp+6)-1;
    cur = src;
  } while ( src != -1);
  pcn = i-1;
  
  do {
    cur = sol_order[--i];
    curp = close_p[cur];
    man_x = *curp;
    man_y = *(curp+1);
    step = *(curp+2)*256+*(curp+3);
    src = *(curp+4)*65536+*(curp+5)*256+*(curp+6)-1;
    curp += DATAHEAD;
    
    for (j = 0; j < n_bags; j++){
      baggage[j].x = *curp++;
      baggage[j].y = *curp++;
    }
    fprintf(ofp,"%d:step(%d):push(%d):src(%d)\n",cur,step,pcn-i,src);
    set_bags();
    print_out_file();
  }while (i > 0);
  fprintf(ofp, "n_close=%d\n",n_close);
  fprintf(ofp, "open_in=%d\n",cell_ptr);
  fprintf(ofp, "push=%d\n",pcn);
  fprintf(ofp, "total steps=%d\n",step);
}

error(s)
char	 *s;
{
  fprintf(ofp,"%s\n",s);
  exit(1);
}
 
print_out_file()
{
  int i, j;

  for ( j = 0; j < size_y; j++){
    for ( i = 0; i < size_x; i++)
      switch(maze[i][j]) {
      case SPACE:
      case AVOID:
	switch(bags[i][j]){
	case BAGGAGE:
	  fprintf(ofp,BAGGRP);
	  break;
	default:
	  if (i == man_x && j == man_y) fprintf(ofp,MANGRP);
	  else  fprintf(ofp,SPACEGRP);
	  break;
	}
	break;
      case MAN:
	fprintf(ofp,MANGRP);
	break;
      case GOAL:
	if (bags[i][j] == BAGGAGE)
	  fprintf(ofp,GOAL_INGRP);
	else if (i == man_x && j == man_y)
	  fprintf(ofp,MAN_GOALGRP);
	else 
	  fprintf(ofp,GOALGRP);
	break;
      case BLOCK:
	fprintf(ofp,BLOCKGRP);
	break;
      default:
	fprintf(ofp,"%d",maze[i][j]);
	break;
      }
    fprintf(ofp,"\n");
  }
}
 
print_buffer(p)
unsigned char   *p;
{
  int     tmp;
  int     j;
  
  fprintf(ofp, "man(%d,%d):",*p++,*p++);
  tmp = (*p++)*256;
  tmp += *p++;
  fprintf(ofp, "stp(%d):",tmp);
  tmp = (*p++)*65536;
  tmp += (*p++)*256;
  tmp += *p++;
  fprintf(ofp, "src(%d):",tmp-1);
  for (j = 0; j < n_bags; j++){
    fprintf(ofp, "bag%d (%d,%d):",j,*p,*(p+1));
    p+=2;
  }
  fprintf(ofp, "\n");
}
