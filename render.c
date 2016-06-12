#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "cairo_locker.h"

void cairo_draw_default(void);
void cairo_draw_keypressed(KeySym ksym);
void cairo_draw_passwd_incorrect(void);

extern renderer_t *renderers;
extern Display *display;
extern int screen_count;

void cairo_draw_default()
{
    for (int i = 0; i < screen_count; ++i) 
    {
        cairo_set_source_rgb(renderers[i].cr, 0.07, 0.11, 0.21);
        cairo_paint(renderers[i].cr);

        XSetWindowBackgroundPixmap(display, renderers[i].window, renderers[i].pixmap);
        XClearWindow(display, renderers[i].window);
    }
}

void cairo_draw_keypressed(KeySym ksym)
{
    for(int i = 0; i < screen_count; i++)
    {
        cairo_set_source_rgb(renderers[i].cr, 0.5, 0.5, 0.5);
        cairo_paint(renderers[i].cr);

        XSetWindowBackgroundPixmap(display, renderers[i].window, renderers[i].pixmap);
        XClearWindow(display, renderers[i].window);
    }
}

void cairo_draw_passwd_incorrect()
{

    for(int i = 0; i < screen_count; i++)
    {
        cairo_set_source_rgb(renderers[i].cr, 0.78, 0.04, 0.04);
        cairo_paint(renderers[i].cr);

        XSetWindowBackgroundPixmap(display, renderers[i].window, renderers[i].pixmap);
        XClearWindow(display, renderers[i].window);
    }
}
