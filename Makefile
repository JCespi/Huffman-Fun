CC = gcc
CFLAGS = -std=c99 -pedantic -Wall 

huffman: huffman.o
	$(CC) $(CFLAGS) -o huffman huffman.o

clean:
	rm -f huffman *.o