CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3
DEPS = huffman.h
OBJS = huffman.o

all: $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o