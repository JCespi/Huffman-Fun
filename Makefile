CC = gcc
CFLAGS = -std=c99 -pedantic -Wall 
DEPS = huffman.h heap.h
OBJ = compress.o huffman.o heap.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

compress: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f compress *.o