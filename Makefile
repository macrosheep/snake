OBJS=snake.o\
     move.o\
     food.o\
     map.o
LDFLAGS=-lcurses -lpthread
CC=gcc
CCFLAGS=-w -g -c

snake:$(OBJS)
	$(CC) -o snake $(OBJS) $(LDFLAGS)

snake.o:snake.c common.h move.h food.h map.h
	$(CC) $(CCFLAGS) snake.c

move.o:move.c common.h
	$(CC) $(CCFLAGS) move.c

food.o:food.c common.h
	$(CC) $(CCFLAGS) food.c

map.o:map.c common.h
	$(CC) $(CCFLAGS) map.c

clean:
	rm -rf *.o snake
