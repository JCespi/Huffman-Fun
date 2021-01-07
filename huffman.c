#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
#include "table.h"

//Macros
#define EFFICIENT 1

//=================Huffman-Specific_Node========================
//comparator function to be used for heap creation and extraction
int comparator(const void *node1, const void *node2){
	if (EFFICIENT)
		return (((Heap_Node*)node1)->freq - ((Heap_Node*)node2)->freq);
	else 
		return (((Heap_Node*)node2)->freq - ((Heap_Node*)node1)->freq);
}

//returns a node with the given parameters. user's responsibility to free
Heap_Node *create_huff_node(char letter, unsigned int freq){
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

//used to interface with functions in table.h to produce a table
void dump_input_info(Code_Word *codewords, unsigned *freq_table){
	unsigned i, j, n_cols;
	char **row_strs;
	FILE *fp;

	//open file to dump table to
	fp = fopen("table.txt", "w");

	//use approrpiate setup functions for table
	n_cols = 5;
	unsigned col_lens[] = {8, 6, 10, 11, 7};
	set_dump_file(fp);
	set_col_lens(col_lens, n_cols);

	//print the average length of a code
	fprintf(fp, "Average Length of codewords = %0.2f\n", find_avg_len(codewords));

	//use table functions to print a title
	char *col_titles[] = {"Letter", "Freq", "Code (2)", "Code (10)", "Nbits"};
	print_pretty_header(col_titles);

	//container to hold the strs to be passed to table.h function
	row_strs = malloc(sizeof(char*));

	//use table functions to print the rows
	for (i=0; i < N_CHARS; i++){
		if (code_exists(codewords[i])){
			//generate strings and place in buffer
			asprintf(&row_strs[0], "%d", i);
			asprintf(&row_strs[1], "%d", freq_table[i]);
			row_strs[2] = convert_dec_to_bin(codewords[i].code_d, codewords[i].n_bits);
			asprintf(&row_strs[3], "%d", codewords[i].code_d);
			asprintf(&row_strs[4], "%d", codewords[i].n_bits);

			//print the string array using table.h
			print_pretty_row(row_strs, 1);

			//free the recently created strings
			for (j=0; j < n_cols; j++)
				free(row_strs[j]);
		}
	}

	//print a footer to close the table
	print_pretty_footer();

	//free container(s)
	free(row_strs);

	//close dump file
	fclose(fp);
}
//=================Huffman-Construction=========================
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
//==============================================================
