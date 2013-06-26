#include <locale.h>
#include <pthread.h>
#include "common.h"
#include "move.h"
#include "food.h"
#include "map.h"

#define INIT_SIZE 6

#define KEY_Q 9000
#define KEY_W 9001
#define KEY_E 9002
#define KEY_S 9003
#define KEY_D 9004
#define KEY_Z 9005
#define KEY_X 9006
#define KEY_R 9007
#define KEY_P 9008

snake_t *head = NULL;
snake_t *tail = NULL;

int curr = RIGHT;
int input = 0;
int exit_flag = 0;
int pause_flag = 0;
int score = 0;
int speed = SP_LEVEL_5;
pthread_t thread_input;

int setup_env()
{
  setlocale(LC_ALL, "C");

  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);

  define_key("q", KEY_Q);
  define_key("w", KEY_W);
  define_key("e", KEY_E);
  define_key("s", KEY_S);
  define_key("d", KEY_D);
  define_key("z", KEY_Z);
  define_key("x", KEY_X);
  define_key("r", KEY_R);
  define_key("p", KEY_P);

  start_color();
  init_pair(FOOD_COLOR, COLOR_RED, COLOR_BLACK); /*food color*/
  init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK); /*snake color*/
  init_pair(WALL_COLOR, COLOR_WHITE, COLOR_BLACK); /*wall color*/

  return 0;
}

void *get_input()
{
  while (1) {
    input = getch();
  };

  pthread_exit(NULL);
}

int init_snake()
{
  snake_t * snake;
  snake_t * prev;
  int x = COLS/2;
  int y = LINES/2;
  int lx = 0;

  head = snake = malloc(sizeof(snake_t));
  if (NULL == snake)
    return -1;
  snake->y = y;
  snake->x = x;
  snake->prev = NULL;
  snake->next = NULL;

  prev = head;
  lx = x-INIT_SIZE;
  for (x--; x > lx; x--) {
    snake = malloc(sizeof(snake_t));
    if (NULL == snake)
      return -1;
    snake->y = y;
    snake->x = x;
    snake->prev = prev;
    snake->prev->next = snake;
    snake->next = NULL;

    prev = snake;
  }

  tail = prev;

  /*initial direction*/
  curr = RIGHT;

  return 0;
}

int draw_snake()
{
  snake_t * snake = head;

  attron(COLOR_PAIR(SNAKE_COLOR));
  while (NULL != snake) {
    mvaddstr(snake->y, snake->x, "@");
    snake = snake->next;
  };
  attroff(COLOR_PAIR(SNAKE_COLOR));
  refresh();

  return 0;
}

int snake_grow_one()
{
  snake_t * snake = NULL;
  snake = malloc(sizeof(snake_t));
  if (NULL == snake)
    return -1;
  snake->y = 0;
  snake->x = 0;
  snake->prev = tail;
  snake->prev->next = snake;
  snake->next = NULL;

  tail = snake;
  return 0;
}

int destory_snake()
{
  snake_t * snake = head;
  snake_t * tmp = NULL;

  while (NULL != snake) {
    tmp = snake;
    snake = snake->next;
    free(tmp);
  };
  head = tail = NULL;

  return 0;
}

int exit_snake()
{
  destory_snake();
  destory_map();

  return 0;
}

int check_input()
{
  if (input == KEY_LEFT && curr != RIGHT)
    curr = LEFT;
  else if (input == KEY_UP && curr != DOWN)
    curr = UP;
  else if (input == KEY_RIGHT && curr != LEFT)
    curr = RIGHT;
  else if (input == KEY_DOWN && curr != UP)
    curr = DOWN;
  else if (input == KEY_W && curr != DOWN_RIGHT)
    curr = UP_LEFT;
  else if (input == KEY_E && curr != DOWN_LEFT)
    curr = UP_RIGHT;
  else if (input == KEY_S && curr != UP_RIGHT)
    curr = DOWN_LEFT;
  else if (input == KEY_D && curr != UP_LEFT)
    curr = DOWN_RIGHT;
  else if (input == KEY_Q)
    exit_flag = 1;
  else if (input == KEY_P)
    pause_flag = 1;
  else if (input == KEY_Z && speed < 1000000)
    speed = speed + 100000;
  else if (input == KEY_X && speed > 100000)
    speed = speed - 100000;
  else
    return 0;

  input = 0;
  return 0;
}

int is_dead_snake()
{
  snake_t * snake = head->next;

  while (NULL != snake) {
    if (snake->x == head->x && snake->y == head->y)
      return 1;
    snake = snake->next;
  };

  return 0;
}

int check_dead()
{
  if (head->x < 1 || head->x > COLS-3 || head->y < 2 || head->y > LINES-2)
    return 1;

  if (is_dead_snake())
    return 1;

  if (is_wall(head->x, head->y))
    return 1;

  return 0;
}

int snake_dead_exit()
{
  int input;
  int ret;

  mvaddstr(LINES/2, COLS/2-10, "Retry(R)  Exit(E)");
input:
  input = getch();

  switch (input) {
    case KEY_E:
      ret = 1;
      break;
    case KEY_R:
      ret = 0;
      break;
    default:
      goto input;
  };

  return ret;
}

int inc_score()
{
  score++;
}

int draw_score()
{
  char str[20];

  sprintf(str, "Score: %d", score);
  mvaddstr(0, 1, str);
  refresh();

  return 0;
}

int draw_help()
{
  mvaddstr(0, 15, "q:quit p:pause | move: w:upleft e:upright s:downleft d:downright x:speedup z:speeddown");
  refresh();
}

int draw_box(int x, int y, int hight, int width)
{
  mvhline(y, x+1, ACS_HLINE, width-1);
  mvhline(y+hight, x+1, ACS_HLINE, width-1);
  mvvline(y+1, x, ACS_VLINE, hight-1);
  mvvline(y+1, x+width, ACS_VLINE, hight-1);
  mvaddch(y, x, ACS_ULCORNER);
  mvaddch(y, x+width, ACS_URCORNER);
  mvaddch(y+hight, x, ACS_LLCORNER);
  mvaddch(y+hight, x+width, ACS_LRCORNER);

  refresh();
}

int snake_start()
{
restart:
  score = 0;
  init_snake();
  init_map();
  clear();
  refresh();
  draw_score();
  draw_help();
  draw_box(0, 1, LINES-2, COLS-2);
  draw_snake();
  draw_map();
  gen_food();

  if ((pthread_create(&thread_input, NULL, get_input, NULL)) != 0)
    exit(1);
  while (1) {
    usleep(speed);
    check_input();
    if (exit_flag) {
      pthread_cancel(thread_input);
      exit_snake();
      break;
    }
    if (pause_flag) {
      pthread_cancel(thread_input);
      getch();
      pause_flag = 0;
      if ((pthread_create(&thread_input, NULL, get_input, NULL)) != 0)
        exit(1);
    }
    move_snake();
    if (check_dead()) {
      pthread_cancel(thread_input);
      if (snake_dead_exit()) {
        exit_snake();
        break;
      }
      goto restart;
    }
    if (check_food()) {
      inc_score();
      draw_score();
      snake_grow_one();
      gen_food();
    }
  };

  return 0;
}

int main()
{
  int i=0;

  setup_env();

  snake_start();

  endwin();
  return 0;
}
