#ifndef SCAN_H
#define SCAN_H

#include <stralloc.h>
#include <stdio.h>

// Returns 0 on success, 1 on EOF
int scan_word(FILE* file, stralloc* word);

#endif //SCAN_H
