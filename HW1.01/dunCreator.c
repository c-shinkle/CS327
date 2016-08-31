#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  int i,j;
  char dungeon[24][80];
  for(i=0;i<24;i++){
    for(j=0;j<80;j++){
	  dungeon[i][j]=' ';
      }
    }
  for(i=0;i<80;i++){
      dungeon[0][i]='-';
      dungeon[23][i]='-';
    }  
  for(j=1;j<23;j++){
      dungeon[j][0]='|';
      dungeon[j][79]='|';
    }
  for(i=0;i<24;i++){
      for(j=0;j<80;j++){
	  printf("%c",dungeon[i][j]);
	}
      printf("\n");
    }
  printf("\n");
  return 0;
}


