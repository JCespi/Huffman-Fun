//API for creating a Huffman code; Jeacy Espinoza

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

//returns a huffman tree by using a priority queue
Heap_Node *build_huffman_tree(char *letters, unsigned int *freqs, int n);

//creates a huffman code, storing it in ______
Heap_Node *create_huffman_code(char *letters, unsigned *freqs, int n, int p_flag);

//frees the huffman tree
void free_huffman_tree(Heap_Node *root);
