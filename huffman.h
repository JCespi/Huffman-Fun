//API for creating a Huffman code; Jeacy Espinoza
#include "heap.h"

//available macros
#define INTERNAL_NODE_MARKER '$'
#define N_CHARS 256
#define ZERO_BIT 0
#define ONE_BIT  1

//a node in a huffman tree (N nodes total)
typedef struct heap_node {
	unsigned int letter;		//1  byte
	unsigned int freq;		    //4  bytes
	struct heap_node *left;		//8  bytes
	struct heap_node *right;	//8  bytes
} Heap_Node;					//21 bytes -> 21*N bytes total

//contains the codeword (decimal) and the num of bits
typedef struct code_word {
	unsigned int code_d;		//codeword in decimal notation
	unsigned int n_bits;		//num of bits and indicator of existence
} Code_Word;

//returns a node with the given parameters. user's responsibility to free
Heap_Node *create_huff_node(char letter, unsigned int freq);

//determines whether the node has a letter/is a leaf
int is_leaf(Heap_Node *node);

//fills in root argument and returns array of codewords
Code_Word *create_huffman_code(Heap_Node **root, unsigned *freq_table, int p_flag);

//frees the huffman tree
void free_huffman_tree(Heap_Node *root);
