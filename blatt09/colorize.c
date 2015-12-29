#include "colorize.h"
#include <math.h>

/*void blend_colors(guchar *a, guchar *b, float a_percent) {
    b[0] = sqrt(a_percent*a[0]*a[0] + (1 - a_percent)*b[0]*b[0]);
    b[1] = sqrt(a_percent*a[1]*a[1] + (1 - a_percent)*b[1]*b[1]);
    b[2] = sqrt(a_percent*a[2]*a[2] + (1 - a_percent)*b[2]*b[2]);
}*/

void get_color(unsigned char* color, double value) {
    value = 0.2*log(value);
    //~ value = value > 1 ? 1 : value;
    //~ value = value < 0 ? 0 : value;
    value = fabs(value);
    value -= (int) value;
    if(value == 0) {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    } else {
        color[0] = value * 255;
        color[1] = 0;
        color[2] = 255 - value * 255;
    }
    /*if(j > 3&& i > 3 && j < width - 3 && i < height - 3) {
        for (int k = -3; k <= 3; k++) {
            for (int l = -3; l <= 3; l++) {
                if((k == 0 && l ==0) || sqrt(k*k + l*l) > 3) {
                    continue;
                }
                blend_colors(color, pixels + (i + k) * rowstride + (j + l)
                    * n_channels, (4 - fabs(sqrt(k*k + l*l))) / 4.0);
            }
        }
    }*/
}
