#include <cairo.h>

typedef struct renderer {
    Window window;
    cairo_t * cr;
    Pixmap pixmap;
} renderer_t;
