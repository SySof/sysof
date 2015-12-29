#include "iterator.h"

double get_iter_value(long double _Complex c, int max_iter_count) {
    long double _Complex z = 0;
    int i = 0;
    while(cabsl(z) < 2 && i < max_iter_count) {
        z = (cabsl(creall(z)) - cabsl(cimagl(z)) * I)
            * (cabsl(creall(z)) - cabsl(cimagl(z)) * I)
            - c;
        i++;
    }
    double iter_value = ((double) i) / max_iter_count;
    return iter_value;
}
