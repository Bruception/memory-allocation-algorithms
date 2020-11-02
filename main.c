#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "algorithms.h"

int main() {
    int size = 15;
    process** process_list = (process**)(malloc(sizeof(process) * size));
    process_list[0] = create_process(0, 33, 0, 5);
    process_list[1] = create_process(1, 9, 3, 6);
    process_list[2] = create_process(2, 13, 5, 7);
    process_list[3] = create_process(3, 273, 2, 5);
    process_list[4] = create_process(4, 23, 1, 9);
    process_list[5] = create_process(5, 33, 0, 5);
    process_list[6] = create_process(6, 750, 3, 6);
    process_list[7] = create_process(7, 91, 5, 7);
    process_list[8] = create_process(8, 189, 2, 5);
    process_list[9] = create_process(9, 28, 1, 9);
    process_list[10] = create_process(10, 777, 0, 15);
    process_list[11] = create_process(11, 7, 3, 6);
    process_list[12] = create_process(12, 910, 5, 7);
    process_list[13] = create_process(13, 108, 12, 15);
    process_list[14] = create_process(14, 1, 10, 12);
    // Average verified with: ./main.out | grep "^External" | cut -d' ' -f3 | sed -e 's/%//g' | awk '{ ++total; sum += $1 } END { print "Average:", sum/total }'
    perf_data* pfd = first_fit(process_list, size, 920);
    printf("--------------------------------\n");
    printf("Firt Fit\n");
    printf("--------------------------------\n");
    printf("Results:\n\tFailed Allocations: %d\n\tIterations: %d\n\tAverage External Fragmentation: %.2lf%%\n", pfd->failed_allocations, pfd->iterations, pfd->average_external_frag);
    pfd = best_fit(process_list, size, 1091);
    printf("--------------------------------\n");
    printf("Best Fit\n");
    printf("--------------------------------\n");
    printf("Results:\n\tFailed Allocations: %d\n\tIterations: %d\n\tAverage External Fragmentation: %.2lf%%\n", pfd->failed_allocations, pfd->iterations, pfd->average_external_frag);
    pfd = worst_fit(process_list, size, 1091);
    printf("--------------------------------\n");
    printf("Worst Fit\n");
    printf("--------------------------------\n");
    printf("Results:\n\tFailed Allocations: %d\n\tIterations: %d\n\tAverage External Fragmentation: %.2lf%%\n", pfd->failed_allocations, pfd->iterations, pfd->average_external_frag);
    pfd = next_fit(process_list, size, 1091);
    printf("--------------------------------\n");
    printf("Next Fit\n");
    printf("--------------------------------\n");
    printf("Results:\n\tFailed Allocations: %d\n\tIterations: %d\n\tAverage External Fragmentation: %.2lf%%\n", pfd->failed_allocations, pfd->iterations, pfd->average_external_frag);
}
