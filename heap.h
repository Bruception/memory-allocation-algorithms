// Priority queue implemented with a min heap.
#include "process.h"

#ifndef HEAP_H
#define HEAP_H

typedef struct heap_node_struct {
  process* process;
} heap_node;

typedef struct heap_struct {
  heap_node** nodes;
  int size;
  int capacity;
  // Function pointer to a custom comparison function.
  // Will be useful when we want to test different priority based scheduling algorithms.
  // Comparison function should return > 0 if p1 has higher priority than p2
  int (*compare)(process* p1, process* p2);
} heap;

heap* create_heap(int capacity, int (*compare)(process* p1, process* p2));
void add_to_heap(heap* h, process* p);
process* remove_min_from_heap(heap* h);
process* get_min_from_heap(heap* h);
void print_heap(heap* h);

#endif
