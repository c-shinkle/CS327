#include <stdio.h>

void times_table(void); /* Prototype prevents implicit declaration warning */

int main(int argc, char *argv[])
{
  times_table();

  return 0;
}

void times_table(void)
{
  int table[10][10]; /* Array is now allocated.  No need for "new" */
  int i, j;

  printf("My %dst %s%c\n", 1, "C Program!", '\n');

  for (i = 1; i <= 10; i++) {
    for (j = 1; j <= 10; j++) {
      table[i - 1][j - 1] = i * j;
    }
  }

  for (i = 1; i <= 10; i++) {
    for (j = 1; j <= 10; j++) {
      printf("%3d ", table[i - 1][j - 1]);
    }
    printf("\n");
  }  
}
