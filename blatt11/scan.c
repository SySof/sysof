#include <stdlib.h>
#include <string.h>
#include "scan.h"

int stralloc_ready_(stralloc *sa,size_t len) {
  register size_t wanted=len+(len>>3)+30; /* heuristic from djb */
  if (!sa->s || sa->a<len) {
    register char* tmp;
    if (!(tmp=realloc(sa->s,wanted)))
      return 0;
    sa->a=wanted;
    sa->s=tmp;
  }
  return 1;
}

int scan_word(FILE* file, stralloc* word) {
    int c;
    word->a = 0;
    word->len = 0;
    word->s = NULL;
    while((c = fgetc(file)) != EOF) {
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            word->len = word->len > 0 ? word->len+1 : 2;
            if(stralloc_ready_(word, word->len) != 1) {
                fprintf(stderr, "stralloc error.\n");
                exit(1);
            }
            word->s[word->len-2] = (unsigned char) c;
            word->s[word->len-1] = '\0';
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
