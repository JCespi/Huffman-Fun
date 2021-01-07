CC = gcc
CFLAGS = -std=c99 -pedantic -Wall 
DEPS = huffman.h heap.h transmit.h compress.h table.h queue.h
OBJ = compress.o huffman.o heap.o transmit.o table.o queue.o

all: encode decode

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

encode: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

decode: encode
	ln -f encode decode

clean:
	rm -f encode decode *.o