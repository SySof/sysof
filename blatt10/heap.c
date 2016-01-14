#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include <time.h>

struct heap {
    int order;
    int elem_count;
    int len, max_len;
    info** storage;
};

void heapify(heap* storage, int i);

void printout_single(info* out, int show_date) {
    struct tm  ts;
    char       buf[80];

    printf("Name: %s", out->name);
    if(show_date == 1) {
        ts = *localtime(&out->metadata.st_mtime);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        printf("; Date: %s", buf);
    }
    printf("\n");
}

void printout(heap* heap_storage, int show_date) {
    heapify(heap_storage, heap_storage->elem_count);
    for(int i = 1; i<=heap_storage->elem_count; i++) {
        printout_single(heap_storage->storage[i], show_date);
    }
}

heap* init_heap(int order, int size) {
    heap* new_heap = (heap*) malloc(sizeof(heap));
    if(new_heap == NULL) {
        return NULL;
    }
    if(size <= 0) {
        size = 1000000;
    }
    new_heap->max_len = size;
    size = size > 8 ? 8 : size;
    new_heap->storage = (info**) malloc(size * sizeof(info*));
    if(new_heap->storage == NULL) {
        return NULL;
    }
    new_heap->order = order;
    new_heap->elem_count = 0;
    new_heap->len = size;
    return new_heap;
}

void check_heap(heap* storage, int i, int len) {
    if(i < 1) {
        return;
    }
    if(2*i <= len) {
        if(storage->order == ORDER_ASCENDING) {
            assert(storage->storage[2*i]->metadata.st_mtime
                <= storage->storage[i]->metadata.st_mtime);
            check_heap(storage, 2*i, len);
        } else {
            assert(storage->storage[2*i]->metadata.st_mtime
                >= storage->storage[i]->metadata.st_mtime);
            check_heap(storage, 2*i, len);
        }
    }
    if((2*i + 1) <= len) {
        if(storage->order == ORDER_ASCENDING) {
            assert(storage->storage[2*i + 1]->metadata.st_mtime
                <= storage->storage[i]->metadata.st_mtime);
            check_heap(storage, 2*i + 1, len);
        } else {
            assert(storage->storage[2*i + 1]->metadata.st_mtime
                >= storage->storage[i]->metadata.st_mtime);
            check_heap(storage, 2*i + 1, len);
        }
    }
}

int add(heap* storage, info* element) {

    int pos;

    if((storage->elem_count + 1) > storage->max_len)
    {
        check_heap(storage, 1, storage->elem_count);
        int last_pos = 0; // index to smallest (or biggest) element
        time_t last_time = element->metadata.st_mtime;
        for(int i = 1; i <= storage->elem_count; i++) {
            if(storage->order == ORDER_ASCENDING) {
                if(storage->storage[i]->metadata.st_mtime <= last_time) {
                    last_pos = i;
                    last_time = storage->storage[i]->metadata.st_mtime;
                }
            } else {
                if(storage->storage[i]->metadata.st_mtime >= last_time) {
                    last_pos = i;
                    last_time = storage->storage[i]->metadata.st_mtime;
                }
            }
        }
        if(last_pos == 0) {
            return 1;
        }
        pos = last_pos;
    } else {
        if(++(storage->elem_count) >= storage->len) {
            storage->len = (2*storage->len > storage->max_len)
                ? storage->max_len : 2*storage->len;
            info** new_storage = realloc(storage->storage, storage->len * sizeof(info*));
            if(new_storage == NULL) {
                free(storage->storage);
                return 0;
            }
            storage->storage = new_storage;
        }
        pos = storage->elem_count;
    }

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

    check_heap(storage, 1, storage->elem_count);

    return 1;
}

void heapify(heap* storage, int i) {
    check_heap(storage, 1, i);
    if(i <= 1) {
        return;
    }
    info* first = storage->storage[1];
    storage->storage[1] = storage->storage[i];
    storage->storage[i] = first;

    for(int pos = 1; 2*pos < i;) {
        int new_pos = pos*2;
        if(storage->order == ORDER_ASCENDING) {
            // if parent node is smaller than one of its child nodes,
            // then swap parent node with the bigger one of the child nodes
            if(storage->storage[new_pos]->metadata.st_mtime
                > storage->storage[pos]->metadata.st_mtime
                || ((new_pos + 1) < i
                && storage->storage[new_pos + 1]->metadata.st_mtime
                > storage->storage[pos]->metadata.st_mtime)) {

                if((new_pos + 1) < i
                    && storage->storage[new_pos + 1]->metadata.st_mtime
                    > storage->storage[new_pos]->metadata.st_mtime) {
                    new_pos++;
                }
                info* swap = storage->storage[new_pos];
                storage->storage[new_pos] = storage->storage[pos];
                storage->storage[pos] = swap;
            } else {
                break;
            }
        } else {
            // if parent node is bigger than one of its child nodes,
            // then swap parent node with the smaller one of the child nodes
            if(storage->storage[new_pos]->metadata.st_mtime
                < storage->storage[pos]->metadata.st_mtime
                || ((new_pos + 1) < i
                && storage->storage[new_pos + 1]->metadata.st_mtime
                < storage->storage[pos]->metadata.st_mtime)) {

                if((new_pos + 1) < i
                    && storage->storage[new_pos + 1]->metadata.st_mtime
                    < storage->storage[new_pos]->metadata.st_mtime) {
                    new_pos++;
                }
                info* swap = storage->storage[new_pos];
                storage->storage[new_pos] = storage->storage[pos];
                storage->storage[pos] = swap;
            } else {
                break;
            }
        }
        pos = new_pos;
    }
    heapify(storage, i - 1);
}

info** get_sorted_elements(heap* storage) {

    heapify(storage, storage->elem_count);
    return storage->storage;
}
