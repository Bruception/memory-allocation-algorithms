#include <stdlib.h>
#include <stdio.h>
#include "process.h"

int main() {
    process* p = create_process(0, 100, 0, 1);
    printf("%d\n", p->size);
    return 0;
}