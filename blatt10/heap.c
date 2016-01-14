#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include <time.h>

void printout(heap* heap_storage){
    info** out =  heap_storage->storage;
    struct tm  ts;
    char       buf[80];

    for(int i = 1; i<=heap_storage->elem_count; i++){
        //printf("Name: %s", out[i]->name);
        ts = *localtime(&out[i]->metadata.st_mtime);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        printf("Date: %s", buf);
        printf("\n");
    }
    printf("\n");
}

heap* init_heap(int order, int initial_size) {
    heap*  new_heap = (heap*) malloc(sizeof(heap));
    if(new_heap == NULL) {
        return NULL;
    }
    new_heap->storage = (info**) malloc(initial_size * sizeof(info*));
    if(new_heap->storage == NULL) {
        return NULL;
    }
    new_heap->order = order;
    new_heap->elem_count = 0;
    new_heap->len = initial_size;
    return  new_heap;
}

int add(heap* storage , info* element) {

    if(++(storage->elem_count) >= storage->len) {
        storage->len *= 2;
        info** new_storage = realloc(storage->storage, storage->len * sizeof(info*));
        if(new_storage == NULL) {
            free(storage->storage);
            return 0;
        }
        storage->storage = new_storage;
    }

    int pos = storage->elem_count;

    for(; pos > 1; pos /= 2) {
        if(storage->order == ORDER_ASCENDING) {
            if(storage->storage[pos/2]->metadata.st_mtime < element->metadata.st_mtime) {
                storage->storage[pos] = storage->storage[pos/2];
            } else {
                break;
            }
        } else {
            if(storage->storage[pos/2]->metadata.st_mtime > element->metadata.st_mtime) {
                storage->storage[pos] = storage->storage[pos/2];
            } else {
                break;
            }
        }
    }

    storage->storage[pos] = element;

    return 1;
}

void heapify(heap* storage, int i) {
    if(i <= 1) {
        return;
    }
    info* first = storage->storage[1];
    storage->storage[1] = storage->storage[i];
    storage->storage[i] = first;

    for(int pos = 1; pos < i; pos *= 2) {
        if(pos*2 > storage->elem_count) {
            break;
        }
        if(storage->order == ORDER_ASCENDING) {
            if(storage->storage[pos*2]->metadata.st_mtime > storage->storage[pos]->metadata.st_mtime) {
                info* swap = storage->storage[pos*2];
                storage->storage[pos*2] = storage->storage[pos];
                storage->storage[pos] = swap;
            } else {
                break;
            }
        } else {
            if(storage->storage[pos*2]->metadata.st_mtime < storage->storage[pos]->metadata.st_mtime) {
                info* swap = storage->storage[pos*2];
                storage->storage[pos*2] = storage->storage[pos];
                storage->storage[pos] = swap;
            } else {
                break;
            }
        }
    }
    heapify(storage, i - 1);
}

info** get_sorted_elements(heap* storage) {

    heapify(storage, storage->elem_count);
    return storage->storage;
}