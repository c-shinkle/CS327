#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct room{
    int x_pos;
    int y_pos;
    int x_size;
    int y_size;
};

char dungeon[24][80];

void create_outline(char dun[24][80]);

int main(int argc, char *argv[]){
  int i,j,k;
  
  for(i=0;i<24;i++){
    for(j=0;j<80;j++){
      dungeon[i][j]=' ';
    }
  }
  
  create_outline(dungeon);

  struct room all_rooms[6];

  srand(time(NULL));
  
  for(i=0;i<6;i++){
    all_rooms[i].x_size=4;
    all_rooms[i].y_size=3;
    
    int r1 = rand();
    int r2 = rand();
    r1=(r1%71)+1;
    r2=(r2%17)+1;
    /*Left off here, need to work on function to check collision*/
    all_rooms[i].x_pos=r1;
    all_rooms[i].y_pos=r2;
    
    
  }
  
  for(i=0;i<6;i++){
    struct room cur = all_rooms[i];    
    for(j=0;j<cur.y_size;j++){
      for(k=0;k<cur.x_size;k++){
	dungeon[cur.y_pos+j][cur.x_pos+k]='.';
      }
    }
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

void create_outline(char dun[24][80]){
  int i,j;

  for(i=0;i<80;i++){
    dun[0][i]='-';
    dun[20][i]='-';
  }

  for(j=1;j<20;j++){
    dun[j][0]='|';
    dun[j][79]='|';
  }
}
