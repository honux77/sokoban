#include	<stdio.h>
#include	<stdlib.h>
#include	"soukodef.h"
#include	"extern.h"

FILE	*fp_oh,*fp_ch;
extern struct CELL *open_hash_head[MAXPOINT+1];
extern int 	maxpoint;
extern	int	cell_ptr;

print_out(s)
     char *s;
{
  int i, j;
  
  printf("%s\n",s);
  for ( j = 0; j < size_y; j++){
    for ( i = 0; i < size_x; i++)
      switch(maze[i][j]) {
      case SPACE:
	switch(bags[i][j]){
	case BAGGAGE:
	  printf(BAGGRP);
	  break;
	default:
	  if (i == man_x && j == man_y) printf(MANGRP);
	  else  printf(SPACEGRP);
	  break;
	}
	break;
      case MAN:
	printf(MANGRP);
	break;
      case GOAL:
	if (bags[i][j] == BAGGAGE)
	  printf(GOAL_INGRP);
	else if (i == man_x && j == man_y)
	  printf(MAN_GOALGRP);
	else 
	  printf(GOALGRP);
	break;
      case AVOID:
	if ( i == man_x && j == man_y )
	  printf(MANGRP);
	else
	  printf("%c",'8');
	break;
      case BLOCK:
	printf(BLOCKGRP);
	break;
      default:
	printf("%d",maze[i][j]);
	break;
      }
    printf("\n");
  }
  printf("\n");
}

print_out2(s)
     char *s;
{
  int i, j;

  printf("%s\n",s);
  for ( j = 0; j < size_y; j++ ){
    for ( i = 0; i < size_x; i++){
      if ( i == man_x && j == man_y ) printf("&");
      else printf("%d",bags[i][j]);
    }
    printf("\n");
  }
}

kyakka(s)
     char *s;
{
  printf("%s\n",s);
}

open_file()
{
  fp_oh = fopen("OPENHASH","w");
  fp_ch = fopen("CLOSEHASH","w");
}

print_open()
{
  static int	cnt = 0;
  int	i, l;
  struct CELL	*cp;
  if (cnt++ % 50) return;
  fprintf(fp_oh,"OPEN HASH %d (now %d),(total %d)\n",cnt,n_open,cell_ptr);
  for (i = maxpoint; i >= 0; i--){
    l = 0;
    if (open_hash_head[i] == NULL)
      continue;
    cp = open_hash_head[i];
    fprintf(fp_oh,"Hash[%d]->",i);
    while (cp != NULL){
      l++;
      cp = cp->next;	
    }
    fprintf(fp_oh,"%d\n",l);
  }
  fprintf(fp_oh,"\n");
}

print_close()
{
  int	i, l;
  struct CELL	*cp;
  
  fprintf(fp_ch,"CLOSE HASH %d\n",n_close);
  for (i = 0; i < HASH; i++){
    l = 0;
    if (hash_head[i] == NULL)
      continue;
    cp = hash_head[i];
    fprintf(fp_ch,"Hash[%d]->",i);
    while (cp != NULL){
      l++;
      cp = cp->next;	
    }
    fprintf(fp_ch,"%d\n",l);
  }
  fprintf(fp_ch,"\n");
}
