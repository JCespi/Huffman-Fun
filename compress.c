#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
#include "transmit.h"

/* TO-DO
 * find a way to reduce the magnitudes of the frequencies (a percentage possibly)
 * check for memory leaks by compiling on the zoo
 * add bit fields
 * add error handling to main()
 * maybe make a main.c that has the main() that calls on encode and decode (defined in some .h)
 * add a little function that prints a frequency plot
 * write a print function in huffman.c that dumps following info into a given file:
 *          - letter : frequency : codeword (binary) : codeword (decimal) : num of bits
 *          - put the box-drawing code in its own box.c and box.h files 
 *          - make it general (linked list of void pointers, everything is turned into a string)
*/

//start of a function to reduce magnitudes of frequencies (why though?)
void squish_freqs(unsigned *freq_table){
    unsigned small_freq, i;

    small_freq = freq_table[0];
    for (i=1; i < N_CHARS; i++)
        if (freq_table[i] && freq_table[i] < small_freq)
            small_freq = freq_table[i];
    for (i=0; i < N_CHARS; i++)
        freq_table[i] /= small_freq;
}

//a high bit followed by a byte signifies a leaf node with following letter
void send_huff_tree(Heap_Node *root){
    if (is_leaf(root)){
        put_bits(1, ONE_BIT);
        put_bits(8, root->letter);
    } else {
        put_bits(1, ZERO_BIT);
        send_huff_tree(root->left);
        send_huff_tree(root->right);
    }
}

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

    //send the huffman tree
    send_huff_tree(root);

    //rewind stdin
    rewind(stdin);

    //transmit the codewords
    while ((ch = getchar()) != EOF)
        put_bits(codewords[ch].n_bits, codewords[ch].code_d);

    //free the tree, the codeword table, and the frequency table
    free_huffman_tree(root);
    free(freq_table);
    free(codewords);
}

//receives stream of bits sent from encode() to construct tree
Heap_Node *receive_huff_tree(void){
    Heap_Node *root;

    if (get_bits(1) == ZERO_BIT){   //internal node
        root = create_huff_node(INTERNAL_NODE_MARKER, 0);
        root->left = receive_huff_tree();
        root->right = receive_huff_tree();
        return root;
    } else {                        //leaf node
        return create_huff_node(get_bits(8), 0);
    }
}

void decode(void){
    Heap_Node *root;

    root = receive_huff_tree();
    
    //while (c = get_bit_input())
    //      char = trace_through_tree <- difficult b/c happens bit by bit
    //      output char 
    free_huffman_tree(root);
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
