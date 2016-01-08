#include <math.h>
#include "output.h"
#include "colorize.h"

void output_image(long double x, long double y, long double hwidth,
    int width, int height, int max_iter_count) {
    GdkPixbuf* image = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    int n_channels = gdk_pixbuf_get_n_channels(image);
    int rowstride = gdk_pixbuf_get_rowstride(image);
    guchar * pixels = gdk_pixbuf_get_pixels(image);
    #pragma openmp parallel for
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            guchar* pp = pixels + i * rowstride + j * n_channels;
            long double cur_x = (long double) j / (long double) width * hwidth - x;
            long double cur_y = ((long double) i / (long double) height * hwidth
                * height / width - y );
            long double _Complex c = cur_x + cur_y * I;
            double iter_value = get_iter_value(c, max_iter_count);
            get_color(pp, iter_value, max_iter_count);
        }
    }
    gdk_pixbuf_save(image, "fractal.png", "png", NULL, NULL, NULL);
}
