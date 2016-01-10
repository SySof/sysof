#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "heap.h"



void traverse(DIR* dir){
	struct dirent* entry;
	while ((entry = readdir(dir))) {
		struct stat statbuf;
		if (lstat(entry->d_name, &statbuf) < 0) {
			perror(entry->d_name); exit(1);
		}
		if (S_ISREG(statbuf.st_mode)) {
			//TODO: Insert heap
			
			
		} else if (S_ISDIR(statbuf.st_mode)) {
			if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
				continue;
			} else {
				if (chdir(entry->d_name) < 0) {
					perror(entry->d_name);
					exit(1);
				}
				DIR* tmp;
				tmp = opendir(".");
				traverse(tmp);
				chdir("..");
			}
		}
	}
}
