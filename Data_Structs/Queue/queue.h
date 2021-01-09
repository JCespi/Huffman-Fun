//a queue node (linked list node)
typedef struct q_node {
    void *key;
    struct q_node *next;
} Q_Node;

//the linked list queue
typedef struct queue {
    unsigned len;
    Q_Node *front;
    Q_Node *rear;
} Queue;

//creates an empty queue
Queue *create_queue(void);

//adds a given key to the end of the queue
void enqueue(Queue *queue, void *key);

//removes and returns key at the front of the queue
void *dequeue(Queue *queue);

//returns whether the queue is empty
unsigned is_empty(Queue *queue);

//returns the number of items in the queue
unsigned len(Queue *queue);

//frees the queue
void free_queue(Queue *queue);
