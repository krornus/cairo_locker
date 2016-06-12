#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "cairo_locker.h"

void cairo_draw_default(void);
void cairo_draw_keypressed(KeySym ksym);
void cairo_draw_passwd_incorrect(void);

void idle(cairo_t* cr);
void keypress(cairo_t *cr);
void incorrect(cairo_t *cr);

extern renderer_t *renderers;
extern Display *display;
extern int screen_count;

void idle(cairo_t* cr)
{
    cairo_set_source_rgb(cr, .0588, .4235, .7411);
    cairo_paint(cr);
}

void keypress(cairo_t *cr)
{
    cairo_set_source_rgb(cr, .0588, .4235, .7411);
    cairo_paint(cr);
}

void incorrect(cairo_t *cr)
{
    cairo_set_source_rgb(cr, .0588, .4235, .7411);
    cairo_paint(cr);
}


void cairo_draw_default()
{
    for (int i = 0; i < screen_count; ++i) 
    {
        idle(renderers[i].cr);
        XSetWindowBackgroundPixmap(display, renderers[i].window, renderers[i].pixmap);
        XClearWindow(display, renderers[i].window);
    }
}

void cairo_draw_keypressed(KeySym ksym)
{
    for(int i = 0; i < screen_count; i++)
    {
        keypress(renderers[i].cr);
        XSetWindowBackgroundPixmap(display, renderers[i].window, renderers[i].pixmap);
        XClearWindow(display, renderers[i].window);
    }
}

void cairo_draw_passwd_incorrect()
{

    for(int i = 0; i < screen_count; i++)
    {
        incorrect(renderers[i].cr);
        XSetWindowBackgroundPixmap(display, renderers[i].window, renderers[i].pixmap);
        XClearWindow(display, renderers[i].window);
    }
}



