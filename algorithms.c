#include <stdlib.h>
#include <stdio.h>
#include "algorithms.h"
#include "heap.h"
#include "queue.h"
#include "process.h"

int compare_entry_time(void* p1, void* p2) {
    process* pp1 = (process*)p1;
    process* pp2 = (process*)p2;
    return pp1->entry_time - pp2->entry_time;
}

typedef struct free_list_node_struct {
    struct free_list_node_struct* next;
    unsigned int size;
    unsigned short empty;
    process* p;
} free_list_node;

void print_list(free_list_node* fl) {
    free_list_node* current = fl;
    while (current != NULL) {
        printf("(size: %d", current->size);
        if (!current->empty) {
            printf(", ID: %d", current->p->id);
        }
        printf(") -> ");
        current = current->next;
    }
    printf("\n");
}

void merge_empty_nodes(free_list_node* fl) {
    free_list_node* current = fl;
    while (current != NULL) {
        free_list_node* next = current->next;
        while (current->empty && next != NULL && next->empty) {
            current->size += next->size;
            current->next = next->next;
            free(next);
            next = current->next;
        }
        current = current->next;
    }
}

int remove_exited_processes(free_list_node* fl, int current_time) {
    int exited = 0;
    free_list_node* current = fl;
    while (current != NULL) {
        process* p = current->p;
        if (current->p != NULL && (p->actual_entry_time + p->duration) <= current_time) {
            current->empty = 1;
            current->p = NULL;
            ++exited;
        }
        current = current->next;
    }
    merge_empty_nodes(fl);
    return exited;
}

perf_data* create_perf_data() {
    perf_data* pf = (perf_data*)malloc(sizeof(perf_data));
    pf->average_external_frag = 0;
    pf->failed_allocations = 0;
    pf->iter_steps = 0;
    return pf;
}

free_list_node* create_free_list(int block_size) {
    free_list_node* fl = (free_list_node*)malloc(sizeof(free_list_node));
    fl->size = block_size;
    fl->next = NULL;
    fl->empty = 1;
    fl->p = NULL;
    return fl;
}

heap* init_entry_time_heap(process** processes, int process_list_size) {
    heap* entry_time_heap = create_heap(process_list_size, &compare_entry_time);
    for (int i = 0; i < process_list_size; ++i) {
        add_to_heap(entry_time_heap, (void*)processes[i]);
    }
    return entry_time_heap;
}

void attempt_first_fit_allocation(
    process* p,
    free_list_node* fl,
    perf_data* pfd,
    int current_time,
    queue* wait_queue
) {
    free_list_node* current = fl;
    while (current != NULL) {
        if (p->size <= current->size && current->empty) {
            break;
        }
        current = current->next;
        ++pfd->iter_steps;
    }
    if (current != NULL) {
        int remainingSize = current->size - p->size;
        if (remainingSize > 0) {
            free_list_node* new_node = create_free_list(remainingSize);
            free_list_node* prev_next = current->next;
            new_node->next = prev_next;
            current->next = new_node;
            current->size = p->size;
        }
        p->actual_entry_time = current_time;
        current->empty = 0;
        current->p = p;
        return;
    }
    ++pfd->failed_allocations;
    add_to_queue(wait_queue, (void*)p);
}

perf_data* first_fit(process** processes, int process_list_size, int block_size) {
    perf_data* pfd = create_perf_data();
    free_list_node* fl = create_free_list(block_size);
    queue* wait_queue = create_queue();
    heap* entry_time_heap = init_entry_time_heap(processes, process_list_size);
    int exited = 0;
    int current_time = 0;
    int queue_size;
    process* next_process;
    while (exited < process_list_size) {
        exited += remove_exited_processes(fl, current_time);
        queue_size = wait_queue->size;
        for (int i = 0; i < queue_size; ++i) {
            next_process = (process*)remove_from_queue(wait_queue);
            attempt_first_fit_allocation(next_process, fl, pfd, current_time, wait_queue);
        }
        while (
            entry_time_heap->size > 0
            && ((process*)get_min_from_heap(entry_time_heap))->entry_time <= current_time
        ) {
            next_process = (process*)remove_min_from_heap(entry_time_heap);
            attempt_first_fit_allocation(next_process, fl, pfd, current_time, wait_queue);
        }
        printf("--------------------------------\n");
        printf("Current Time: %d\n", current_time);
        print_list(fl);
        ++current_time;
    }
    return pfd;
}

perf_data* best_fit(process** processes, int process_list_size, int block_size) {
    return NULL;
}

perf_data* worst_fit(process** processes, int process_list_size, int block_size) {
    return NULL;
}

perf_data* next_fit(process** processes, int process_list_size, int block_size) {
    return NULL;
}
