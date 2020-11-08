#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "algorithms.h"

void printPerformanceData(
    char* name,
    perf_data* (*algo)(process**, int, int),
    process** processes,
    int size,
    int memory_size
) {
    perf_data* pfd = algo(processes, size, memory_size);
    printf("--------------------------------\n");
    printf("%s\n", name);
    printf("--------------------------------\n");
    double successPercentage = (pfd->attempted_allocations - pfd->failed_allocations) / (double)pfd->attempted_allocations;
    printf("Results:\n");
    printf("\tAttempts: %d Fails: %d\n", pfd->attempted_allocations, pfd->failed_allocations);
    printf("\tAllocation success %%: %.2lf%%\n", successPercentage * 100.0);
    printf("\tIterations: %d\n", pfd->iterations);
    printf("\tAverage External Fragmentation: %.2lf%%\n", pfd->average_external_frag);
    free(pfd);
}

int main() {
    int size = 15;
    int memory_size = 800;
    process** process_list = (process**)(malloc(sizeof(process) * size));
    process_list[0] = create_process(0, 330, 0, 15);
    process_list[1] = create_process(1, 90, 1, 8);
    process_list[2] = create_process(2, 130, 2, 17);
    process_list[3] = create_process(3, 273, 3, 15);
    process_list[4] = create_process(4, 230, 4, 19);
    process_list[5] = create_process(5, 330, 5, 7);
    process_list[6] = create_process(6, 750, 6, 16);
    process_list[7] = create_process(7, 91, 7, 17);
    process_list[8] = create_process(8, 189, 8, 15);
    process_list[9] = create_process(9, 280, 9, 11);
    process_list[10] = create_process(10, 777, 10, 15);
    process_list[11] = create_process(11, 70, 13, 16);
    process_list[12] = create_process(12, 800, 12, 17);
    process_list[13] = create_process(13, 108, 13, 15);
    process_list[14] = create_process(14, 10, 14, 16);
    // Average verified with: ./main.out | grep "^External" | cut -d' ' -f3 | sed -e 's/%//g' | awk '{ ++total; sum += $1 } END { print "Average:", sum/total }'
    printPerformanceData("First Fit", &first_fit, process_list, size, memory_size);
    printPerformanceData("Best Fit", &best_fit, process_list, size, memory_size);
    printPerformanceData("Worst Fit", &worst_fit, process_list, size, memory_size);
    printPerformanceData("Next Fit", &next_fit, process_list, size, memory_size);
    for (int i = 0; i < size; ++i) {
        free(process_list[i]);
    }
    free(process_list);
}
