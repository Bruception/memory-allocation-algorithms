#include <stdlib.h>
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
