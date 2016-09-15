#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <endian.h>

struct room{
  char x_pos;
  char y_pos;
  char x_size;
  char y_size;
};

char dungeon[24][80];

uint8_t hardness[21][80];

uint32_t version;

uint32_t room_num;

struct room all_rooms[50];

void make_blank(void);

void generate_hardness(void);

void create_outline(void);

void generate_rooms(void);

void draw_corridors(struct room from, struct room to);

int can_collide(struct room *room_1, struct room *room_2);

void paint_rooms(void);

void print_rooms(void);

void room_centroid(int *x, int *y, struct room r);

static int numAdjacentPaths(int r, int c);

void load_file(void);

void save_file(void);

int main(int argc, char *argv[]){
  int i;
  int save_flag=0;
  int load_flag=0;
  for(i=1;i<argc;i++){
    if(!strcmp(argv[i], "--load"))
      load_flag = 1;
    if(!strcmp(argv[i], "--save"))
      save_flag = 1;
  }
 
  srand(time(NULL));
  if(load_flag && save_flag){
    
    load_file();
    
    make_blank();
    
    create_outline();
    
    int i;
    for(i = 1; i<room_num; i++)
      draw_corridors(all_rooms[i], all_rooms[0]);
    
    paint_rooms();

    print_rooms();
    
    save_file();
  }
  else if(load_flag){
      
    load_file();
    
    make_blank();
    
    create_outline();
    
    int i;
    for(i = 1; i<room_num; i++)
      draw_corridors(all_rooms[i], all_rooms[0]);
    
    paint_rooms();

    print_rooms();
  } 
  else if(save_flag){

    room_num = 6;
    version = 0;
    
    generate_hardness();

    make_blank();
  
    create_outline();
    
    generate_rooms();
  
    paint_rooms();
  
    print_rooms();
    
    save_file();
  }
  else{
    room_num = 6;
    version = 0;
    
    generate_hardness();

    make_blank();
  
    create_outline();
    
    generate_rooms();
  
    paint_rooms();
  
    print_rooms();
  }
  return 0;
}



void make_blank(void){
  int i, j;
  for(i=0;i<24;i++){
    for(j=0;j<80;j++){
      dungeon[i][j]=' ';
    }
  }
}

void generate_hardness(void){
  int i, j;
  for(i=0;i<80;i++){
    hardness[0][i] = 255;
    hardness[20][i] = 255;
  }
  for(i=1;i<20;i++){
    hardness[i][0]=255;
    hardness[i][79]=255;
  }
  for(i=1;i<20;i++){
    for(j=1;j<79;j++){
      hardness[i][j] = rand()%254+1;
    }
  }
}

void create_outline(void){
  int i,j;

  for(i=0;i<80;i++){
    dungeon[0][i]='-';
    dungeon[20][i]='-';
  }

  for(j=1;j<20;j++){
    dungeon[j][0]='|';
    dungeon[j][79]='|';
  }
}

int can_collide(struct room *room_1, struct room *room_2){
  int xpr1 = room_1->x_pos;
  int ypr1 = room_1->y_pos;
  int xsr1 = room_1->x_size;
  int ysr1 = room_1->y_size;
  
  int xpr2 = room_2->x_pos;
  int ypr2 = room_2->y_pos;
  int xsr2 = room_2->x_size;
  int ysr2 = room_2->y_size;
  
  if(xpr1-xpr2<=0){
    if(xpr2-xpr1-xsr1>0)
      return 0;
    else{/*x-coor. do intersect, so there is a chance that the y-coor. intersect also*/
      if(ypr1-ypr2<=0){
	if(ypr2-ypr1-ysr1>0)
	  return 0;
	else
	  return 1;
      }
      else{
	if (ypr1-ypr2-ysr2>0)
	  return 0;
	else
	  return 1;
      }
    }
  }
  else{
    if(xpr1-xpr2-xsr2>0)
      return 0;
    else{
      if(ypr1-ypr2<=0){
	if(ypr2-ypr1-ysr1>0)
	  return 0;
	else
	  return 1;
      }
      else{
	if(ypr1-ypr2-ysr2>0)
	  return 0;
	else
	  return 1;
      }
    }
  }
}

void generate_rooms(void){
  int i;
  int j=0;
  for(i=0;i<room_num;i++){
    all_rooms[i].x_size = rand()%3+4;
    all_rooms[i].y_size = rand()%3+3;
    
    int r1 = rand();
    int r2 = rand();
    r1 = (r1%72) + 2;
    r2 = (r2%13) + 3;
    all_rooms[i].x_pos = r1;
    all_rooms[i].y_pos = r2;
    /*
      if(all_rooms[i].x_pos + all_rooms[i].x_size>78 
      || all_rooms[i].y_pos + all_rooms[i].y_size>22){
      i--;
      continue;
      }
    */
    if(i>0){
      for(j = i-1;j>=0;j--){
        if(can_collide(&all_rooms[i], &all_rooms[j])){
	  i--;
	  break;
	}
      }
      if(j==-1)
	draw_corridors(all_rooms[i], all_rooms[0]);
    }
  }  
}

