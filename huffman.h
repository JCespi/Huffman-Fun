//API for creating a Huffman code; Jeacy Espinoza
#include "min_heap.h"

#define N_CHARS 256

//a node in a huffman tree (N nodes total)
typedef struct min_heap_node {
	unsigned int letter;		    //1  byte
	unsigned int freq;				//4  bytes
	struct min_heap_node *left;		//8  bytes
	struct min_heap_node *right;	//8  bytes
} Heap_Node;						//25 bytes -> 21*N bytes total

//contains the codeword (decimal) and the num of bits
typedef struct code_word {
	unsigned int code_d;		//codeword in decimal notation
	unsigned int n_bits;		//num of bits and indicator of existence
} Code_Word;

//fills in root argument and returns array of codewords
Code_Word *create_huffman_code(Heap_Node **root, unsigned *freq_table, int p_flag);

//given a codeword node, determines whether that codeword exists
int code_exists(Code_Word codeword);

//frees the huffman tree
void free_huffman_tree(Heap_Node *root);
