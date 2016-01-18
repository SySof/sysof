#include <stdlib.h>
#include <string.h>
#include "hash.h"

typedef struct hash {
	stralloc sa;
	struct hash* next;	
} hash;

hash** init_hash() {
	hash** h = (hash**) malloc(sizeof(hash*) * HASHLEN);
	return h;
}

int add_string(hash** h, stralloc sa) {
	hash* elem = h[get_hash(sa)];
	if(elem == NULL) {
		hash* nh = (hash*) malloc(sizeof(hash));
		if(nh == NULL) {
			return 0;	
		} else {
			nh->sa = sa;
			nh->next = NULL;
			elem = nh;
			return 1;
		}
	}
	while(elem->next != NULL) {
		if(!strcmp(h[index]->sa.s, sa.s)) {
			return 1;	
		}
		elem = elem->next;	
	}
	if(!strcmp(h[index]->sa.s, sa.s)) {
		return 1;	
	}
	hash* nh = (hash*) malloc(sizeof(hash));
	if(nh == NULL) {
		return 0;	
	} else {
		nh->sa = sa;
		nh->next = Null;
		elem.next = nh;
		return 1;
	} 
}

int contains_string(hash** h,stralloc sa) {
	hash* elem = h[get_hash(sa)];
	if(elem == NULL) {
		return 0;
	}
	while(elem->next != NULL) {
		if(!strcmp(h[index]->sa.s, sa.s)) {
			return 1;	
		}
		elem = elem->next;	
	}
	if(!strcmp(h[index]->sa.s, sa.s)) {
		return 1;	
	} else {
		return 0;	
	}
}

int get_hash(stralloc sa) {
    int h = 5381;
	for(int i = 0; i < sa.len; i++) {
		h = ((h << 5) + h) ^ sa.s[i];
	}
	return h % 256;
}
