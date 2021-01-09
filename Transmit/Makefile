CC = gcc
CFLAGS = -std=c99 -pedantic -Wall
DEPS = transmit.h
OBJS = transmit.o

all: $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o