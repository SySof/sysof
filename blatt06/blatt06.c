#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define buflen 256

struct city {
    char name[buflen];
    unsigned int population;
};

void print_city_list(struct city **list, unsigned int n) {
    unsigned int i;
    printf("City list:\n");
    for(i = 0; i < n; i++) {
        printf("City %d: %s\n", i, list[i]->name);
        
    }
}

void print_sorted_city_list(struct city **list, unsigned int n) {
    unsigned int i;
    printf("Sorted cities:\n");
    for(i = 0; i < n; i++) {
        printf("%d: \n", i);
        printf("    %s\n", list[i]->name);
    }
    printf("%d: \n", n);
}

unsigned int read_city_list(char *filename, struct city *list, unsigned int n) {
    char buffer[buflen];
    char *colon_pos, *newline_pos;
    unsigned int t, M;
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return 0;
    }
    for(t = 0; fgets(buffer, buflen, file); t++) {
        colon_pos = strchr(buffer, ':');
        newline_pos = strchr(buffer, '\n');
        if(colon_pos == NULL || newline_pos == NULL || colon_pos > newline_pos) {
            continue;
        }
        if((M = t) < n || (M = rand() % t) < n) {
            *colon_pos = 0;
            strncpy(list[M].name, buffer, colon_pos-buffer+1);
            *newline_pos = 0;
            list[M].population = atoi(colon_pos+1);
        }
    }
    fclose(file);
    return 1;
}

unsigned int sorted(struct city **list, unsigned int n) {
    unsigned int i;
    if(n == 0) return 1;
    for(i = 0; i < n-1; i++) {
        if(list[i]->population > list[i+1]->population) {
            unsigned int j;
            printf("Sorry, the list is not sorted!\n");
            for(j = 0; j < n; j++) {
                printf("%s %d: %s, %d\n", (j==i ? "->" : "  "), j, list[j]->name, list[j]->population);
            }
            return 0;
        }
    }
    return 1;
}

int main() {
    const unsigned int n = 8;
    struct city list[n];
    struct city *unsorted_list[n];
    struct city *sorted_list[n];
    unsigned int sorted_cities;
    unsigned int city_num, city_pos, i;

    srand(time(NULL));
    sorted_cities = 0;
    if(!read_city_list("staedte", list, n)) {
        printf("Error reading file!\n");
        return 1;
    }

    for(city_num = 0; city_num < n; city_num++) {
        unsorted_list[city_num] = list + city_num;
    }

    printf("Welcome to the game. Sort the following cities in ascending population number.\n");
    while(sorted(sorted_list, sorted_cities) && sorted_cities < n) {
        print_city_list(unsorted_list, n-sorted_cities);
        print_sorted_city_list(sorted_list, sorted_cities);
        do {
            printf("Please input a city number and an insert position:\n");
        } while(scanf("%d %d", &city_num, &city_pos) != 2
            || city_num >= (n-sorted_cities) || city_pos > sorted_cities);
        for(i = sorted_cities; i > city_pos; i--) {
            sorted_list[i] = sorted_list[i-1];
        }
        sorted_list[city_pos] = unsorted_list[city_num];
        sorted_cities++;
        unsorted_list[city_num] = unsorted_list[n-sorted_cities];
    }
    printf("You got %d points.\n", sorted_cities);
    return 0;
}
