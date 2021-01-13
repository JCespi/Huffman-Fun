#define _GNU_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compress.h"
#include "Table/table.h"
#include "Huffman/huffman.h"
#include "Transmit/transmit.h"
#include "Data_Structs/Queue/queue.h"

//Macros
#define BIT      1  //size of a bit  (in bits)
#define BYTE     8  //size of a byte (in bits)
#define LEAF     1  //signifies a leaf node
#define INTERNAL 0  //signifies an internal node
#define N_CHARS 256 //the number of characters (0 - 255)
#define EOC     256 //signifies a 256th character that signals an end to transmission

//=============================Dump_Functions=================================
//helper function that uses table print functions
void process_node(Heap_Node *node, Code_Word *codewords, unsigned n_cols){
    unsigned i, code, n_bits;
    char letter, **row_strs;
    float freq;

    //container to hold the strs to be passed to table.h function
	row_strs = malloc(sizeof(char*) * n_cols);

    if (is_leaf(node) && node->letter != EOC){
        //generate data to be converted to strings
        letter = node->letter;
        freq = node->freq;
        code = codewords[(int)letter].code_d;
        n_bits = codewords[(int)letter].n_bits;
        
        //place data into buffer to use table function
        row_strs[0] = convert_letter_to_str(letter);
        asprintf(&row_strs[1], "%0.3f", freq);
        row_strs[2] = convert_dec_to_bin(code, n_bits);
        asprintf(&row_strs[3], "%d", code);
        asprintf(&row_strs[4], "%d", n_bits);

        //print the string array using table.h
        print_pretty_row(row_strs, 1);

        //free the recently created strings
        for (i=0; i < n_cols; i++)
            free(row_strs[i]);
    }

    free(row_strs);
}

//frees a string array
void free_str_container(char **container, int n){
    int i;

    for (i=0; i < n; i++)
        free(container[i]);

    free(container);
}

//initialize tile and column length arrays
void init_metadata(Code_Word *codewords, char **col_titles, unsigned *col_lens, unsigned n_cols, unsigned n){
    unsigned i, min_bit_col, min_dig_col, padding;

    padding = 2; //(padding)
    min_bit_col = find_max(codewords, n, 1) + padding;          //minimum size of columns that depend on bits
    min_dig_col = get_n_digits(find_max(codewords, n, 0)) + padding; //min size of cols that depend on digits

    //fill in column titles string array as well as the column lengths array
    char *str_arr[] = {"Letter", "Freq (%)", "Code (2)", "Code (10)", "Nbits"};
    for (i=0; i < n_cols; i++){
        col_titles[i] = strdup(str_arr[i]);
        col_lens[i]   = strlen(col_titles[i]) + padding;
    }
        
   //update the column lengths that depend upon the data in those columns (choose larger value)
    col_lens[2] = (min_bit_col > col_lens[2]) ? min_bit_col : col_lens[2];
    col_lens[3] = (min_dig_col > col_lens[3]) ? min_dig_col : col_lens[3];
    col_lens[4] = (min_bit_col > col_lens[4]) ? min_bit_col : col_lens[4];
}

//used to interface with functions in table.h to produce a table
void dump_input_info(char *dmp_file, Heap_Node *root, Code_Word *codewords, float comp, unsigned n){
	unsigned n_cols, *col_lens;
	char *str_buffer, **col_titles;
    Heap_Node *min_node;
    Queue *queue;
	FILE *fp;

	//open file to dump table to
	if (!(fp = fopen(dmp_file, "w")))
        return;

	//initialize the column lengths metadata and str titles
	n_cols = 5;
    col_lens = calloc(n_cols, sizeof(int));
    col_titles = malloc(sizeof(char*) * n_cols);
    init_metadata(codewords, col_titles, col_lens, n_cols, n);

    //call on appropriate functions to set up the table
	set_dump_file(fp);
	set_col_lens(col_lens, n_cols);

	//print the average length of a code and the amount of compression
    asprintf(&str_buffer, "Compression = %0.2f%%", comp);
    print_pretty_centered(str_buffer);
    free(str_buffer);
	asprintf(&str_buffer, "Average Length of codewords = %0.2f", find_avg_len(codewords, n));
	print_pretty_centered(str_buffer);
    free(str_buffer);

	//use table functions to print a title
	print_pretty_header(col_titles);
    free_str_container(col_titles, n_cols);

    //perform a bfs and use table functions to print the rows in order of n_bits
    queue = create_queue();         //create queue
    enqueue(queue, (void*)root);    //initialize it with the root

    while (!is_empty(queue)){
        min_node = (Heap_Node*)dequeue(queue);

        process_node(min_node, codewords, n_cols);

        if (min_node->left)
            enqueue(queue, (void*)(min_node->left));

        if (min_node->right)
            enqueue(queue, (void*)(min_node->right));
    }

	//print a footer to close the table
	print_pretty_footer();

	//free container(s)
    free_queue(queue);
    free(col_lens);

	//close dump file
	fclose(fp);
}

