#include "output.h"

void output_image (long double x, long double y, long double hwidth, int width, int height, int iter_count) {
    GdkPixbuf* image = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    int n_channels = gdk_pixbuf_get_n_channels(image);
    int rowstride = gdk_pixbuf_get_rowstride(image);
    guchar * pixels = gdk_pixbuf_get_pixels(image);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            guchar* pp = pixels + i * rowstride + j * n_channels;
            long double _Complex c = (long double) j / (long double) width * hwidth - x + ((long double) i / (long double) height * hwidth * height / width - y ) * I;
            int max_iter_count = cabsl(c) >= 2 ? 1 : get_iter_count(c, iter_count);
            if(!max_iter_count) {
                pp[0] = 255;
                pp[1] = 0;
                pp[2] = 0;
            } else {
                pp[0] = (float) max_iter_count / (float) iter_count * 255;
                pp[1] = 0;
                pp[2] = 255 - (float) max_iter_count / (float) iter_count * 255;
            }
        }
    }
    gdk_pixbuf_save(image, "fractal.png", "png", NULL, NULL, NULL);
}