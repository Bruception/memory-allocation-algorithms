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

typedef struct free_list_struct {
    struct free_list_struct* next;
    unsigned int size;
    unsigned short empty;
    process* p;
} free_list;

void print_list(free_list* fl) {
    free_list* current = fl;
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

void merge_empty_nodes(free_list* fl) {
    free_list* current = fl;
    while (current != NULL) {
        free_list* next = current->next;
        while (current->empty && next != NULL && next->empty) {
            current->size += next->size;
            current->next = next->next;
            free(next);
            next = current->next;
        }
        current = current->next;
    }
}

int remove_exited_processes(free_list* fl, int current_time) {
    int exited = 0;
    free_list* current = fl;
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
    pf->iterations = 0;
    return pf;
}

free_list* create_free_list(int block_size) {
    free_list* fl = (free_list*)malloc(sizeof(free_list));
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

double calculate_external_fragmentation(free_list* fl) {
    int total_free_bytes = 0;
    int max_free_size = 0;
    free_list* current = fl;
    while(current != NULL) {
        if (current->empty) {
            if (current->size > max_free_size) {
                max_free_size = current->size;
            }
            total_free_bytes += current->size;
        }
        current = current->next;
    }
    if (total_free_bytes == 0) {
        return 0;
    }
    return (total_free_bytes - max_free_size) / (double)total_free_bytes * 100.0;
}

void attempt_allocation(
    free_list* (*allocation_algorithm)(free_list* head, process* p, perf_data* pfd),
    free_list* fl,
    process* p,
    perf_data* pfd,
    int current_time,
    queue* wait_queue
) {
    free_list* current = allocation_algorithm(fl, p, pfd);
    if (current != NULL) {
        int remainingSize = current->size - p->size;
        if (remainingSize > 0) {
            free_list* new_node = create_free_list(remainingSize);
            free_list* prev_next = current->next;
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

perf_data* execute_allocation_algorithm(
    free_list* (*allocation_algorithm)(free_list* head, process* p, perf_data* pfd),
    process** processes,
    int process_list_size,
    int block_size
) {
    perf_data* pfd = create_perf_data();
    free_list* fl = create_free_list(block_size);
    queue* wait_queue = create_queue();
    heap* entry_time_heap = init_entry_time_heap(processes, process_list_size);
    int exited = 0;
    int current_time = 0;
    int queue_size;
    process* next_process;
    double average_external_frag = 0;
    double external_frag = 0;
    while (exited < process_list_size) {
        exited += remove_exited_processes(fl, current_time);
        queue_size = wait_queue->size;
        for (int i = 0; i < queue_size; ++i) {
            next_process = (process*)remove_from_queue(wait_queue);
            attempt_allocation(allocation_algorithm, fl, next_process, pfd, current_time, wait_queue);
        }
        while (
            entry_time_heap->size > 0
            && ((process*)get_min_from_heap(entry_time_heap))->entry_time <= current_time
        ) {
            next_process = (process*)remove_min_from_heap(entry_time_heap);
            attempt_allocation(allocation_algorithm, fl, next_process, pfd, current_time, wait_queue);
        }
        printf("--------------------------------\n");
        printf("Current Time: %d\n", current_time);
        print_list(fl);
        external_frag = calculate_external_fragmentation(fl);
        average_external_frag += calculate_external_fragmentation(fl);
        printf("External Fragmentation: %.2lf%%\n", external_frag);
        ++current_time;
    }
    pfd->average_external_frag = average_external_frag / current_time;
    return pfd;
}

// Finds the first large enough block
free_list* first_fit_allocation(free_list* head, process* p, perf_data* pfd) {
    free_list* current = head;
    while (current != NULL) {
        if (p->size <= current->size && current->empty) {
            break;
        }
        current = current->next;
        ++pfd->iterations;
    }
    return current;
}

perf_data* first_fit(process** processes, int process_list_size, int block_size) {
    return execute_allocation_algorithm(&first_fit_allocation, processes, process_list_size, block_size);
}

// Finds the smallest large enough block
free_list* best_fit_allocation(free_list* head, process* p, perf_data* pfd) {
    free_list* current = head;
    int best_block_size = -1;
    free_list* best = NULL;
    while (current != NULL) {
        if (p->size <= current->size && current->empty) {
            best_block_size = (best_block_size == -1) ? current->size : -1;
            if (current->size <= best_block_size) {
                best_block_size = current->size;
                best = current;
            }
        }
        current = current->next;
        ++pfd->iterations;
    }
    return best;
}

perf_data* best_fit(process** processes, int process_list_size, int block_size) {
    return execute_allocation_algorithm(&best_fit_allocation, processes, process_list_size, block_size);
}

// Finds the largest large enough block
free_list* worst_fit_allocation(free_list* head, process* p, perf_data* pfd) {
    free_list* current = head;
    int best_block_size = -1;
    free_list* worst = NULL;
    while (current != NULL) {
        if (p->size <= current->size && current->empty) {
            best_block_size = (best_block_size == -1) ? current->size : -1;
            if (current->size >= best_block_size) {
                best_block_size = current->size;
                worst = current;
            }
        }
        current = current->next;
        ++pfd->iterations;
    }
    return worst;
}
perf_data* worst_fit(process** processes, int process_list_size, int block_size) {
    return execute_allocation_algorithm(&worst_fit_allocation, processes, process_list_size, block_size);
}

perf_data* next_fit(process** processes, int process_list_size, int block_size) {
    return NULL;
}
