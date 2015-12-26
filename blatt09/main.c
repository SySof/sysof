#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "output.h"

char* usage =  "Usage: [-x Re(c)] [-y Im(c)] [-d Re(cmax -cmin)] [-w width] [-h height] [-i maximal iteration count]\n";

int main(int argc, char** argv){
    int c;
    long double x = 2.5;
    long double y = 2.5;
    long double d = 5;
    int w = 800;
    int h = 600;
    int i = 200;
    while((c = getopt(argc, argv, "x:y:w:h:d:i:")) != -1) {
        switch(c) {
            case 'x':
                x = atof(optarg);
                break;
            case 'y':
                y = atof(optarg);
                break;
            case 'd':
                d = atof(optarg);
                break;
            case 'w':
                w = atoi(optarg);
                break;
            case 'h':
                h = atoi(optarg);
                break;
            case 'i':
                i = atoi(optarg);
                break;
            case ':':
                switch(optopt) {
                    case '?':
                        printf(usage);
                        return  1;
                    default:
                        break;
                }
                break;
            default:
                printf(usage);
                return 1;
        }
    }
    output_image(x, y, d, w, h, i);
}
