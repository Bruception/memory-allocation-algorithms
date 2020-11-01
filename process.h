#ifndef PROCESS_H
#define PROCESS_H

typedef struct process_struct {
    int id;
    int size;
    int entry_time;
    int exit_time;
} process;

process* create_process(int id, int size, int entry_time, int exit_time);

#endif