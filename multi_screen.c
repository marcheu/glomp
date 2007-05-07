
#include "init.h"
#include "multi_screen.h"
#include "lib_funcs.h"
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

static GLWindow GLWin;

void GLOMP_multi_screen_init_window(int* glxattribs)
{
	if (client_num>0)
	{
		// create a window
		// voir nehe lesson 02
		XVisualInfo *vi;
		Colormap cmap;
		int glxMajorVersion, glxMinorVersion;
		int vidModeMajorVersion, vidModeMinorVersion;
		XF86VidModeModeInfo **modes;
		int modeNum;
		char envDisplay[16];

		printf("creating win\n");

		sprintf(envDisplay,":0.%d",client_num);
		if (getenv("FORCE_GPU"))
			sprintf(envDisplay,":0.0");

		GLWin.fs = 0;
		/* get a connection */
		GLWin.dpy = lib_XOpenDisplay(envDisplay);
		GLWin.screen = DefaultScreen(GLWin.dpy);
		lib_XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion,
				&vidModeMinorVersion);
		printf("XF86VidModeExtension-Version %d.%d\n", vidModeMajorVersion,
				vidModeMinorVersion);
		lib_XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);
		/* use the desktop resolution */ 
		GLWin.deskMode = *modes[0];
		if (!getenv("FORCE_GPU"))
		{
			width=modes[0]->hdisplay;
			height=modes[0]->vdisplay;
		}
		/* get an appropriate visual */
		vi = lib_glXChooseVisual(GLWin.dpy, GLWin.screen, glxattribs);

		lib_glXQueryVersion(GLWin.dpy, &glxMajorVersion, &glxMinorVersion);
		printf("glX-Version %d.%d\n", glxMajorVersion, glxMinorVersion);
		/* create a GLX context */
		GLWin.ctx = lib_glXCreateContext(GLWin.dpy, vi, 0, GL_TRUE);
		/* create a color map */
		cmap = lib_XCreateColormap(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
				vi->visual, AllocNone);
		GLWin.attr.colormap = cmap;
		GLWin.attr.border_pixel = 0;

		/* create a fullscreen window */
		GLWin.attr.override_redirect = True;
		GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
			StructureNotifyMask;
		if (!getenv("FORCE_GPU"))
			GLWin.win = lib_XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
				0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect, &GLWin.attr);
		else
			GLWin.win = lib_XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
				0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask, &GLWin.attr);
		/* only set window title and handle wm_delete_events if in windowed mode */
		//wmDelete = XInternAtom(GLWin.dpy, "WM_DELETE_WINDOW", True);
		//XSetWMProtocols(GLWin.dpy, GLWin.win, &wmDelete, 1);
		//XSetStandardProperties(GLWin.dpy, GLWin.win, title,
		//		title, None, NULL, 0, NULL);
		lib_XMapRaised(GLWin.dpy, GLWin.win);

		/* connect the glx-context to the window */
		lib_glXMakeCurrent(GLWin.dpy, GLWin.win, GLWin.ctx);
	}
}

void GLOMP_multi_screen_swap(Display *dpy, GLXDrawable drawable)
{
	if (client_num>0)
		lib_glXSwapBuffers(GLWin.dpy, GLWin.win);
	else
		lib_glXSwapBuffers(dpy, drawable);
}

void GLOMP_multi_screen_close()
{
	lib_XCloseDisplay(GLWin.dpy);
}