//=============================*code_Functions================================
//reads chars from stdin, building a frequency table. fills variable passed by reference
unsigned build_freq_table(float **freq_table, unsigned table_size){
    unsigned i, tot_n_chars, ch;
    float smallest_freq, *output_arr;

    tot_n_chars = 0;
    smallest_freq = 100.0;

    *freq_table = calloc(table_size, sizeof(float));
    output_arr = *freq_table;

    //receive characters from stdin
    while ((ch = getchar()) != EOF){
        output_arr[ch]++;
        tot_n_chars++;
    }
    
    //update the frequencies to be percentages
    for (i=0; i < table_size; i++){
        if (output_arr[i]){
            output_arr[i] = (output_arr[i] / tot_n_chars) * 100;

            if (output_arr[i] < smallest_freq)
                smallest_freq = output_arr[i];
        }
    }

    //add the special "EOF" character to the table with the smallest frequency
    output_arr[EOC] = smallest_freq;
        
    //return the total number of bits in the input file
    return tot_n_chars * BYTE;
}

//computes amount of compression b/w output stream and input stream
float compute_compression(unsigned n_bits_in, unsigned n_bits_out){
    return (((float)n_bits_in - (float)n_bits_out) / (float)n_bits_in) * 100;
}

//a low bit signifies an internal node.
//a high bit followed by a low bit followed by a byte signifies leaf node and letter
//a high bit followed by a high bit signifies an EOC leaf node
//returns num of output bytes
unsigned send_huff_tree(Heap_Node *root){
    if (is_leaf(root)){
        put_bits(BIT, LEAF);
        put_bits(BIT, root->letter == EOC);

        if (root->letter != EOC)
            put_bits(BYTE, root->letter);

        return BIT + BIT + BYTE;
    } else {
        put_bits(BIT, INTERNAL);
        return BIT + send_huff_tree(root->left) + send_huff_tree(root->right);
    }
}

//given a char, put_bits() the char and returns num of written bits
unsigned send_codeword(Code_Word *codeword){
    put_bits(codeword->n_bits, codeword->code_d);
    return codeword->n_bits;
}

//read file to form frequency table, generate huffman code and tree, send tree and codewords
void encode(int dump, char *dmp_file){
    unsigned n_bits_in, n_bits_out, ch, table_size;
    Code_Word *codewords;
    float *freq_table;
    Heap_Node *root;

    //initialize variables to compute compression
    n_bits_in = n_bits_out = 0;

    //build the frequency table
    table_size = N_CHARS + 1;
    n_bits_in += build_freq_table(&freq_table, table_size);
    
    //create the huffman code
    codewords = create_huffman_code(&root, freq_table, table_size);

    //send the huffman tree
    n_bits_out += send_huff_tree(root);

    //rewind stdin
    rewind(stdin);

    //transmit the codewords
    while ((ch = getchar()) != EOF)
        n_bits_out += send_codeword(&codewords[ch]);
    
    //output the codeword that signals the end
    n_bits_out += send_codeword(&codewords[EOC]);
        
    //flush any bits caught in buffer
    n_bits_out += flush_bits();

    //dump the tree by performing breadth-first search
    if (dump)
        dump_input_info(dmp_file, root, codewords, compute_compression(n_bits_in, n_bits_out), table_size);

    //free the tree, the codeword table, and the frequency table
    free_huffman_tree(root);
    free(freq_table);
    free(codewords);
}

//receives stream of bits sent from encode() to construct tree
Heap_Node *receive_huff_tree(void){
    Heap_Node *root;

    if (get_bits(BIT) == INTERNAL){   //internal node
        root = create_huff_node(INTERNAL_NODE_MARKER, 0);
        root->left = receive_huff_tree();
        root->right = receive_huff_tree();
        return root;
    } else {                          //leaf node
        if (get_bits(BIT))
            return create_huff_node(EOC, 0);
        else
            return create_huff_node(get_bits(BYTE), 0);
    }
}

//read the huffman tree, read codewords and output corresponding letters
void decode(void){
    Heap_Node *root, *ptr;
    long bit;

    //receive the tree from stdin
    root = receive_huff_tree();
    ptr = root;
    
    while ((bit = get_bits(BIT)) != EOF){
        //move the pointer appropriately according to bit received (0->left, 1->right)
        ptr = (bit == 0) ? ptr->left : ptr->right;

        //a leaf node (a letter or EOC) found
        if (is_leaf(ptr)){
            if (ptr->letter == EOC)
                break;

            putchar(ptr->letter);
            ptr = root; //reset to root of tree
        }
    }

    //free the tree
    free_huffman_tree(root);
}
//============================================================================
