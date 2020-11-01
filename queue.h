// FIFO queue implemented with a linked list.

#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_node_struct {
  struct queue_node_struct* next;
  struct queue_node_struct* prev;
  void* data;
} queue_node;

typedef struct queue_struct {
  queue_node* head;
  queue_node* tail;
  int size;
} queue;

queue* create_queue();
void add_to_queue(queue* q, void* data);
void* remove_from_queue(queue* q);
void print_queue(queue* q);
void* get_front(queue* q);

#endif
