#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define buflen 128
#define BUCKET_COUNT 256

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI acos(-1)
#endif

struct city {
    char name[buflen];
    unsigned int population;
    double latitude, longitude;
    struct city *next;
};

double to_rad(double degree) {
    return degree*(M_PI/180);
}

double dist_vincenty2(double lat1, double lon1, double lat2, double lon2) {
    static const double a = 6378137;
    static const double f = 1 / 298.257223563;

    double lat1_rad = to_rad(lat1);
    double lon1_rad = to_rad(lon1);
    double lat2_rad = to_rad(lat2);
    double lon2_rad = to_rad(lon2);

    double F = (lat1_rad + lat2_rad) / 2.0;
    double G = (lat1_rad - lat2_rad) / 2.0;
    double l = (lon1_rad - lon2_rad) / 2.0;

    double S = sin(G)*sin(G)*cos(l)*cos(l) + cos(F)*cos(F)*sin(l)*sin(l);
    double C = cos(G)*cos(G)*cos(l)*cos(l) + sin(F)*sin(F)*sin(l)*sin(l);
    double w = atan(sqrt(S/C));
    double D = 2*w*a;

    double T = sqrt(S*C)/w;
    double H1 = (3*T - 1)/(2*C);
    double H2 = (3*T + 1)/(2*S);

    double s = D*(1 + f*H1*sin(F)*sin(F)*cos(G)*cos(G) - f*H2*cos(F)*cos(F)*sin(G)*sin(G));
    return s/1000.0;
}

double dist_vincenty(struct city *city1, struct city *city2) {
    return dist_vincenty2(city1->latitude, city1->longitude, city2->latitude, city2->longitude);
}

unsigned char get_hashval(const char *str) {
    unsigned long int hashval = 0;
    while(*str++) {
        hashval = (hashval << 3ul) ^ *str ^ (hashval >> 28ul);
    }
    return hashval%BUCKET_COUNT;
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

void strcpy_encode(unsigned char *dst, unsigned char *src) {
    /* encode from ISO-8859-1 to UTF-8 */
    while(*src) {
        if(*src < 128) {
            *dst++ = *src++;
        } else {
            *dst++ = 0xc2 + (*src > 0xbf);
            *dst++ = (*src++ & 0x3f) + 0x80;
        }
    }
    *dst = '\0';
}

unsigned int city_list_read(char *filename, struct city **buckets) {
    char buffer[buflen];
    struct city cur_city;
    struct city *cur_city_bucket;
    unsigned char hash;
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return 1;
    }
    while(fgets(buffer, buflen, file)) {
        if(sscanf(buffer, "%63[^:]:%lf:%lf", cur_city.name, &cur_city.latitude, &cur_city.longitude) == 3) {
            strcpy_encode((unsigned char*)buffer, (unsigned char*)cur_city.name);
            hash = get_hashval(buffer);
            cur_city_bucket = buckets[hash] = city_list_prepend(buckets[hash]);
            strcpy(cur_city_bucket->name, buffer);
            cur_city_bucket->latitude = cur_city.latitude;
            cur_city_bucket->longitude = cur_city.longitude;
        }
    }
    fclose(file);
    return 0;
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

struct city *read_city(struct city **buckets, FILE *file) {
    char name[buflen];
    struct city *cur_city;
    while(1) {
        printf("Please enter a city name:\n");
        if(!fgets(name, buflen, file)) {
            printf("EOF while reading.\n");
            exit(1);
        }
        name[strlen(name) - 1] = '\0'; /* delete newline */
        for(cur_city = buckets[get_hashval(name)]; cur_city; cur_city = cur_city->next) {
            if(strcmp(name, cur_city->name) == 0) {
                return cur_city;
            }
        }
        printf("City not found.\n");
    }
}

int main() {
    struct city *city_buckets[BUCKET_COUNT];
    struct city *start_city, *dest_city, *cur_city, *next_city;
    double cur_distance, max_distance;
    unsigned int i;
    const char *rating[7] = {"Impossible!", "Fantastic!", "Great!",
        "Very good!", "Good.", "Not bad.", "You can do better."};

    /* init */
    memset(city_buckets, 0, BUCKET_COUNT * sizeof(struct city*));
    if(city_list_read("gemeinden.txt", city_buckets)) {
        printf("Error reading file!\n");
        return 1;
    }
    max_distance = 0;

    /* game start */
    printf("Welcome to the 'Long chain of short trips' game.\n");
    printf("To begin, please enter a city where you would like to start.\n");
    cur_city = start_city = read_city(city_buckets, stdin);
    printf("Next enter a city as your destination.\n");
    dest_city = read_city(city_buckets, stdin);
    while(dest_city == cur_city) {
        printf("You have to select a different city.\n");
        dest_city = read_city(city_buckets, stdin);
    }
    printf("\nYour aim is to travel from %s to %s using steps as small as possible.\n",
        start_city->name, dest_city->name);

    /* game loop */
    while(cur_city != dest_city) {
        printf("You are now located in %s.\n", cur_city->name);
        next_city = read_city(city_buckets, stdin);
        if(next_city == cur_city) {
            printf("You have to select a different city.\n");
            continue;
        }
        cur_distance = dist_vincenty(cur_city, next_city);
        if(cur_distance > max_distance) {
            max_distance = cur_distance;
            printf("New maximum distance is %.1f km.\n", max_distance);
        } else {
            printf("Distance is %.1f km, maximum distance is %.1f km.\n", cur_distance, max_distance);
        }
        cur_city = next_city;
    }
    /* game end */
    printf("Congratulations, you reached the destination!\n");
    printf("Your maximum distance is %.1f km. %s\n", max_distance, rating[(int)log(max_distance)]);

    /* clear up */
    for(i = 0; i < BUCKET_COUNT; i++) {
        city_list_free(city_buckets[i]);
    }
    return 0;
}

