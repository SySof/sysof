#ifndef TRAVERSE_H
#define TRAVERSE_H

#include <sys/types.h>

typedef void (*TraverseHandler)(void* handle, char* path, struct stat statbuf);
void traverse(DIR* dir, char* dirname, TraverseHandler handler, void* handle);

#endif
