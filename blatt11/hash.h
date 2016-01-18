#ifndef HASH_H
#define HASH_H

#include <stralloc.h>

#define HASHLEN 256

char* init_hash();

int add_string(stralloc sa);

int contains_string(stralloc sa);

#endif //HASH_H
