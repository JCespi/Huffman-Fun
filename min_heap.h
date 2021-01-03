//API for creating a special minimum heap; Jeacy Espinoza

//a node in a huffman tree (N nodes total)
typedef struct min_heap_node {
	unsigned int letter;		    //1  byte
	unsigned int freq;				//4  bytes
	struct min_heap_node *left;		//8  bytes
	struct min_heap_node *right;	//8  bytes
} Heap_Node;						//25 bytes -> 21*N bytes total

//container for the minimum heap
typedef struct min_heap {
	unsigned int size;
	unsigned int capacity;
	Heap_Node **array;
} Min_Heap;

/*
 * For the comparator parameter, int return value as signified:
 * 	  - (< 0) means that the first parameter is less than the second parameter
 *    - (= 0) means that the first parameter is equal to the second parameter
 * 	  - (> 0) means that the first parameter is greater than the second parameter
 * */

Min_Heap *create_heap(unsigned int capacity);

void build_heap(Min_Heap *min_heap, int (*compar)(const void*, const void*));

Heap_Node *create_new_node(char letter, unsigned int freq);

Heap_Node *extract_min(Min_Heap *min_heap, int (*compar)(const void*, const void*));

void insert_new_node(Min_Heap *min_heap, Heap_Node *new_node);

int has_one_element(Min_Heap *min_heap);

int is_leaf(Heap_Node *node);

void free_min_heap(Min_Heap *min_heap);
