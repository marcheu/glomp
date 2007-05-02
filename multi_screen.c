
#include "init.h"
#include "multi_screen.h"
#include <X11/extensions/xf86vmode.h>

void GLOMP_multi_screen_init()
{
	// nothing needed here
}

typedef struct {
    Display *dpy;
    int screen;
    Window win;
    GLXContext ctx;
    XSetWindowAttributes attr;
    Bool fs;
    Bool doubleBuffered;
    XF86VidModeModeInfo deskMode;
    int x, y;
    unsigned int width, height;
    unsigned int depth;    
} GLWindow;

GLWindow GLWin;

/* attributes for a double buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 4, 
    GLX_GREEN_SIZE, 4, 
    GLX_BLUE_SIZE, 4, 
    GLX_DEPTH_SIZE, 16,
    None };

void GLOMP_multi_screen_init_window()
{
	if (client_num>0)
	{
		// create a window
		// voir nehe lesson 02
		XVisualInfo *vi;
		Colormap cmap;
		int dpyWidth, dpyHeight;
		int i;
		int glxMajorVersion, glxMinorVersion;
		int vidModeMajorVersion, vidModeMinorVersion;
		XF86VidModeModeInfo **modes;
		int modeNum;
		int bestMode;
		Window winDummy;
		unsigned int borderDummy;
		
		printf("creating win\n");
		GLWin.fs = 1;
		/* set best mode to current */
		bestMode = 0;
		/* get a connection */
		GLWin.dpy = XOpenDisplay(0);
		GLWin.screen = DefaultScreen(GLWin.dpy);
		XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion,
				&vidModeMinorVersion);
		printf("XF86VidModeExtension-Version %d.%d\n", vidModeMajorVersion,
				vidModeMinorVersion);
		XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);
		/* save desktop-resolution before switching modes */
		GLWin.deskMode = *modes[0];
		/* look for mode with requested resolution */
		for (i = 0; i < modeNum; i++)
		{
			if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
			{
				bestMode = i;
			}
		}
		/* get an appropriate visual */
		vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListDbl);

		glXQueryVersion(GLWin.dpy, &glxMajorVersion, &glxMinorVersion);
		printf("glX-Version %d.%d\n", glxMajorVersion, glxMinorVersion);
		/* create a GLX context */
		GLWin.ctx = glXCreateContext(GLWin.dpy, vi, 0, GL_TRUE);
		/* create a color map */
		cmap = XCreateColormap(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
				vi->visual, AllocNone);
		GLWin.attr.colormap = cmap;
		GLWin.attr.border_pixel = 0;

		XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, modes[bestMode]);
		XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
		dpyWidth = modes[bestMode]->hdisplay;
		dpyHeight = modes[bestMode]->vdisplay;
		printf("Resolution %dx%d\n", dpyWidth, dpyHeight);
		XFree(modes);

		/* create a fullscreen window */
		GLWin.attr.override_redirect = True;
		GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
			StructureNotifyMask;
		GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
				0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
				&GLWin.attr);
		XWarpPointer(GLWin.dpy, None, GLWin.win, 0, 0, 0, 0, 0, 0);
		XMapRaised(GLWin.dpy, GLWin.win);
		XGrabKeyboard(GLWin.dpy, GLWin.win, True, GrabModeAsync,
				GrabModeAsync, CurrentTime);
		XGrabPointer(GLWin.dpy, GLWin.win, True, ButtonPressMask,
				GrabModeAsync, GrabModeAsync, GLWin.win, None, CurrentTime);

		/* connect the glx-context to the window */
		glXMakeCurrent(GLWin.dpy, GLWin.win, GLWin.ctx);
		XGetGeometry(GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
				&GLWin.width, &GLWin.height, &borderDummy, &GLWin.depth);
		printf("Depth %d\n", GLWin.depth);
	}
}

void GLOMP_multi_screen_swap(Display *dpy, GLXDrawable drawable)
{
	lib_glXSwapBuffers(dpy, drawable);
}

