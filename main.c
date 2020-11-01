#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "algorithms.h"

int main() {
    int size = 5;
    process** process_list = (process**)(malloc(sizeof(process) * size));
    process_list[0] = create_process(0, 100, 0, 5);
    process_list[1] = create_process(1, 100, 1, 6);
    process_list[2] = create_process(2, 100, 2, 7);
    process_list[3] = create_process(3, 100, 3, 5);
    process_list[4] = create_process(4, 1000, 4, 9);
    perf_data* pfd = first_fit(process_list, size, 1000);
    printf("Failed: %d, Iterations: %d\n", pfd->failed_allocations, pfd->iter_steps);
}
