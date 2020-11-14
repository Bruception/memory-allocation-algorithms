#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "process.h"

process* create_process(int id, int size, int entry_time, int exit_time) {
    process* p = (process*)malloc(sizeof(process));
    p->id = id;
    p->size = size;
    p->entry_time = entry_time;
    p->actual_entry_time = 0;
    p->duration = exit_time - entry_time;
    return p;
}

int* parse_row(char* row, int entry_size) {
    int i = 0;
    int* row_values = (int*)malloc(sizeof(int) * entry_size);
    char* token = strtok(row, ", ");
    while (token != NULL) { 
        row_values[i] = atoi(token);
        token = strtok(NULL, ", ");
        ++i;
    } 
    return row_values;
}

process_dataset** parse_process_dataset(char* file_name, int data_set_size, int entry_size) {
    static int default_start_times[10] = {1, 1, 4, 4, 5, 6, 6, 7, 7, 7}; 
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        return NULL;
    }
    process_dataset** data = (process_dataset**)malloc(sizeof(process_dataset*) * data_set_size);
    int buffer_size = 256;
    char buffer[buffer_size];
    int current = -1;
    while(fgets(buffer, buffer_size, fp)) {
        if (isalpha(buffer[0])) {
            ++current;
            int str_len = strlen(buffer);
            data[current] = (process_dataset*)malloc(sizeof(process_dataset));
            data[current]->name = (char*)malloc(strlen(buffer));
            strncpy(data[current]->name, buffer, str_len - 1);
            int* sizes = parse_row(fgets(buffer, buffer_size, fp), entry_size);
            int* durations = parse_row(fgets(buffer, buffer_size, fp), entry_size);
            data[current]->process_list = (process**)malloc(sizeof(process*) * entry_size);
            for (int i = 0; i < entry_size; ++i) {
                int end_time = default_start_times[i] + durations[i];
                data[current]->process_list[i] = create_process(i, sizes[i], default_start_times[i], end_time);
            }
        }
    }
    fclose(fp);
    return data;
}
