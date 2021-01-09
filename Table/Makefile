CC = gcc
CFLAGS = -std=c99 -pedantic -Wall
DEPS = table.h
OBJS = table.o

all: $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o