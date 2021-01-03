//API for creating a Huffman code; Jeacy Espinoza

//a node in a huffman tree (N nodes total)
typedef struct min_heap_node {
	char letter;					//1  byte
	unsigned int freq;				//4  bytes
	unsigned int end_i;				//4 bytes
	struct min_heap_node *left;		//8  bytes
	struct min_heap_node *right;	//8  bytes
} Heap_Node;						//25 bytes -> 25*N bytes total

//returns a huffman tree by using a priority queue
Heap_Node *build_huffman_tree(char *letters, unsigned int *freqs, int n);

//creates a huffman code, storing it in ______
Heap_Node *create_huffman_code(char *letters, unsigned *freqs, int n, int p_flag);

//frees the huffman tree
void free_huffman_tree(Heap_Node *root);
