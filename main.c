#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "algorithms.h"

typedef struct algo_pair_struct {
    char* name;
    perf_data* (*algo)(process**, int, int, int);
} algo_pair;

void printPerformanceData(
    algo_pair* pair,
    process** processes,
    int size,
    int memory_size
) {
    static int show_verbose = 0;
    perf_data* pfd = pair->algo(processes, size, memory_size, show_verbose);
    printf("--------------------------------\n");
    printf("%s\n", pair->name);
    printf("--------------------------------\n");
    double successPercentage = (pfd->attempted_allocations - pfd->failed_allocations) / (double)pfd->attempted_allocations;
    printf("Results:\n");
    printf("\tAllocation attempts: %d Allocation fails: %d\n", pfd->attempted_allocations, pfd->failed_allocations);
    printf("\tAllocation success %%: %.2lf%%\n", successPercentage * 100.0);
    printf("\tIterations: %d\n", pfd->iterations);
    // Average verified with: ./main.out | grep "^External" | cut -d' ' -f3 | sed -e 's/%//g' | awk '{ ++total; sum += $1 } END { print "Average:", sum/total }'
    printf("\tAverage External Fragmentation: %.2lf%%\n", pfd->average_external_frag);
    free(pfd);
}

int main() {
    algo_pair algo_map[4] = {
        {"First Fit", &first_fit},
        {"Best Fit", &best_fit},
        {"Worst Fit", &worst_fit},
        {"Next Fit", &next_fit},
    };
    int size = 10;
    int data_set_size = 4;
    int memory_size = 1000;
    process_dataset** data = parse_process_dataset("data.csv", data_set_size, size);
    if (data == NULL) {
        return 1;
    }
    for (int i = 0; i < 4; ++i) {
        printf("############################################################\n");
        printf("Testing %s ...\n", data[i]->name);
        printf("############################################################\n");
        for (int j = 0; j < 4; ++j) {
            algo_pair algo = algo_map[j];
            printPerformanceData(&algo, data[i]->process_list, size, memory_size);
        }
    }
    return 0;
}
