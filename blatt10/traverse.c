#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "heap.h"


void traverse(DIR* dir, heap* heap_storage, char* dirname){
    static int count = 0;
	struct dirent* entry;
	while ((entry = readdir(dir))) {
		struct stat statbuf;
		if (lstat(entry->d_name, &statbuf) < 0) {
			perror(entry->d_name); exit(1);
		}
		if (S_ISREG(statbuf.st_mode)) {
			info* element = malloc(sizeof(info));
            char* name = malloc(strlen(entry->d_name) + strlen(dirname) + 1);
            strcpy(name, dirname);
            strcpy(name + strlen(dirname), entry->d_name);
			element->metadata = statbuf;
			element->name = name;
			add(heap_storage , element);
            printout(heap_storage);

		} else if (S_ISDIR(statbuf.st_mode)) {
            count++;
            if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            } else {
                if (chdir(entry->d_name) < 0) {
                    perror(entry->d_name);
                    exit(1);
                }
                DIR* tmp;
                tmp = opendir(".");
                char new_dirname[strlen(dirname) + strlen(entry->d_name) + 2];
                strcpy(new_dirname, dirname);
                strcpy(new_dirname + strlen(dirname), entry->d_name);
                new_dirname[strlen(dirname) + strlen(entry->d_name)] = '/';
                new_dirname[strlen(dirname) + strlen(entry->d_name) + 1] = '\0';
                traverse(tmp, heap_storage, new_dirname);
                chdir("..");
            }
		}
	}
}
