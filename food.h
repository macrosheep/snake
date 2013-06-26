#ifndef _FOOD_H
#define _FOOD_H

struct food {
  int x;
  int y;
};
typedef struct food food_t;

int gen_food();
int check_food();

#endif
