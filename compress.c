#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main(int argc, char **argv){
    Heap_Node *root;
    Code_Word *codewords;

    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' }; 
    unsigned freq[] = { 5, 9, 12, 13, 16, 45 }; 
	int size = 6;

    codewords = create_huffman_code(&root, arr, freq, size, 1);

    free_huffman_tree(root);
	free(codewords);

    return 0;
}
