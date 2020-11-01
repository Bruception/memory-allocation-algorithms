#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

heap* create_heap(int capacity, int (*compare)(void* p1, void* p2)) {
  heap* h = (heap*)malloc(sizeof(heap));
  h->nodes = (heap_node**)malloc(sizeof(heap_node*) * capacity);
  h->size = 0;
  h->compare = compare;
  h->capacity = capacity;
  return h;
}

void swim(heap* h) {
  int child_index = h->size - 1;
  int parent_index = (child_index - 1) / 2;
  heap_node* temp;
  while (
    child_index > 0 &&
    h->compare(h->nodes[parent_index]->data, h->nodes[child_index]->data) > 0
  ) {
    temp = h->nodes[child_index];
    h->nodes[child_index] = h->nodes[parent_index];
    h->nodes[parent_index] = temp;
    child_index = parent_index;
    parent_index = (child_index - 1) / 2;
  }
}

void add_to_heap(heap* h, void* p) {
  if (h->size == h->capacity) return;
  heap_node* node = (heap_node*)malloc(sizeof(heap_node));
  node->data = p;
  h->nodes[h->size] = node;
  ++h->size;
  swim(h);
}

void sink(heap* h) {
  int parent_index = 0;
  int min_child_index = (2 * parent_index) + 1;
  int heap_len = h->size - 1;
  heap_node* temp;
  while (min_child_index <= heap_len) {
    if (
      min_child_index < heap_len &&
      h->compare(h->nodes[min_child_index]->data, h->nodes[min_child_index + 1]->data) > 0
    ) {
      ++min_child_index;
    }
    if (h->compare(h->nodes[min_child_index]->data, h->nodes[parent_index]->data) > 0) {
      break;
    }
    temp = h->nodes[min_child_index];
    h->nodes[min_child_index] = h->nodes[parent_index];
    h->nodes[parent_index] = temp;
    parent_index = min_child_index;
    min_child_index = (2 * parent_index) + 1;
  }
}

void* remove_min_from_heap(heap* h) {
  if (h->size == 0) return NULL;
  heap_node* top = h->nodes[0];
  void* top_process = top->data;
  h->nodes[0] = h->nodes[h->size - 1];
  h->nodes[h->size - 1] = top;
  free(h->nodes[h->size - 1]);
  h->nodes[h->size - 1] = NULL;
  --h->size;
  sink(h);
  return top_process;
}

void* get_min_from_heap(heap* h) {
  if (h->size == 0) return NULL;
  return h->nodes[0]->data;
}

void print_heap(heap* h) {
  for(int i = 0; i < h->size; ++i) {
    printf("Node: %p -> Item: %p, ", h->nodes[i], h->nodes[i]->data);
  }
  printf("\n");
}
