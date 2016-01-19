#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define HASHLEN 256

int get_hash(stralloc sa);

typedef struct hash_elem {
    stralloc sa;
    struct hash_elem* next;
} hash_elem;

hash hash_init() {
    hash h = (hash) malloc(sizeof(hash_elem*) * HASHLEN);
    return h;
}

int hash_add_string(hash h, stralloc sa) {
    hash_elem* elem = h[get_hash(sa)];
    if(elem == NULL) {
        hash_elem* nh = (hash_elem*) malloc(sizeof(hash_elem));
        if(nh == NULL) {
            return 0;
        } else {
            nh->sa = sa;
            nh->next = NULL;
            elem = nh;
            return 1;
        }
    }
    while(elem != NULL) {
        if(!strcmp(elem->sa.s, sa.s)) {
            return 1;
        }
        elem = elem->next;
    }
    hash_elem* nh = (hash_elem*) malloc(sizeof(hash_elem));
    if(nh == NULL) {
        return 0;
    } else {
        nh->sa = sa;
        nh->next = NULL;
        elem->next = nh;
        return 1;
    }
}

int hash_contains_string(hash h, stralloc sa) {
    hash_elem* elem = h[get_hash(sa)];
    if(elem == NULL) {
        return 0;
    }
    while(elem != NULL) {
        if(!strcmp(elem->sa.s, sa.s)) {
            return 1;
        }
        elem = elem->next;
    }
    return 0;
}

int get_hash(stralloc sa) {
    int h = 5381;
    for(int i = 0; i < sa.len; i++) {
        h = ((h << 5) + h) ^ sa.s[i];
    }
    return h % 256;
}
