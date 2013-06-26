#include "common.h"

int **wall_map;

int init_map()
{
  int i,j;

  wall_map = (int **)calloc(COLS, sizeof(int*));
  for (i=0; i<COLS; i++)
    wall_map[i] = (int *)calloc(LINES, sizeof(int));

  for (i=0; i<10; i++)
    wall_map[COLS/2+20][LINES/2-5+i] = 1;
  for (i=0; i<10; i++)
    wall_map[COLS/2+21][LINES/2-5+i] = 1;
  for (i=0; i<10; i++)
    wall_map[COLS/2-20][LINES/2-5+i] = 1;
  for (i=0; i<10; i++)
    wall_map[COLS/2-21][LINES/2-5+i] = 1;
}

int destory_map()
{
  int i;

  for (i=0; i<COLS; i++)
    free(wall_map[i]);
  free(wall_map);

  return 0;
}

int draw_map()
{
  int i,j;

  attron(COLOR_PAIR(WALL_COLOR));
  for (i=0; i<COLS; i++)
    for (j=0; j<LINES; j++)
      if (wall_map[i][j])
        mvaddstr(j, i, "#");
  attroff(COLOR_PAIR(WALL_COLOR));

  refresh();
  return 0;
}

int is_wall(int x, int y)
{
  if (wall_map[x][y])
    return 1;

  return 0;
}
