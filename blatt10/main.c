#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "heap.h"


char* usage =  "Usage: [-c max number of files] [-d show date of last change]" \
"[-o list oldest files] [-C select other direction]\n";

void traverse(DIR*, heap*);

int main(int argc, char* argv[]) {
	int c = 0, amount = 10, show_date = 0, oldest = 0;
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
				printf("I've been here!");
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

	heap* heap_storage = init_heap(20000, oldest);
	
	traverse(dir, heap_storage);
	
	info** out =  get_sorted_elements(heap_storage);

	for(int i = 1; i<heap_storage->elem_count; i++){
		if(i > amount) break;
		printf("Name: %s", out[i]->name);
		if(show_date == 1)printf("; Date: %ld", out[i]->metadata.st_mtime);
		printf("\n");
	}
	
	
	closedir(dir);
}
