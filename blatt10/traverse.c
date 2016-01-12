#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "heap.h"


void traverse(DIR* dir, heap* heap_storage){
	struct dirent* entry;
	while ((entry = readdir(dir))) {
		struct stat statbuf;
		if (lstat(entry->d_name, &statbuf) < 0) {
			perror(entry->d_name); exit(1);
		}
		if (S_ISREG(statbuf.st_mode)) {
			printf("REG");
			//TODO: Insert heap
			info* element = malloc(sizeof(info));
			element->metadata = statbuf;
			element->name = entry->d_name;
			add(heap_storage , element);
//			if(add(heap_storage , element) == 1){
//				printf("ERROR");
//				return;
//			}
		} else if (S_ISDIR(statbuf.st_mode)) {
			printf("Hallo");
			if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
				continue;
			} else {
				if (chdir(entry->d_name) < 0) {
					perror(entry->d_name);
					exit(1);
				}
				DIR* tmp = malloc(sizeof(DIR*));
				tmp = opendir(".");
				traverse(tmp, heap_storage);
				chdir("..");
				dir = opendir(".");
			}
		}
	}
}
