#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compress.h"
#include "huffman.h"
#include "transmit.h"
#include "table.h"
#include "queue.h"

/* TO-DO
 * find a way to reduce the magnitudes of the frequencies (a percentage possibly)
 * check for memory leaks by compiling on the zoo
 * add bit fields
 * add more detailed comments to header file functions. (@param, @return)
 * add function to find percentage of compression
 * add error handling to main()
 * maybe make a main.c that has the main() that calls on encode and decode (defined in some .h)
 * add a little function that prints a frequency plot
 * for dumping the table:
 *      - handle multiple sources of redirection with a default
 *      - add functions to find appropriate sizes for column widths
 *      - change stack memory to heap memory for column widths AND column titles
*/

//start of a function to reduce magnitudes of frequencies (why though?)
void squish_freqs(unsigned *freq_table){
    unsigned small_freq, i;

    small_freq = freq_table[0];
    for (i=1; i < N_CHARS; i++)
        if (freq_table[i] && freq_table[i] < small_freq)
            small_freq = freq_table[i];
    for (i=0; i < N_CHARS; i++)
        freq_table[i] /= small_freq;
}

//a high bit followed by a byte signifies a leaf node with following letter
void send_huff_tree(Heap_Node *root){
    if (is_leaf(root)){
        put_bits(1, ONE_BIT);
        put_bits(8, root->letter);
    } else {
        put_bits(1, ZERO_BIT);
        send_huff_tree(root->left);
        send_huff_tree(root->right);
    }
}

//helper function that uses table print functions
void process_node(Heap_Node *node, Code_Word *codewords, unsigned n_cols){
    unsigned i, freq, code, n_bits;
    char letter, **row_strs;

    //container to hold the strs to be passed to table.h function
	row_strs = malloc(sizeof(char*) * n_cols);

    if (is_leaf(node)){
        //generate data to be converted to strings
        letter = node->letter;
        freq = node->freq;
        code = codewords[(int)letter].code_d;
        n_bits = codewords[(int)letter].n_bits;
        
        //place data into buffer to use table function
        row_strs[0] = convert_letter_to_str(letter);
        asprintf(&row_strs[1], "%d", freq);
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

//used to interface with functions in table.h to produce a table
void dump_input_info(Heap_Node *root, Code_Word *codewords, unsigned *freq_table){
	unsigned n_cols;
	char *str_buffer;
    Heap_Node *min_node;
    Queue *queue;
	FILE *fp;

	//open file to dump table to
	fp = fopen("table.txt", "w");

	//use approrpiate setup functions for table
	n_cols = 5;
	unsigned col_lens[] = {8, 6, 10, 11, 7};
	set_dump_file(fp);
	set_col_lens(col_lens, n_cols);

	//print the average length of a code
	asprintf(&str_buffer, "Average Length of codewords = %0.2f", find_avg_len(codewords));
	print_pretty_centered(str_buffer);

	//use table functions to print a title
	char *col_titles[] = {"Letter", "Freq", "Code (2)", "Code (10)", "Nbits"};
	print_pretty_header(col_titles);

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
	free(str_buffer);

	//close dump file
	fclose(fp);
}

//read file to form frequency table, generate huffman code and tree, send tree and codewords
void encode(int dump){
    Heap_Node *root;
    Code_Word *codewords;
    unsigned *freq_table, ch;

    freq_table = calloc(N_CHARS, sizeof(int));

    //build the frequency table
    while ((ch = getchar()) != EOF)
        freq_table[ch]++;

    //create the huffman code
    codewords = create_huffman_code(&root, freq_table);

    //send the huffman tree
    send_huff_tree(root);

    //dump the tree by eating the tree
    if (dump)
        dump_input_info(root, codewords, freq_table);

    //rewind stdin and transmit the codewords
    rewind(stdin);
    while ((ch = getchar()) != EOF)
        put_bits(codewords[ch].n_bits, codewords[ch].code_d);

    //flush any bits caught in buffer
    flush_bits();

    //free the tree, the codeword table, and the frequency table
    free_huffman_tree(root);
    free(freq_table);
    free(codewords);
}

//receives stream of bits sent from encode() to construct tree
Heap_Node *receive_huff_tree(void){
    Heap_Node *root;

    if (get_bits(1) == ZERO_BIT){   //internal node
        root = create_huff_node(INTERNAL_NODE_MARKER, 0);
        root->left = receive_huff_tree();
        root->right = receive_huff_tree();
        return root;
    } else {                        //leaf node
        return create_huff_node(get_bits(8), 0);
    }
}

//read the huffman tree, read codewords and output corresponding letters
void decode(void){
    Heap_Node *root, *ptr;
    long bit;

    root = receive_huff_tree();
    ptr = root;
    
    while ((bit = get_bits(1)) != EOF){
        //move the pointer appropriately
        ptr = (bit == ZERO_BIT) ? ptr->left : ptr->right;

        //a leaf node (a letter) found
        if (is_leaf(ptr)){
            putchar(ptr->letter);
            ptr = root; //reset to root of tree
        }
    }

    free_huffman_tree(root);
}

int main(int argc, char **argv){
    char *path, *exectuable;
    int i, dump;
    
    //get executable name
    path = argv[0];
    exectuable = &path[strlen(path) - 6];

    if (strcmp("decode", exectuable) == 0){
        if (argc > 1)
            DIE("%s", "decode: accepts no options");

        decode();
    } else {
        dump = 0;   //default dump option is false
        i = 1;

        while (i < argc){
            if (strcmp(argv[i], "-d") == 0){
                dump = 1;
                i++;
            } else 
                DIE("%s", "encode: unknown flag");
        }

        encode(dump);
    }

    return 0;
}
