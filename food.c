#include "common.h"
#include "food.h"
#include "map.h"

food_t food;

int is_snake(int x, int y)
{
  snake_t * snake = head;

  while (NULL != snake) {
    if (snake->x == x && snake->y == y)
      return 1;
    snake = snake->next;
  };

  return 0;
}

int invalid_food()
{
  if (is_snake(food.x, food.y))
    return 1;

  if (is_wall(food.x, food.y))
    return 1;

  return 0;
}

int gen_food()
{
regen:
  srand(time(NULL));
  food.x = rand()%(COLS-2) + 1;
  food.y = rand()%(LINES-3) + 2;

  if (invalid_food())
    goto regen;

  attron(COLOR_PAIR(FOOD_COLOR));
  mvaddstr(food.y, food.x, "*");
  attroff(COLOR_PAIR(FOOD_COLOR));
  refresh();
  return 0;
}

int check_food()
{
  if (head->x == food.x && head->y == food.y)
    return 1;

  return 0;
}
