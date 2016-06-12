#include <cairo.h>

typedef struct renderer {
    Window window;
    cairo_t * cr;
    cairo_surface_t *desktop;
    Pixmap pixmap;
} renderer_t;
