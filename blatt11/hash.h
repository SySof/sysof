#ifndef HASH_H
#define HASH_H

#include <stralloc.h>

struct hash_elem;
typedef struct hash_elem** hash;

hash hash_init();

int hash_add_string(hash h, stralloc sa);

int hash_contains_string(hash h, stralloc sa);

#endif //HASH_H
