#ifndef _COMMON_H
#define _COMMON_H

#include <curses.h>
#include <stdlib.h>

struct snake {
  int x;
  int y;
  struct snake *prev;
  struct snake *next;
};
typedef struct snake snake_t;

enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT,
};

#define SP_LEVEL_0 1000000
#define SP_LEVEL_1 900000
#define SP_LEVEL_2 800000
#define SP_LEVEL_3 700000
#define SP_LEVEL_4 600000
#define SP_LEVEL_5 500000
#define SP_LEVEL_6 400000
#define SP_LEVEL_7 300000
#define SP_LEVEL_8 200000
#define SP_LEVEL_9 100000

#define FOOD_COLOR 1
#define SNAKE_COLOR 2
#define WALL_COLOR 3

extern snake_t *head;
extern snake_t *tail;

extern int curr;

#endif
