CC = gcc
CFLAGS = -std=c99 -pedantic -Wall 

huffman: huffman.o
	$(CC) $(CFLAGS) -o huffman huffman.o

huffman.o: huffman.h

clean:
	rm -f huffman *.o