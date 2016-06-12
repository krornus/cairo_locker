#define _XOPEN_SOURCE 500

#include <shadow.h>
#include <ctype.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "cairo_locker.h"

#define PASSWD_LEN 256

int main(int argc, char ** argv);
int handle_input(KeySym ksym, char * buf, char * passwd, int * len);
Window init_screen(Display *display, int screen_index);
void clear_screens();
renderer_t init_cairo_renderer(Display * display, int screen_index);
int check_passwd(char * passwd);

void cairo_draw_default(void);
void cairo_draw_keypressed(KeySym ksym);
void cairo_draw_passwd_incorrect(void);

Display * display;
renderer_t * renderers;
int screen_count;
XImage *desktop;

int main(int argc, char ** argv)
{
    int authorized;
    char input_passwd[PASSWD_LEN]; 
    char buf[PASSWD_LEN];
    int len;
    int rcount;
    KeySym ksym;

    if((display = XOpenDisplay(NULL)) == NULL)
    {
        printf("Could not open xdisplay\n");
        exit(1);
    }

    screen_count = ScreenCount(display);
    renderers = malloc(sizeof(renderer_t)*screen_count);


    for(int sc = 0; sc < screen_count; sc++)
    {
        renderers[sc] = init_cairo_renderer(display, sc); 
    }

    XSync(display, False);
    XEvent event;

    authorized = 0;
    len = 0;

    cairo_draw_default();

    while(!authorized && !XNextEvent(display, &event))
    {
        rcount = XLookupString(&event.xkey, buf, sizeof(buf), &ksym, 0);
        switch(event.type)
        {
            case KeyPress:
                {

                    authorized = handle_input(ksym, buf, input_passwd, &len);
                    break;
                }
            default:
                {
                    for(int sc = 0; sc < screen_count; sc++)
                    {
                        XRaiseWindow(display, renderers[sc].window); 
                    }
                    break;
                }
        }

    }

    clear_screens();
    free(renderers);
}

int handle_input(KeySym ksym, char * buf, char * passwd, int * len)
{

    if(*len >= PASSWD_LEN - 1)
    {
        printf("passwd exceeded max len");
        return 0;
    }

    if (IsKeypadKey(ksym))
    {
        if (ksym == XK_KP_Enter)
            ksym = XK_Return;
        else if (ksym >= XK_KP_0 && ksym <= XK_KP_9)
            ksym = (ksym - XK_KP_0) + XK_0;

    }

    switch(ksym)
    {

        case XK_Return:
            {
                passwd[*len] = '\0';

                if(check_passwd(passwd) == 0)
                {
                    return 1;
                }
                else
                {
                   cairo_draw_passwd_incorrect();
                }

                int i = 0; 
                *len = 0;
                while(passwd[i] != '\0')
                {
                    passwd[i] = '\0';
                    i++;
                }

                break;
            }
        case XK_BackSpace:
            {
                if(*len > 0)
                {
                    (*len)--;
                }

                if(*len == 0)
                {
                   cairo_draw_default();
                }
                break;
            }
        default:
            {
                if(!isascii(ksym))
                    break;
                
                cairo_draw_keypressed(ksym);
                memcpy(passwd + *len, buf, 1); 
                *len += 1; 

                break;
            }
    }

    return 0;
}

int check_passwd(char * passwd)
{
    const char * pw;
    struct spwd *sp;
    sp = getspnam(getenv("USER"));

    if(!sp)
    {
        printf("null shadow entry, suid and uuid\n");
        return 0;
    }

    endspent();

    pw = sp->sp_pwdp;

    return strcmp(crypt(passwd, pw), pw);
}



Window init_screen(Display *display, int screen_index)
{
    Window window, root;
    XSetWindowAttributes attr;
    int x;
    int y;
    int len;

    root = RootWindow(display, screen_index);

    attr.override_redirect = 1;
    attr.background_pixel = BlackPixel(display, screen_index);

    x = DisplayWidth(display, screen_index);
    y = DisplayHeight(display, screen_index);

    window = XCreateWindow(display, root, 0, 0, x, y, 0, 
            DefaultDepth(display, screen_index), CopyFromParent, 
            DefaultVisual(display, screen_index), CWOverrideRedirect | CWBackPixel, 
            &attr);
    

    XMapRaised(display, window);

    len = 1000;

    if(len > 0)
    {
        for(len = 1000; len; len--)
        {
            if(XGrabKeyboard(display, root, True, GrabModeAsync, GrabModeAsync, CurrentTime) == GrabSuccess)
            {
                break;
                usleep(1000);
            }  
        }
    }

    XSelectInput(display, root, SubstructureNotifyMask);

    return window;
}

renderer_t init_cairo_renderer(Display* display, int screen_index)
{
    renderer_t renderer;
    cairo_surface_t * surface;
    Screen * screen;
    int x;
    int y;

    renderer.window = init_screen(display, screen_index);
    screen = ScreenOfDisplay(display, screen_index);
    x = screen -> width;
    y = screen -> height;

    renderer.pixmap = XCreatePixmap(display, DefaultRootWindow(display), x, y, DefaultDepth(display, screen_index));
    surface = cairo_xlib_surface_create(display, renderer.pixmap, DefaultVisual(display, screen_index), x, y);
    renderer.cr = cairo_create(surface);


    return renderer;
}

void clear_screens()
{
    for(int sc = 0; sc < screen_count; sc++)
    {
        XDestroyWindow(display, renderers[sc].window);
    }
}

