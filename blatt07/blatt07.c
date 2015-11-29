#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define buflen 128

struct city {
    char name[buflen];
    unsigned int population;
    double latitude, longitude;
    struct city *next;
};

unsigned long int get_hashval(const char *str) {
    unsigned long int hashval;
    const char *cp;
    hashval = 0;
    for (cp = str; *cp; ++cp) {
        hashval = (hashval << 3ul) ^ *cp ^ (hashval >> 28ul);
    }
    return hashval;
}

struct city *city_list_prepend(struct city *city_list) {
    struct city *new_city;
    new_city = malloc(sizeof(struct city));
    if(!new_city) {
        printf("Allocating failed.\n");
        exit(1);
    }
    new_city->next = city_list;
    return new_city;
}

struct city *city_list_read(char *filename) {
    char buffer[buflen];
    struct city cur_city;
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return NULL;
    }
    cur_city.next = NULL;
    while(fgets(buffer, buflen, file)) {
        if(sscanf(buffer, "%127[^:]:%lf:%lf", cur_city.name, &cur_city.latitude, &cur_city.longitude) == 3) {
            cur_city.next = city_list_prepend(cur_city.next);
            strcpy(cur_city.next->name, cur_city.name);
            cur_city.next->latitude = cur_city.latitude;
            cur_city.next->longitude = cur_city.longitude;
        }
    }
    fclose(file);
    return cur_city.next;
}

void city_list_print(struct city *city_list) {
    if(city_list) {
        city_list_print(city_list->next);
        printf("City %s: %f, %f\n", city_list->name, city_list->latitude, city_list->longitude);
    }
}

void city_list_free(struct city *city_list) {
    if(city_list) {
        city_list_free(city_list->next);
        free(city_list);
    }
}

int main() {
    struct city *city_list;

    srand(time(NULL));
    if(!(city_list = city_list_read("gemeinden.txt"))) {
        printf("Error reading file!\n");
        return 1;
    }

    printf("City list:\n");
    city_list_print(city_list);
    city_list_free(city_list);

    return 0;
}

