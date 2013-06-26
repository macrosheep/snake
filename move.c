#include "common.h"
#include "move.h"

int cut_tail()
{
  mvaddstr(tail->y, tail->x, " ");
  return 0;
}

int draw_head()
{
  attron(COLOR_PAIR(2));
  mvaddstr(head->y, head->x, "@");
  attroff(COLOR_PAIR(2));
  return 0;
}

int move_one_step(int x, int y)
{
  snake_t * snake;

  cut_tail();

  snake = tail->prev;
  snake->next = NULL;

  tail->x = x;
  tail->y = y;
  tail->prev = NULL;
  tail->next = head;
  head->prev = tail;

  head = tail;
  tail = snake;

  draw_head();

  refresh();
  return 0;
}

int move_left()
{
  move_one_step(head->x - 1, head->y);
  return 0;
}

int move_right()
{
  move_one_step(head->x + 1, head->y);
  return 0;
}

int move_up()
{
  move_one_step(head->x, head->y - 1);
  return 0;
}

int move_down()
{
  move_one_step(head->x, head->y + 1);
  return 0;
}

int move_up_left()
{
  move_one_step(head->x - 1, head->y - 1);
  return 0;
}

int move_up_right()
{
  move_one_step(head->x + 1, head->y - 1);
  return 0;
}

int move_down_left()
{
  move_one_step(head->x - 1, head->y + 1);
  return 0;
}

int move_down_right()
{
  move_one_step(head->x + 1, head->y + 1);
  return 0;
}

int move_snake()
{
  switch (curr) {
    case UP:
      move_up();
      break;
    case DOWN:
      move_down();
      break;
    case RIGHT:
      move_right();
      break;
    case LEFT:
      move_left();
      break;
    case UP_LEFT:
      move_up_left();
      break;
    case UP_RIGHT:
      move_up_right();
      break;
    case DOWN_RIGHT:
      move_down_right();
      break;
    case DOWN_LEFT:
      move_down_left();
      break;
    default:
      return 0;
  };
}
