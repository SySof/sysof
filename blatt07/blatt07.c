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
    double a, b, f, L, U1, U2, sinU1, cosU1, sinU2, cosU2, lambda, lambdaP;
    double sinLambda, cosLambda, sinSigma, cosSigma, sigma, sinAlpha;
    double cosSqAlpha, cos2SigmaM, C, uSq, A, B, deltaSigma, s;
    unsigned int iterLimit;

    a = 6378137;
    b = 6356752.314245;
    f = 1 / 298.257223563;

    L = to_rad(lon2 - lon1);
    U1 = atan((1 - f) * tan(to_rad(lat1)));
    U2 = atan((1 - f) * tan(to_rad(lat2)));
    sinU1 = sin(U1);
    cosU1 = cos(U1);
    sinU2 = sin(U2);
    cosU2 = cos(U2);

    lambda = L;
    iterLimit = 100;
    do {
        sinLambda = sin(lambda),
        cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2 * sinLambda) * (cosU2 * sinLambda) +
            (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda) * (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda));
        if(sinSigma == 0) {
            return 0; /* co-incident points */
        }
        cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1 - sinAlpha * sinAlpha;
        cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;
        if(cos2SigmaM != cos2SigmaM) { /* NaN */
            cos2SigmaM = 0; /* equatorial line: cosSqAlpha=0 (§6) */
        }
        C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1 - C) * f * sinAlpha * (sigma + C * sinSigma * (cos2SigmaM + C * cosSigma * (-1 + 2 * cos2SigmaM * cos2SigmaM)));
    } while(abs(lambda - lambdaP) > 1e-12 && --iterLimit > 0);

    if(iterLimit == 0) {
        return .0/.0; /* formula failed to converge */
    }
    uSq = cosSqAlpha * (a * a - b * b) / (b * b);
    A = 1 + uSq / 16384 * (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
    B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
    deltaSigma = B * sinSigma * (cos2SigmaM + B / 4 * (cosSigma *
        (-1 + 2 * cos2SigmaM * cos2SigmaM) - B / 6 * cos2SigmaM * (-3 + 4 * sinSigma * sinSigma) *
        (-3 + 4 * cos2SigmaM * cos2SigmaM)));
    s = b * A * (sigma - deltaSigma);

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

