#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

//Macros
#define INTERNAL_NODE_MARKER '$'
#define EFFICIENT 1

//=============================================================
//comparator function to be used for heap creation and extraction
int comparator(const void *node1, const void *node2){
	if (EFFICIENT)
		return (((Heap_Node*)node1)->freq - ((Heap_Node*)node2)->freq);
	else 
		return (((Heap_Node*)node2)->freq - ((Heap_Node*)node1)->freq);
}

Heap_Node *create_huff_node(char letter, unsigned int freq){
	Heap_Node *new_node;

	new_node = malloc(sizeof(Heap_Node));
	new_node->letter = letter;
	new_node->freq = freq;
	new_node->left = new_node->right = NULL;
	
	return new_node;	
}

int is_leaf(Heap_Node *node){
	return !(node->left) && !(node->right);
}
//=============================================================
Heap *create_and_build_heap(unsigned *freq_table){
	Heap *min_heap;
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
			min_heap->array[i++] = create_huff_node(f, freq_table[f]);
	
	min_heap->size = n_used_chars;
	build_heap(min_heap, comparator);

	return min_heap;
}

Heap_Node *build_huffman_tree(unsigned *freq_table){
	Heap_Node *left_node, *top_node, *right_node, *root_node;
	Heap *min_heap;
	unsigned int combined_freq;

	if ((min_heap = create_and_build_heap(freq_table)) == NULL)
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

void print_arr(int *arr, int n){ 
    int i; 

    for (i = 0; i < n; ++i) 
        printf("%d", arr[i]); 
  
    printf("\n"); 
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

//given a decimal number and a num of bits, returns (str) binary number equivalent
char *convert_dec_to_bin(unsigned decimal_n, unsigned n_bits){
	int i;
	unsigned bin_num[n_bits], j;
	char *str_bin_num;

	str_bin_num = calloc(n_bits + 1, sizeof(char));

	//fill in the integer array "backwards"
	for (i=0; decimal_n > 0; i++, decimal_n /= 2)
		bin_num[i] = decimal_n % 2;
	
	//fill the rest of the array with 0's
	for (; i < n_bits; i++)
		bin_num[i] = ZERO_BIT;
	
	//fill the string representation
	for (i=n_bits-1, j=0; i >= 0; i--, j++)
		str_bin_num[j] = (bin_num[i]) ? '1' : '0';
	
	str_bin_num[n_bits] = '\0';
	
	return str_bin_num;
}

int code_exists(Code_Word codeword){
	return codeword.n_bits != 0;
}

float find_avg_len(Code_Word *codewords){
	unsigned i, num_sum, denom;
	float avg_len;

	avg_len = num_sum = denom = 0;

	//find the number of characters
	for (i = 0; i < N_CHARS; i++)
		if (code_exists(codewords[i])){
			num_sum += codewords[i].n_bits;
			denom++;
		}
	
	if (denom == 0)
		return avg_len;
	else
		return (num_sum) / denom;
}

//dumps letter|frequency|code(2)|code(10)|n_bits to a file
void dump_input_info(Code_Word *codewords, unsigned *freq_table){
	unsigned code, n_bits, i;
	char *str_bin_num;
	
	printf("Average Length of codewords = %0.2f\n", find_avg_len(codewords));
	printf("Letter | Frequency | Code (2) | Code (10) | Num of Bits\n");
	for (i=0; i < N_CHARS; i++)
		
		if (code_exists(codewords[i])){
			n_bits = codewords[i].n_bits;
			code = codewords[i].code_d;
			
			str_bin_num = convert_dec_to_bin(code, n_bits);
			
			printf("%c | %d | %s | %d | %d\n", 
				     i, freq_table[i], str_bin_num, code, n_bits);

			free(str_bin_num);
		}
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
	assign_codes(huff_tree, bit_buffer, b_index, codewords);
	
	//print info on input and average code length if appropriate
	if (p_flag)
		dump_input_info(codewords, freq_table);

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
