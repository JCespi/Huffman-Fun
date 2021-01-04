#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

void encode(){
    //create freq table
    //while (c = getchar())
    //      freq_table[c]++;
    //
    //create_huffman_code()
    //
    //transmit the huffman tree
    //iterate through stdin again I guess, sending
    //freq_table[c] to stdout as a bitstream
    ;
}

void decode(){
    //receive the tree and make a replica
    //
    //while (c = get_bit_input())
    //      char = trace_through_tree <- difficult b/c happens bit by bit
    //      output char 
    ;
}

unsigned *create_freq_table(void){
    unsigned int *freq_table; //char-indexed frequency array

    freq_table = calloc(N_CHARS, sizeof(int));

    //sample input
    freq_table['a'] = 45;
    freq_table['b'] = 16;
    freq_table['c'] = 13;
    freq_table['d'] = 12;
    freq_table['e'] = 9;
    freq_table['f'] = 5;
    freq_table['j'] = 46;

    return freq_table;
}

int main(int argc, char **argv){
    Heap_Node *root;
    Code_Word *codewords;
    unsigned *freq_table;

    freq_table = create_freq_table();
    codewords = create_huffman_code(&root, freq_table, 1);

    free_huffman_tree(root);
	free(codewords);
    free(freq_table);

    return 0;
}
