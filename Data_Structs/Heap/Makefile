CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3
DEPS = heap.h
OBJS = heap.o

all: $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
