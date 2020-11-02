#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "algorithms.h"

int main() {
    int size = 5;
    process** process_list = (process**)(malloc(sizeof(process) * size));
    process_list[0] = create_process(0, 33, 0, 5);
    process_list[1] = create_process(1, 75, 0, 6);
    process_list[2] = create_process(2, 91, 0, 7);
    process_list[3] = create_process(3, 1089, 0, 5);
    process_list[4] = create_process(4, 238, 0, 9);
    perf_data* pfd = best_fit(process_list, size, 1100);
    printf("--------------------------------\n");
    // Average verified with: ./main.out | grep "^External" | cut -d' ' -f3 | sed -e 's/%//g' | awk '{ ++total; sum += $1 } END { print "Average:", sum/total }'
    printf("Results:\n\tFailed Allocations: %d\n\tIterations: %d\n\tAverage External Fragmentation: %.2lf%%\n", pfd->failed_allocations, pfd->iterations, pfd->average_external_frag);
}
