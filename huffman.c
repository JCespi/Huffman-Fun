#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

#define INTERNAL_NODE_MARKER '$'
#define ZERO_BIT 0
#define ONE_BIT  1

//container for the minimum heap
typedef struct min_heap {
	unsigned int size;
	unsigned int capacity;
	Heap_Node **array;
} Min_Heap;

/* TO-DO
 * add code to find the average length of the huffman code
 * create a header and c file for the min heap
 * write an encoder that uses the huffman() to send a sequence of bits (maybe in form of bytes?)
 * write a decoder that uses the data structure thought up ^^ there to decode
 * check for memory leaks by compiling on the zoo
 * add bit fields
*/

//====================Heap Functions===========================
Min_Heap *create_heap(unsigned int capacity){
	Min_Heap *min_heap;

	if (capacity == 0)
		return NULL;

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
	left  = 2 * index + 1;
	right = 2 * index + 2;

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

Min_Heap *create_and_build_heap(unsigned *freq_table){
	Min_Heap *min_heap;
	int i, f, n_used_chars;

	n_used_chars = 0;

	//find the number of used characters to create approp. sized heap
	for (i=0; i < N_CHARS; i++)
		if (freq_table[i])
			n_used_chars++;
			
	if ((min_heap = create_heap(n_used_chars)) == NULL)
		return NULL;

	//iterate through entire freq table, but only add used chars to heap
	for (i = 0, f = 0; f < N_CHARS; f++)
		if (freq_table[f])
			min_heap->array[i++] = create_new_node(f, freq_table[f]);
	
	min_heap->size = n_used_chars;
	build_heap(min_heap);

	return min_heap;
}

Heap_Node *build_huffman_tree(unsigned *freq_table){
	Heap_Node *left_node, *top_node, *right_node, *root_node;
	Min_Heap *min_heap;
	unsigned int combined_freq;

	if ((min_heap = create_and_build_heap(freq_table)) == NULL)
		return NULL;

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

void print_decimal_codes(Code_Word *codewords){
	for (int i=0; i < N_CHARS; i++)
		if (codewords[i].n_bits)
			printf("Letter: %c, Codeword: %d, bits: %d\n", i, codewords[i].code_d, codewords[i].n_bits);
}

//given an array representing a binary number, computes the decimal equivalent
unsigned convert_bin_to_dec(int *bit_array, int last_index){
	unsigned i, decimal_output, multiplier;

	decimal_output = 0;			      //decimal number
	multiplier = 1 << (last_index-1); //2 ^ (last_index - 1)

	for (i=0; i < last_index; i++){
		decimal_output += (bit_array[i]) ? multiplier : 0;
		multiplier /= 2;
	}
	
	return decimal_output;
}

void assign_codes(Heap_Node *root, int *buffer, int b_index, Code_Word *codewords, int p_flag){

	if (root->left){
		buffer[b_index] = ZERO_BIT;
		assign_codes(root->left, buffer, b_index + 1, codewords, p_flag);
	}

	if (root->right){
		buffer[b_index] = ONE_BIT;
		assign_codes(root->right, buffer, b_index + 1, codewords, p_flag);
	}

	if (is_leaf(root)){
		codewords[root->letter].code_d = convert_bin_to_dec(buffer, b_index);
		codewords[root->letter].n_bits = b_index;

		if (p_flag){
			printf("%c: ", root->letter);
			print_arr(buffer, b_index);
		}
	}
}

unsigned int get_huff_tree_height(Heap_Node *root){
	unsigned int l_subtree_h, r_subtree_h;

	if (root == NULL)
		return 0;
	else {
		l_subtree_h = get_huff_tree_height(root->left);
		r_subtree_h = get_huff_tree_height(root->right);

		if (l_subtree_h > r_subtree_h)
			return l_subtree_h + 1;
		else
			return r_subtree_h + 1;
	}
}

Code_Word *create_huffman_code(Heap_Node **root, unsigned *freq_table, int p_flag){
	int *bit_buffer, b_index;
	Code_Word *codewords;
	Heap_Node *huff_tree;

	//generate the huffman tree and save its root to argument
	if ((huff_tree = build_huffman_tree(freq_table)) == NULL)
		return NULL;

	*root = huff_tree;

	//initialize the bit buffer and the array of code word structs
	bit_buffer = calloc(get_huff_tree_height(huff_tree), sizeof(int));
	codewords  = calloc(N_CHARS, sizeof(Code_Word));
	b_index = 0;

	//fill in the array of code word structs
	assign_codes(huff_tree, bit_buffer, b_index, codewords, p_flag);
	
	//print the array of code words if appropriate
	if (p_flag)
		print_decimal_codes(codewords);

	//free the buffer used to store bits
	free(bit_buffer);

	return codewords;
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
