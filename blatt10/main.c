#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "heap.h"
#include "traverse.h"


char* usage =  "Usage: [-c max number of files] [-d show date of last change]" \
"[-o list oldest files] [-C select other direction]\n";

int main(int argc, char* argv[]) {
    int c = 0, amount = 100, show_date = 0, oldest = 0;
    char* dirname = ".";

    while((c = getopt(argc, argv, "c:doC:")) != -1) {
        switch(c) {
            case 'c':
                amount = atoi(optarg);
                break;
            case 'd':
                show_date = 1;
                break;
            case 'o':
                oldest = 1;
                break;
            case 'C':
                dirname = optarg;
                break;
            case ':':
                switch(optopt) {
                    case '?':
                        printf("%s", usage);
                        return  1;
                    default:
                        break;
                }
                break;
            default:
                printf("%s", usage);
                return 1;
        }
    }

    if (chdir(dirname) < 0) {
        perror(dirname);
        exit(1);
    }
    DIR* dir = opendir(".");
    if (!dir) {
        perror(dirname);
        exit(1);
    }

    heap* heap_storage = init_heap(oldest, amount);

    traverse(dir, heap_storage, "./");

    printout(heap_storage, show_date);


    closedir(dir);
}
