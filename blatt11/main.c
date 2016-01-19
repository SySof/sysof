#include <stralloc.h>
#include "hash.h"

void test_hash() {
    hash h = hash_init();
    stralloc str = {0};
    stralloc_copys(&str, "Test");
    hash_add_string(h, str);
}

int main() {
    test_hash();
    return 0;
}
