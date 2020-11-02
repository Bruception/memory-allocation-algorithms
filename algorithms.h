#include "process.h"

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

typedef struct perf_data_struct {
    double average_external_frag;
    int failed_allocations;
    int iterations;
} perf_data;

perf_data* first_fit(process** processes, int process_list_size, int block_size);
perf_data* best_fit(process** processes, int process_list_size, int block_size);
perf_data* worst_fit(process** processes, int process_list_size, int block_size);
perf_data* next_fit(process** processes, int process_list_size, int block_size);

#endif