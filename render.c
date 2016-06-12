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

// called when screen is initialized or after backspacing back to beginning of password
void idle(cairo_t* cr)
{
}

// called when ascii key is pressed, calling function (below) also has access to pressed KeySym
void keypress(cairo_t *cr)
{
}

// called when password entered is incorrect
void incorrect(cairo_t *cr)
{
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



