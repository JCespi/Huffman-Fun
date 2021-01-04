#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

//=============================================================
Min_Heap *create_heap(unsigned int capacity){
	Min_Heap *min_heap;

	if (capacity == 0)
		return NULL;

	min_heap = malloc(sizeof(Min_Heap));
	min_heap->size = 0;
	min_heap->capacity = capacity;
	min_heap->array = malloc(sizeof(void*) * capacity);

	return min_heap;
}

void swap_heap_nodes(void **n1, void **n2){
	void *temp_node;

	temp_node = *n1;
	*n1 = *n2;
	*n2 = temp_node;
}

void heapify(Min_Heap *min_heap, unsigned index, int (*compar)(const void*, const void*)){
	unsigned int smallest, left, right;
	
	smallest = index;
	left  = 2 * index + 1;
	right = 2 * index + 2;
	
	if (left < min_heap->size && compar(min_heap->array[left], min_heap->array[smallest]) < 0)
		smallest = left;
	
	if (right < min_heap->size && compar(min_heap->array[right], min_heap->array[smallest]) < 0)
		smallest = right;
	
	if (smallest != index){
		swap_heap_nodes(&min_heap->array[smallest], &min_heap->array[index]);
		heapify(min_heap, smallest, compar);
	}
}

int has_one_element(Min_Heap *min_heap){
	return min_heap->size == 1;
}

void *extract_min(Min_Heap *min_heap, int (*compar)(const void*, const void*)){
	void *min_node;

	min_node = min_heap->array[0];								//pop off the root
	min_heap->array[0] = min_heap->array[min_heap->size - 1];	//insert (large) leaf as root
	min_heap->size--;											//decrease heap size
	heapify(min_heap, 0, compar);							    //ripple the new root down

	return min_node;
}

void insert_new_node(Min_Heap *min_heap, void *new_node, int (*compar)(const void*, const void*)){
	int i;

	min_heap->size++;
	i = min_heap->size - 1;
	
	while (i && compar(new_node, min_heap->array[(i - 1) / 2]) < 0){
		min_heap->array[i] = min_heap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	min_heap->array[i] = new_node;
}
	
void build_heap(Min_Heap *min_heap, int (*compar)(const void*, const void*)){
	int i, n;

	n = min_heap->size - 1;
	i = (n - 1) /2;

	while (i >= 0){
		heapify(min_heap, i, compar);
		i--;
	}
}

void free_min_heap(Min_Heap *min_heap){
	free(min_heap->array);
	free(min_heap);
}
//=============================================================
