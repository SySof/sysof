#include <getopt.h>
#include <stralloc.h>
#include <stdio.h>
#include "hash.h"
#include "scan.h"

void print_usage(char* program_name) {
    fprintf(stderr, "Usage: %s [-i (ignore case)]"
        "[-v (invert result)] <dictionary>\n", program_name);
}

int read_words(hash h, char* words_file) {
    FILE* file;
    if(words_file == NULL) {
        file = stdin;
    } else {
        file = fopen(words_file, "r");
        if(file == NULL) {
            fprintf(stderr, "Could not open file %s.\n", words_file);
            return 1;
        }
    }
    stralloc sa = {0};
    while(scan_word(file, &sa) == 0) {
        printf("New word: %s.\n", sa.s);
        hash_add_string(h, sa);
    }
    if(words_file != NULL) {
        fclose(file);
    }
    return 0;
}

int main(int argc, char** argv) {
    int c, invert = 0, ignore_case = 0;
    char* dict_file;

    while((c = getopt(argc, argv, "iv")) != -1) {
        switch(c) {
            case 'i':
                ignore_case = 1;
                break;
            case 'v':
                invert = 1;
                break;
            case ':':
                switch(optopt) {
                    case '?':
                        print_usage(argv[0]);
                        return  1;
                    default:
                        break;
                }
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    if (optind >= argc) {
        print_usage(argv[0]);
        return 1;
    }
    dict_file = argv[optind];

    hash dict_hash = hash_init();
    int ret = read_words(dict_hash, dict_file);
    if(!ret) {
        return 1;
    }
    hash input_hash = hash_init();
    ret = read_words(input_hash, NULL);
    if(!ret) {
        return 1;
    }

    // TODO: iterate hash and check input against dictionary

    return 0;
}
