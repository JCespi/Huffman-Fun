//API for creating a Huffman code; Jeacy Espinoza

//a node in a huffman tree
typedef struct min_heap_node {
	char letter;
	unsigned int freq;
	struct min_heap_node *left;
	struct min_heap_node *right;
} Heap_Node;

//returns a huffman tree by using a priority queue
Heap_Node *build_huffman_tree(char *letters, unsigned int *freqs, int n);

//creates a huffman code, storing it in ______
Heap_Node *create_huffman_code(char *letters, unsigned *freqs, int n, int p_flag);

//frees the huffman tree
void free_huffman_tree(Heap_Node *root);
