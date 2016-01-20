#ifndef HASH_H
#define HASH_H

#include <stralloc.h>

struct hash_elem;
typedef struct hash_elem** hash;

hash hash_init();

int hash_add_string(hash h, stralloc sa);

int hash_contains_string(hash h, stralloc sa);

void hash_compare(hash dict, hash text, int show_missing);

#endif //HASH_H
