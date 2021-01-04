//API for creating a special minimum heap; Jeacy Espinoza

//container for the minimum heap
typedef struct min_heap {
	unsigned int size;
	unsigned int capacity;
	void **array;
} Min_Heap;

/*
 * For the comparator parameter, int return value as signified:
 * 	  - (< 0) means that the first parameter is less than the second parameter
 *    - (= 0) means that the first parameter is equal to the second parameter
 * 	  - (> 0) means that the first parameter is greater than the second parameter
 * */

Min_Heap *create_heap(unsigned int capacity);

void build_heap(Min_Heap *min_heap, int (*compar)(const void*, const void*));

void *extract_min(Min_Heap *min_heap, int (*compar)(const void*, const void*));

void insert_new_node(Min_Heap *min_heap, void *new_node, int (*compar)(const void*, const void*));

int has_one_element(Min_Heap *min_heap);

void free_min_heap(Min_Heap *min_heap);
