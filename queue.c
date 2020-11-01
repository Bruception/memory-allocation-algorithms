#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_queue_node(queue_node* node, queue_node* next, queue_node* prev, void* data) {
  node->next = next;
  node->prev = prev;
  node->data = data;
}

queue* create_queue() {
  queue* q = (queue*)malloc(sizeof(queue));
  q->size = 0;
  // Dummy head and tail.
  q->head = (queue_node*)malloc(sizeof(queue_node));
  q->tail = (queue_node*)malloc(sizeof(queue_node));
  init_queue_node(q->head, q->tail, NULL, NULL);
  init_queue_node(q->tail, NULL, q->head, NULL);
  return q;
}

// Adds to the back of the queue.
void add_to_queue(queue* q, void* data) {
  queue_node* new_node = (queue_node*)malloc(sizeof(queue_node));
  queue_node* old_tail = q->tail->prev;
  q->tail->prev = new_node;
  new_node->prev = old_tail;
  new_node->next = q->tail;
  old_tail->next = new_node;
  new_node->data = data;
  ++q->size;
}

// Removes from the front of the queue.
void* remove_from_queue(queue* q) {
  queue_node* old_head = q->head->next;
  if (old_head == q->tail) return NULL;
  q->head->next = old_head->next;
  q->head->next->prev = q->head;
  void* p = old_head->data;
  free(old_head);
  --q->size;
  return p;
}

// Get first element in quque
void* get_front(queue* q) {
  if (q->size == 0) return NULL;
  return q->head->next->data;
}

// Print queue from head to tail.
void print_queue(queue* q) {
  queue_node* current = q->head->next;
  printf("Head: %p, Next: %p, Prev: %p\n", q->head, q->head->next, q->head->prev);
  while(current != NULL && current != q->tail) {
    printf("\tNode: %p, Next: %p, Prev: %p\n", current, current->next, current->prev);
    printf("\t\tData: %p\n", current->data);
    current = current->next;
  }
  printf("Tail: %p, Next: %p, Prev: %p\n\n", q->tail, q->tail->next, q->tail->prev);
}
