#include "iterator.h"

#define MAX_ABS 2

double get_iter_value(long double _Complex c, int max_iter_count) {
    long double _Complex z = 0;
    int i = 0;
    long double cur_abs = cabsl(z), last_abs = cur_abs;
    while(cur_abs < MAX_ABS && i < max_iter_count) {
        z = (cabsl(creall(z)) - cabsl(cimagl(z)) * I)
            * (cabsl(creall(z)) - cabsl(cimagl(z)) * I)
            - c;
        last_abs = cur_abs;
        cur_abs = cabsl(z);
        i++;
    }
    double interpolated_i = i;
    if(cur_abs != last_abs && i != max_iter_count) {
        interpolated_i += ((MAX_ABS - last_abs) / (cur_abs - last_abs));
    }
    double iter_value = interpolated_i / max_iter_count;
    return iter_value;
}
