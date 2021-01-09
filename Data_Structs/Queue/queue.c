#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

//used to create a node in the queue
Q_Node *new_node(void *key){
    Q_Node *new = malloc(sizeof(Q_Node));
    new->key = key;
    new->next = NULL;
    return new;
}

Queue *create_queue(void){
    Queue *queue = malloc(sizeof(Queue));
    queue->len = 0;
    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(Queue *queue, void *key){
    Q_Node *new;

    if (!queue)
        return;

    new = new_node(key);
    queue->len++;

    if (!queue->rear)
        queue->front = queue->rear = new;
    else {
        queue->rear->next = new;
        queue->rear = new;
    }
}

void *dequeue(Queue *queue){
    Q_Node *front_node;

    if (!queue)
        return NULL;
    
    front_node = queue->front;
    queue->front = queue->front->next;
    queue->len--;

    if (!queue->front)
        queue->rear = NULL;

    //free the old front of the queue
    free(front_node);
    
    return front_node->key;
}

unsigned len(Queue *queue){
    if (!queue)
        return 0;

    return queue->len;
}

unsigned is_empty(Queue *queue){
    if (!queue)
        return 1;
    return queue->len == 0;
}

void free_queue(Queue *queue){
    Q_Node *front, *temp;

    if (!queue)
        return;
    
    front = queue->front;
    while (front){
        temp = front;
        front = front->next;

        free(temp);
    }

    free(queue);
}
