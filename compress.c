#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
#include "transmit.h"

/* TO-DO
 * find a way to reduce the magnitudes of the frequencies (a percentage possibly)
 * write an encoder that uses the huffman() to send a sequence of bits (maybe in form of bytes?)
 * write a decoder that uses the data structure thought up ^^ there to decode
 * check for memory leaks by compiling on the zoo
 * add bit fields
 * write code in encode to copy stdin to a temporary file
 * add error handling to main()
 * maybe make a main.c that has the main() that calls on encode and decode (defined in some .h)
*/

void encode(void){
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

void decode(void){
    //receive the tree and make a replica
    //
    //while (c = get_bit_input())
    //      char = trace_through_tree <- difficult b/c happens bit by bit
    //      output char 
    ;
}

int main(int argc, char **argv){
    char *path, *exectuable;
    
    //get executable name
    path = argv[0];
    exectuable = &path[strlen(path) - 6];

    if (strcmp("decode", exectuable) == 0){
        decode();
    } else {

        encode();
    }

    return 0;
}
