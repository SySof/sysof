#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"

#define HASHLEN 256

unsigned int get_hash(stralloc sa);

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
            h[get_hash(sa)] = nh;
            return 1;
        }
    }
    while(elem->next != NULL) {
        if(stralloc_equal(&(elem->sa), &sa)) {
            return 1;
        }
        elem = elem->next;
    }
    if(stralloc_equal(&(elem->sa), &sa)) {
        return 1;
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
        if(stralloc_equal(&(elem->sa), &sa)) {
            return 1;
        }
        elem = elem->next;
    }
    return 0;
}

void hash_compare(hash dict, hash text, int show_missing) {
    for(int i = 0; i < HASHLEN; i++) {
        hash_elem* elem = text[i];
        if(elem == NULL) {
            continue;
        }
        while(elem->next != NULL) {
            if(show_missing) {
                if (!hash_contains_string(dict, elem->sa)) {
                    stralloc_0(&(elem->sa));
                    printf("%s\n", elem->sa.s);
                }
            } else {
                if (hash_contains_string(dict, elem->sa)) {
                    stralloc_0(&(elem->sa));
                    printf("%s\n", elem->sa.s);
                }
            }
            elem = elem->next;
        }
        if(show_missing) {
            if (!hash_contains_string(dict, elem->sa)) {
                stralloc_0(&(elem->sa));
                printf("%s\n", elem->sa.s);
            }
        } else {
            if (hash_contains_string(dict, elem->sa)) {
                stralloc_0(&(elem->sa));
                printf("%s\n", elem->sa.s);
            }
        }
    }
}

unsigned int get_hash(stralloc sa) {
    unsigned int h = 5381;
    for(unsigned int i = 0; i < sa.len; i++) {
        h = ((h << 5) + h) ^ sa.s[i];
    }
    return h % 256;
}
