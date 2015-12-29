#include "colorize.h"
#include <math.h>

unsigned char red[] = {255, 0, 0};
unsigned char green[] = {0, 255, 0};
unsigned char blue[] = {0, 0, 255};
unsigned char cyan[] = {0, 255, 255};
unsigned char magenta[] = {255, 0, 255};
unsigned char yellow[] = {255, 255, 0};
unsigned char black[] = {0, 0, 0};

unsigned char* colors[] = {black, red, green, blue};
int n_colors = sizeof(colors) / sizeof(unsigned char*);

void blend_colors(unsigned char *a, unsigned char *b, unsigned char* c, double value) {
    c[0] = sqrt(value*a[0]*a[0] + (1 - value)*b[0]*b[0]);
    c[1] = sqrt(value*a[1]*a[1] + (1 - value)*b[1]*b[1]);
    c[2] = sqrt(value*a[2]*a[2] + (1 - value)*b[2]*b[2]);
}

void get_color(unsigned char* color, double value, int max_iter_count) {
    if(value == 1) {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    } else {
        value = -0.2*log(value / 200 * max_iter_count);
        // normalize value into [0, 1]:
        value -= (int) value;
        int color_index = (int) (value * (n_colors - 1));
        value *= (n_colors - 1);
        // normalize value into [0, 1]:
        value -= (int) value;
        blend_colors(colors[color_index+1], colors[color_index], color, value);
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
