//API for creating a special minimum heap; Jeacy Espinoza

/* _____________________________________________________________________________________
 * |______________________Instructions To Use Functions Below__________________________|
 * | For the comparator parameter, int return value meaning:						   |	
 * |	  1. (< 0) means that the first parameter is less than the second parameter	   |	
 * |      2. (= 0) means that the first parameter is equal to the second parameter	   |
 * |	  3. (> 0) means that the first parameter is greater than the second parameter |
 * |																				   |
 * | Thus, write your comparator as follows:										   |	
 * |		 to create a minimum heap, comparator(node1, node2) <  0				   | 
 * |								 and										       |
 * |	     to create a maximum heap, comparator(node1, node2) >= 0				   |
 * _____________________________________________________________________________________*/

//container for the minimum heap
typedef struct min_heap {
	unsigned int size;
	unsigned int capacity;
	void **array;
} Min_Heap;

Min_Heap *create_heap(unsigned int capacity);

void build_heap(Min_Heap *min_heap, int (*compar)(const void*, const void*));

void *extract_min(Min_Heap *min_heap, int (*compar)(const void*, const void*));

void insert_new_node(Min_Heap *min_heap, void *new_node, int (*compar)(const void*, const void*));

int has_one_element(Min_Heap *min_heap);

void free_min_heap(Min_Heap *min_heap);
