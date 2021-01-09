CC = gcc
CFLAGS = -std=c99 -pedantic -Wall 
DEPS = compress.h 				    \
	   ./Huffman/huffman.h 		    \
	   ./Data_Structs/Heap/heap.h   \
	   ./Data_Structs/Queue/queue.h \
	   ./Transmit/transmit.h 		\
	   ./Table/table.h
OBJS = ./main.o 			        \
	   ./compress.o 		        \
       ./Huffman/huffman.o 	 	    \
	   ./Data_Structs/Heap/heap.o   \
	   ./Data_Structs/Queue/queue.o \
	   ./Transmit/transmit.o 		\
	   ./Table/table.o
MFLAGS = clean

all: encode decode

sub_folders: +$(MAKE) -C Data_Structs \
	 		 +$(MAKE) -C Huffman      \
	 		 +$(MAKE) -C Table 	      \
	 		 +$(MAKE) -C Transmit

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

encode: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

decode: encode
	ln -f encode decode

clean:
	rm -f encode decode *.o
	cd Data_Structs && $(MAKE) $(MFLAGS)
	cd Huffman 		&& $(MAKE) $(MFLAGS)
	cd Table 		&& $(MAKE) $(MFLAGS)
	cd Transmit 	&& $(MAKE) $(MFLAGS)