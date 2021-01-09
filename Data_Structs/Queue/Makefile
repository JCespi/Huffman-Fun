CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3
DEPS = queue.h
OBJS = queue.o

all: $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o