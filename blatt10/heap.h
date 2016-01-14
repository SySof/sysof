#ifndef HEAP_H
#define HEAP_H

#include <sys/stat.h>

#define ORDER_ASCENDING 0
#define ORDER_DESCENDING 1

typedef struct info {
    struct stat metadata;
    char* name;
} info;

struct heap;
typedef struct heap heap;

heap* init_heap(int order, int size);

int add(heap* storage, info* element);

info** get_sorted_elements(heap* storage);

void printout(heap* heap_storage, int show_date);

#endif
