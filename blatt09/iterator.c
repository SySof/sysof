#include "iterator.h"

int get_iter_count(long double _Complex c , int max_iter_count) {
    long double _Complex z = 0;
    for(int i = 0; i < max_iter_count; i++) {
        z = (cabsl(creall(z)) - cabsl(cimagl(z)) * I) *  (cabsl(creall(z)) - cabsl(cimagl(z)) * I)  - c;
        if(cabsl(z)> 2) {
            return i;
        }
    }
    return 0;
}