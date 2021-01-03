#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"

//=============================================================
Min_Heap *create_heap(unsigned int capacity){
	Min_Heap *min_heap;

	if (capacity == 0)
		return NULL;

	min_heap = malloc(sizeof(Min_Heap));
	min_heap->size = 0;
	min_heap->capacity = capacity;
	min_heap->array = malloc(sizeof(Heap_Node*) * capacity);

	return min_heap;
}

Heap_Node *create_new_node(char letter, unsigned int freq){
	Heap_Node *new_node;

	new_node = malloc(sizeof(Heap_Node));
	new_node->letter = letter;
	new_node->freq = freq;
	new_node->left = new_node->right = NULL;
	
	return new_node;	
}

void swap_heap_nodes(Heap_Node **n1, Heap_Node **n2){
	Heap_Node *temp_node;

	temp_node = *n1;
	*n1 = *n2;
	*n2 = temp_node;
}

void heapify(Min_Heap *min_heap, unsigned int index){
	unsigned int smallest, left, right;
	
	smallest = index;
	left  = 2 * index + 1;
	right = 2 * index + 2;

	if (left < min_heap->size && min_heap->array[left]->freq < min_heap->array[smallest]->freq)
		smallest = left;
	
	if (right < min_heap->size && min_heap->array[right]->freq < min_heap->array[smallest]->freq)
		smallest = right;
	
	if (smallest != index){
		swap_heap_nodes(&min_heap->array[smallest], &min_heap->array[index]);
		heapify(min_heap, smallest);
	}
}

int has_one_element(Min_Heap *min_heap){
	return min_heap->size == 1;
}

Heap_Node *extract_min(Min_Heap *min_heap){
	Heap_Node *min_node;

	min_node = min_heap->array[0];								//pop off the root
	min_heap->array[0] = min_heap->array[min_heap->size - 1];	//insert (large) leaf as root
	min_heap->size--;											//decrease heap size
	heapify(min_heap, 0);										//ripple the new root down

	return min_node;
}

void insert_new_node(Min_Heap *min_heap, Heap_Node *new_node){
	int i;

	min_heap->size++;
	i = min_heap->size - 1;

	while (i && new_node->freq < min_heap->array[(i - 1) / 2]->freq){
		min_heap->array[i] = min_heap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	min_heap->array[i] = new_node;
}

int is_leaf(Heap_Node *node){
	return !(node->left) && !(node->right);
}
	
void build_heap(Min_Heap *min_heap){
	int i, n;

	n = min_heap->size - 1;
	i = (n - 1) /2;

	while (i >= 0){
		heapify(min_heap, i);
		i--;
	}
}

void free_min_heap(Min_Heap *min_heap){
	free(min_heap->array);
	free(min_heap);
}
//=============================================================
