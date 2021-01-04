#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "transmit.h"

/* TO-DO
 * find a way to reduce the magnitudes of the frequencies (a percentage possibly)
 * write a commandline parser for compress
 * write an encoder that uses the huffman() to send a sequence of bits (maybe in form of bytes?)
 * write a decoder that uses the data structure thought up ^^ there to decode
 * check for memory leaks by compiling on the zoo
 * add bit fields
 * write code in encode to copy stdin to a temporary file
*/

void encode(){
    Heap_Node *root;
    Code_Word *codewords;
    unsigned *freq_table, ch;

    freq_table = calloc(N_CHARS, sizeof(int));

    //build the frequency table
    while ((ch = getchar()) != EOF)
        freq_table[ch]++;

    //create the huffman code
    codewords = create_huffman_code(&root, freq_table, 1);

    //rewind stdin

    free_huffman_tree(root);
    free(freq_table);
    free(codewords);
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
    encode();

    return 0;
}
