CC = gcc
CFLAGS = -std=c99 -pedantic -Wall 

compress: compress.o huffman.o
	$(CC) $(CFLAGS) -o compress compress.o huffman.o

compress.o: huffman.h
huffman.o: huffman.h min_heap.h

clean:
	rm -f compress *.o