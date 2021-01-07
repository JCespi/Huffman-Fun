#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compress.h"

/* TO-DO
 * find a way to reduce the magnitudes of the frequencies (a percentage possibly)
 * check for memory leaks by compiling on the zoo
 * add bit fields
 * add more detailed comments to header file functions. (@param, @return)
 * add function to find percentage of compression
 * add error handling to main()
 * maybe make a main.c that has the main() that calls on encode and decode (defined in some .h)
 * add a little function that prints a frequency plot
 * write code to handle printing characters themselves. handle diff sources of redirection
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

//read file to form frequency table, generate huffman code and tree, send tree and codewords
void encode(int dump){
    Heap_Node *root;
    Code_Word *codewords;
    unsigned *freq_table, ch;

    freq_table = calloc(N_CHARS, sizeof(int));

    //build the frequency table
    while ((ch = getchar()) != EOF)
        freq_table[ch]++;

    //create the huffman code
    codewords = create_huffman_code(&root, freq_table, dump);

    //send the huffman tree
    send_huff_tree(root);

    //rewind stdin
    rewind(stdin);

    //transmit the codewords
    while ((ch = getchar()) != EOF)
        put_bits(codewords[ch].n_bits, codewords[ch].code_d);

    //flush any bits caught in buffer
    flush_bits();

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

//read the huffman tree, read codewords and output corresponding letters
void decode(void){
    Heap_Node *root, *ptr;
    long bit;

    root = receive_huff_tree();
    ptr = root;
    
    while ((bit = get_bits(1)) != EOF){
        //move the pointer appropriately
        ptr = (bit == ZERO_BIT) ? ptr->left : ptr->right;

        //a leaf node (a letter) found
        if (is_leaf(ptr)){
            putchar(ptr->letter);
            ptr = root; //reset to root of tree
        }
    }

    free_huffman_tree(root);
}

int main(int argc, char **argv){
    char *path, *exectuable;
    int i, dump;
    
    //get executable name
    path = argv[0];
    exectuable = &path[strlen(path) - 6];

    if (strcmp("decode", exectuable) == 0){
        if (argc > 1)
            DIE("%s", "decode: accepts no options");

        decode();
    } else {
        dump = 0;   //default dump option is false
        i = 1;

        while (i < argc){
            if (strcmp(argv[i], "-d") == 0){
                dump = 1;
                i++;
            } else 
                DIE("%s", "encode: unknown flag");
        }

        encode(dump);
    }

    return 0;
}
