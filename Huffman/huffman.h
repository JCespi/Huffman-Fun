//API for creating a Huffman code; Jeacy Espinoza

//available macro(s)
#define INTERNAL_NODE_MARKER '$'

//a node in a huffman tree (N nodes total)
typedef struct heap_node {
	unsigned letter;		//1  byte
	float freq;		    		//4  bytes
	struct heap_node *left;		//8  bytes
	struct heap_node *right;	//8  bytes
} Heap_Node;					//21 bytes -> 21*N bytes total

//contains the codeword (decimal) and the num of bits
typedef struct code_word {
	unsigned int code_d;		//codeword in decimal notation
	unsigned int n_bits;		//num of bits and indicator of existence
} Code_Word;
//==============================================================
//returns a node with the given parameters. user's responsibility to free
Heap_Node *create_huff_node(unsigned letter, float freq);

//determines whether the node has a letter/is a leaf
int is_leaf(Heap_Node *node);

//finds the average length of the code
float find_avg_len(Code_Word *codewords, unsigned table_size);

//finds the maximum number of bits among codewords or maximum codeword
unsigned find_max(Code_Word *codewords, unsigned table_size, int n_bits);

//fills in root argument and returns array of codewords
Code_Word *create_huffman_code(Heap_Node **root, float *freq_table, unsigned table_size);

//frees the huffman tree
void free_huffman_tree(Heap_Node *root);
//==============================================================
