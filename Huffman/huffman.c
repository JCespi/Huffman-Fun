#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
#include "../Data_Structs/Heap/heap.h"

//Macros
#define EFFICIENT 1    //a macro that can be set to 0 to assign less frequent words shorter codes

//=================Huffman-Specific_Node========================
//comparator function to be used for heap creation and extraction
int comparator(const void *node1, const void *node2){
	if (EFFICIENT)
		return (((Heap_Node*)node1)->freq - ((Heap_Node*)node2)->freq);
	else 
		return (((Heap_Node*)node2)->freq - ((Heap_Node*)node1)->freq);
}

//returns a node with the given parameters. user's responsibility to free
Heap_Node *create_huff_node(unsigned letter, float freq){
	Heap_Node *new_node;

	new_node = malloc(sizeof(Heap_Node));
	new_node->letter = letter;
	new_node->freq = freq;
	new_node->left = new_node->right = NULL;
	
	return new_node;	
}

//determines whether the node has a letter/is a leaf
int is_leaf(Heap_Node *node){
	return !(node->left) && !(node->right);
}

//=================Huffman-Related_Info=========================
int code_exists(Code_Word codeword){
	return codeword.n_bits != 0;
}

//finds the average length of the code
float find_avg_len(Code_Word *codewords, unsigned table_size){
	unsigned i, denom;
	float num_sum, avg_len;

	avg_len = num_sum = denom = 0;

	//find the number of characters
	for (i = 0; i < table_size; i++)
		if (code_exists(codewords[i])){
			num_sum += codewords[i].n_bits;
			denom++;
		}
	
	if (denom == 0)
		return avg_len;
	else
		return (num_sum) / denom;
}

//finds the maximum number of bits among codewords or maximum codeword
unsigned find_max(Code_Word *codewords, unsigned table_size, int n_bits){
	unsigned i, max_value, competing_value;

	if (!codewords)
		return 0;

	max_value = (n_bits) ? codewords[0].n_bits : codewords[0].code_d;

	for (i=1; i < table_size; i++){
		competing_value = (n_bits) ? codewords[i].n_bits : codewords[i].code_d;

		if (competing_value > max_value)
			max_value = competing_value;
	}
	
	return max_value;
}

//=================Huffman-Construction=========================
Heap *create_and_build_heap(float *freq_table, unsigned table_size){
	Heap *min_heap;
	int i, f, n_used_chars;

	n_used_chars = 0;

	//find the number of used characters to create approp. sized heap
	for (i=0; i < table_size; i++)
		if (freq_table[i])
			n_used_chars++;
			
	if ((min_heap = create_heap(n_used_chars)) == NULL)
		return NULL;

	//iterate through entire freq table, but only add used chars to heap
	for (i = 0, f = 0; f < table_size; f++)
		if (freq_table[f])
			min_heap->array[i++] = create_huff_node(f, freq_table[f]);
	
	min_heap->size = n_used_chars;
	build_heap(min_heap, comparator);

	return min_heap;
}

Heap_Node *build_huffman_tree(float *freq_table, unsigned table_size){
	Heap_Node *left_node, *top_node, *right_node, *root_node;
	Heap *min_heap;
	float combined_freq;

	if ((min_heap = create_and_build_heap(freq_table, table_size)) == NULL)
		return NULL;

	while (!has_one_element(min_heap)){
		left_node  = (Heap_Node*)extract_min(min_heap, comparator);
		right_node = (Heap_Node*)extract_min(min_heap, comparator);

		combined_freq = left_node->freq + right_node->freq;
		top_node = create_huff_node(INTERNAL_NODE_MARKER, combined_freq);
		top_node->left = left_node;
		top_node->right = right_node;

		insert_new_node(min_heap, (void*)top_node, comparator);
	}

	root_node = min_heap->array[0];			    //extract the huffman tree
	free_min_heap(min_heap);					//free min heap container

	return root_node;
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

void assign_codes(Heap_Node *root, int *buffer, int b_index, Code_Word *codewords){
	if (!root)
		return;

	if (root->left){
		buffer[b_index] = ZERO_BIT;
		assign_codes(root->left, buffer, b_index + 1, codewords);
	}

	if (root->right){
		buffer[b_index] = ONE_BIT;
		assign_codes(root->right, buffer, b_index + 1, codewords);
	}

	if (is_leaf(root)){
		codewords[root->letter].code_d = convert_bin_to_dec(buffer, b_index);
		codewords[root->letter].n_bits = b_index;
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

Code_Word *create_huffman_code(Heap_Node **root, float *freq_table, unsigned table_size){
	int *bit_buffer, b_index;
	Code_Word *codewords;
	Heap_Node *huff_tree;

	//generate the huffman tree and save its root to argument
	if ((huff_tree = build_huffman_tree(freq_table, table_size)) == NULL)
		return NULL;

	*root = huff_tree;

	//initialize the bit buffer and the array of code word structs
	bit_buffer = calloc(get_huff_tree_height(huff_tree), sizeof(int));
	codewords  = calloc(table_size, sizeof(Code_Word));
	b_index = 0;

	//fill in the array of code word structs
	assign_codes(huff_tree, bit_buffer, b_index, codewords);
	
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
//==============================================================
