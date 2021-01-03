#include <stdio.h>
#include <stdlib.h>

#define INTERNAL_NODE_MARKER '$'
#define ZERO_BIT 0
#define ONE_BIT  1

typedef struct min_heap_node {
	char letter;
	unsigned int freq;
	struct min_heap_node *left;
	struct min_heap_node *right;
} Heap_Node;

typedef struct min_heap {
	unsigned int size;
	unsigned int capacity;
	Heap_Node **array;
} Min_Heap;

/* TO-DO
 * understand the code and the algorithm
 * fix output
 * write function to find height of huffman tree
 * fix the free-ing (free the min heap container and the tree)
 * add code to find the average length of the huffman code
 * make an API to make working with essential functions easier for items below
 * think of a data structure to hold the codes
 * use a hashtable to store the letters along with their frequencies b/c
 * 			- I can then delete the letter field in the node (no need for internal node marker)
 * 			- wouldn't work if there were duplicate frequencies however
 * write an encoder that uses the huffman() to send a sequence of bits (maybe in form of bytes?)
 * write a decoder that uses the data structure thought up ^^ there to decode
*/

//====================Heap Functions===========================
Min_Heap *create_heap(unsigned int capacity){
	Min_Heap *min_heap;

	min_heap = malloc(sizeof(Min_Heap));
	min_heap->size = 0;
	min_heap->capacity = capacity;
	min_heap->array = malloc(sizeof(Heap_Node*) * capacity);

	return min_heap;
}

Heap_Node *create_new_node(char letter, unsigned int freq){
	Heap_Node *new_node;

	new_node = malloc(sizeof(Heap_Node));
	new_node->letter = letter;
	new_node->freq = freq;
	new_node->left = new_node->right = NULL;
	
	return new_node;	
}

void swap_heap_nodes(Heap_Node **n1, Heap_Node **n2){
	Heap_Node *temp_node;

	temp_node = *n1;
	*n1 = *n2;
	*n2 = temp_node;
}

void heapify(Min_Heap *min_heap, unsigned int index){
	unsigned int smallest, left, right;
	
	smallest = index;
	left = 2 * index + 1;
	right = 2 * index + 1;

	if (left < min_heap->size && min_heap->array[left]->freq < min_heap->array[smallest]->freq)
		smallest = left;
	
	if (right < min_heap->size && min_heap->array[right]->freq < min_heap->array[smallest]->freq)
		smallest = right;
	
	if (smallest != index){
		swap_heap_nodes(&min_heap->array[smallest], &min_heap->array[index]);
		heapify(min_heap, smallest);
	}
}

int has_one_element(Min_Heap *min_heap){
	return min_heap->size == 1;
}

Heap_Node *extract_min(Min_Heap *min_heap){
	Heap_Node *min_node;

	min_node = min_heap->array[0];								//pop off the root
	min_heap->array[0] = min_heap->array[min_heap->size - 1];	//insert (large) leaf as root
	min_heap->size--;											//decrease heap size
	heapify(min_heap, 0);										//ripple the new root down

	return min_node;
}

void insert_new_node(Min_Heap *min_heap, Heap_Node *new_node){
	int i;

	min_heap->size++;
	i = min_heap->size - 1;

	while (i && new_node->freq < min_heap->array[(i - 1) / 2]->freq){
		min_heap->array[i] = min_heap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	min_heap->array[i] = new_node;
}

int is_leaf(Heap_Node *node){
	return !(node->left) && !(node->right);
}
	
void build_heap(Min_Heap *min_heap){
	int i, n;

	n = min_heap->size - 1;
	i = (n - 1) /2;

	while (i >= 0){
		heapify(min_heap, i);
		i--;
	}
}

void free_min_heap(Min_Heap *min_heap){
	free(min_heap->array);
	free(min_heap);
}
//=============================================================
Heap_Node *extract_huff_tree(Min_Heap *min_heap){
	if (min_heap->size != 1)
		return NULL;

	return min_heap->array[0];
}

Min_Heap *create_and_build_heap(char *letters, unsigned int *freqs, int n){
	Min_Heap *min_heap;
	int i;

	min_heap = create_heap(n);

	for (i = 0; i < n; i++)
		min_heap->array[i] = create_new_node(letters[i], freqs[i]);
	
	min_heap->size = n;
	build_heap(min_heap);

	return min_heap;
}

Heap_Node *build_huffman_tree(char *letters, unsigned int *freqs, int n){
	Heap_Node *left_node, *top_node, *right_node, *root_node;
	Min_Heap *min_heap;
	unsigned int combined_freq;

	min_heap = create_and_build_heap(letters, freqs, n);

	while (!has_one_element(min_heap)){
		left_node = extract_min(min_heap);
		right_node = extract_min(min_heap);

		combined_freq = left_node->freq + right_node->freq;
		top_node = create_new_node(INTERNAL_NODE_MARKER, combined_freq);
		top_node->left = left_node;
		top_node->right = right_node;

		insert_new_node(min_heap, top_node);
	}

	root_node = extract_huff_tree(min_heap);	//extract the huffman tree
	free_min_heap(min_heap);					//free min heap container

	return root_node;
}

void print_arr(int *arr, int n) { 
    int i; 

    for (i = 0; i < n; ++i) 
        printf("%d", arr[i]); 
  
    printf("\n"); 
} 

void print_codes(Heap_Node *root, int *buffer, int b_index){

	if (root->left){
		buffer[b_index] = ZERO_BIT;
		print_codes(root->left, buffer, b_index + 1);
	}

	if (root->right){
		buffer[b_index] = ONE_BIT;
		print_codes(root->right, buffer, b_index + 1);
	}

	if (is_leaf(root)){
		printf("%c: ", root->letter);
		print_arr(buffer, b_index);
	}
}

unsigned int get_huff_tree_height(Heap_Node *root){
	return 100;
}

Heap_Node *create_huffman_code(char *letters, unsigned int *freqs, int n){
	Heap_Node *root;
	int *buffer, b_index;

	root = build_huffman_tree(letters, freqs, n);
	buffer = malloc(sizeof(int) * get_huff_tree_height(root));
	b_index = 0;

	print_codes(root, buffer, b_index);

	return root;
}

void free_huffman_tree(Heap_Node *root){
	if (root == NULL)
		return;
	else {
		if (root->left)
			free_huffman_tree(root->left);
		
		if (root->right)
			free_huffman_tree(root->right);
		
		free(root);
	}
}
//=============================================================
int main(int argc, char **argv){
	Heap_Node *root;

	char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' }; 
    unsigned int freq[] = { 5, 9, 12, 13, 16, 45 }; 
	int size = 6;

	root = create_huffman_code(arr, freq, size);
	free_huffman_tree(root);

	return 0;
}
//=============================================================