void draw_corridors(struct room from, struct room to){
  
  int fromx, fromy, tox, toy;
	
  room_centroid(&fromx, &fromy, from);
  room_centroid(&  tox, &  toy,   to);

  int dx = tox - fromx;
  int dy = toy - fromy;

  int incx = dx > 0 ? 1 : -1;
  int incy = dy > 0 ? 1 : -1;
	
  int r = fromy, c = fromx;

  while (r!=toy && c!=tox)
    {	
      if (dungeon[r][c]!= '.')
	dungeon[r][c] = '#';
		
      // if we connect to existing path, then stop
      if (numAdjacentPaths(r, c)>1)
	break;
			
      if (rand() % 2)
	c+=incx;
      else
	r+=incy;
    }
  while (r!=toy || c!=tox)
    {
      if (dungeon[r][c]!= '.')
	dungeon[r][c] = '#';

      // if we connect to existing path, then stop
      if (numAdjacentPaths(r, c)>1)
	break;

      if (r!=toy)
	r+=incy;
      if (c!=tox)
	c+=incx;
    }
}

void paint_rooms(void){
  int i, j, k;
  for(i=0;i<room_num;i++){
    struct room cur = all_rooms[i];
    for(j=0;j<cur.y_size;j++){
      for(k=0;k<cur.x_size;k++){
	dungeon[cur.y_pos+j][cur.x_pos+k]='.';
      }
    }
  }
}

void print_rooms(void){
  int i, j;
  for(i=0;i<24;i++){
    for(j=0;j<80;j++){
      printf("%c",dungeon[i][j]);
    }
    printf("\n");
  }
}

void room_centroid(int *x, int *y, struct room r){
  *x = r.x_pos + r.x_size/2;
  *y = r.y_pos + r.y_size/2;
}

static int numAdjacentPaths(int r, int c){
  int num = 0;
  if(dungeon[r-1][c]=='#')
    num++;
  if(dungeon[r+1][c]=='#')
    num++;
  if(dungeon[r][c-1]=='#')
    num++;
  if(dungeon[r][c+1]=='#')
    num++;
  return num;
}

void load_file(void){
  FILE *f;
  char n[6];
  char *home = getenv("HOME");
  if(!home){
    printf("The home path wasn't found");
    exit(-1);
  }
  
  strcat(home, "/.rlg327/dungeon");
  
  f = fopen(home, "r");
  
  fread(n, sizeof(char), 6, f);

  fread(&version, sizeof(uint32_t), 1, f);
  version = be32toh(version);

  fread(&room_num, sizeof(uint32_t), 1, f);
  room_num = be32toh(room_num);
  room_num =  (room_num - 1694)/4;

  fread(hardness, 1, 80*21, f);
 
  int i;
  for(i=0; i<room_num; i++){
    uint8_t h,j,k,l;
    
    fread(&h, 1, 1, f);
    fread(&j, 1, 1, f);
    fread(&k, 1, 1, f);
    fread(&l, 1, 1, f);
    
    all_rooms[i].x_pos = h;
    all_rooms[i].x_size = j;
    all_rooms[i].y_pos = k;
    all_rooms[i].y_size = l;
  }
 

 
  if(fclose(f)){
    printf("Something went wrong while closing");
    exit(-1);
  }
}

void save_file(void){
  FILE *f;
  char n[6] = "RLG327";
  char *home = getenv("HOME");
  int temp_room_num = room_num;
  uint32_t size = 1694 + (room_num * 4);
  
  if(!home)
    exit(-1);
  
  strcat(home, "/.rlg327/dungeon");
  
  f = fopen(home, "w");
  
  fwrite(n, sizeof(char), 6, f);

  version++;
  version = htobe32(version);
  fwrite(&version, sizeof(uint32_t), 1, f);
  
  size = htobe32(size);
  fwrite(&size, sizeof(uint32_t), 1, f);
  
  fwrite(hardness, sizeof(uint8_t), 80*21, f);

  int i;
  for(i=0;i<temp_room_num;i++){
    uint8_t h, j, k, l;
    h = all_rooms[i].x_pos;
    j = all_rooms[i].x_size;
    k = all_rooms[i].y_pos;
    l = all_rooms[i].y_size;
    
    fwrite(&h, 1, 1, f);
    fwrite(&j, 1, 1, f);
    fwrite(&k, 1, 1, f);
    fwrite(&l, 1, 1, f);
  }

  if(fclose(f)){
    printf("Something went wrong while closing");
    exit(-1);
  } 
}

