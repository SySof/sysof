#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define buflen 128

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
        if(sscanf(buffer, "%63[^:]:%lf:%lf", cur_city.name, &cur_city.latitude, &cur_city.longitude) == 3) {
            cur_city.next = city_list_prepend(cur_city.next);
            strcpy_encode((unsigned char*)cur_city.next->name, (unsigned char*)cur_city.name);
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
    struct city *city_list, *city_list2;

    srand(time(NULL));
    if(!(city_list = city_list_read("gemeinden.txt"))) {
        printf("Error reading file!\n");
        return 1;
    }

    for(city_list2 = city_list; city_list2 && city_list2->next; city_list2 = city_list2->next) {
        printf("Distance between %s and %s: %.1f km\n", city_list2->name, city_list2->next->name,
            dist_vincenty(city_list2, city_list2->next));
    }

    city_list_free(city_list);
    return 0;
}

