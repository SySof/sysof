#ifndef HEAP_H
#define HEAP_H

#include <sys/stat.h>

#define ORDER_ASCENDING 0
#define ORDER_DESCENDING 1

typedef struct info {
    struct stat metadata;
    char* name;
} info;

typedef struct heap {
    int order;
    int elem_count;
    int len;
    info** storage;
} heap;

heap* init_heap(int order, int initial_size);

int add(heap* storage, info* element);

info** get_sorted_elements(heap* storage);

void printout(heap* heap_storage);

#endif
