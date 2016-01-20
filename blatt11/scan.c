#include <ctype.h>
#include <stdlib.h>
#include "scan.h"

#define MIN_ALLOC 5

int scan_word(FILE* file, stralloc* word, int ignore_case) {
    int c;
    word->a = 0;
    word->len = 0;
    word->s = NULL;
    while((c = fgetc(file)) != EOF) {
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'' || c == 133 || c > 160) {
            if(stralloc_readyplus(word, MIN_ALLOC) != 1) {
                fprintf(stderr, "stralloc error.\n");
                exit(1);
            }
            if(ignore_case && c >= 'A' && c <= 'Z') {
                c = tolower(c);
            }
            stralloc_append(word, (char*)&c);
        } else {
            if(word->len == 0) {
                continue; // skip characters at beginning of word
            } else {

                return 0; // return if end of word detected
            }
        }
    }
    if(word->len == 0) {
        return 1;
    }
    return 0;
}
