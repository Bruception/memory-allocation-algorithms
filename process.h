#ifndef PROCESS_H
#define PROCESS_H

typedef struct process_struct {
    int id;
    int size;
    int entry_time;
    int actual_entry_time;
    int duration;
} process;

typedef struct process_dataset_struct {
    char* name;
    process** process_list;
} process_dataset;

process* create_process(int id, int size, int entry_time, int exit_time);
process_dataset** parse_process_dataset(char* file_name, int data_set_size, int entry_size);

#endif