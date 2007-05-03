#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib_funcs.h"

/* handles for dlopen/dlclose */
static void* lib_handle_libGL = 0;
static void* lib_handle_libX11 = 0;
static void* lib_handle_libXxf86vm = 0;
static void* lib_handle_libC = 0;

/* GLX stuff */
PFNGLXGETPROCADDRESSARBPROC lib_glXGetProcAddressARB=0;
void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable)=0;
GLXWindow (*lib_glXCreateWindow) (Display *dpy, GLXFBConfig config,
 				 Window win, const int *attrib_list);
char* (*lib_glXGetClientString)(Display *, int)=0;
char* (*lib_glXQueryServerString)(Display *dpy, int screen, int name)=0;
GLXFBConfig* (*lib_glXChooseFBConfig)(Display*,int,int*,int*)=0;
int (*lib_glXGetFBConfigAttrib)(Display *dpy, GLXFBConfig config,
                                int attribute, int *value)=0;
GLXPbuffer (*lib_glXCreatePbuffer)(Display *dpy, GLXFBConfig config,
                                   const int *attrib_list)=0;
GLXContext (*lib_glXCreateNewContext)(Display *dpy, GLXFBConfig config,
                                      int render_type, GLXContext share_list,
                                      Bool direct)=0;
Bool (*lib_glXIsDirect)(Display *dpy, GLXContext ctx)=0;
Bool (*lib_glXMakeCurrent)(Display *dpy, GLXDrawable drawable,
                           GLXContext ctx)=0;
XVisualInfo* (*lib_glXChooseVisual)(Display *dpy, int screen,int *attrib_list)=0;
Bool (*lib_glXQueryVersion)(Display *dpy, int *major, int *minor)=0;
GLXContext (*lib_glXCreateContext)(Display *dpy, XVisualInfo *vis,GLXContext share_list, Bool direct)=0;



/* GL stuff */
void (*lib_glBindTexture) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenTextures) ( GLsizei p0 , GLuint *p1 )=0;	
void (*lib_glFrustum) ( GLdouble left,
			       GLdouble right,
			       GLdouble bottom,
			       GLdouble top,
			       GLdouble zNear,
			       GLdouble zFar	)=0;
void (*lib_glOrtho) ( GLdouble left,
			     GLdouble right,
			     GLdouble bottom,
			     GLdouble top,
			     GLdouble zNear,
			     GLdouble zFar	)=0;
GLuint (*lib_glGenLists)( GLsizei range )=0;
void (*lib_glNewList) (GLuint list,GLenum mode)=0;
void (*lib_glCallList) (GLuint list)=0;
void (*lib_glCallLists) (GLsizei n, GLenum type, const GLvoid *lists)=0;
void (*lib_glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)=0;
void (*lib_glCopyTexImage1D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)=0;
void (*lib_glCopyTexImage2D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)=0;
void (*lib_glCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)=0;
void (*lib_glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)=0;
void (*lib_glBindTextureEXT) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenTexturesEXT) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glGenQueries) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glBindBuffer) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenBuffers) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glBindProgramARB) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenProgramsARB) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glBindBufferARB) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenBuffersARB) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glGenQueriesARB) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glGenFencesNV) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glBindProgramNV) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenProgramsNV) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glGenOcclusionQueriesNV) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glBindRenderbufferEXT) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenRenderbuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glBindFramebufferEXT) ( GLenum p0 , GLuint p1 )=0;
void (*lib_glGenFramebuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteTextures) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glFlush) ()=0;
void (*lib_glFinish) ()=0;
void (*lib_glDeleteQueries) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteBuffers) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteProgramsARB) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteBuffersARB) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteQueriesARB) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteTexturesEXT) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteFencesNV) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteProgramsNV) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteOcclusionQueriesNV) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteRenderbuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glDeleteFramebuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
void (*lib_glGetIntegerv)(GLenum p0,GLint *p1)=0;
const GLubyte* (*lib_glGetString)( GLenum name )=0;
void (*lib_glMatrixMode) (GLenum mode)=0;
void (*lib_glPushMatrix) (void)=0;
void (*lib_glPopMatrix) (void)=0;
void (*lib_glLoadIdentity) (void)=0;
void (*lib_glViewport)( GLint x,GLint y,GLsizei width,GLsizei height)=0;
void (*lib_glRasterPos2i)( GLint x,GLint y)=0;
void (*lib_glRasterPos2f)( GLfloat x,GLfloat y)=0;
void (*lib_glDrawPixels)( GLsizei width,
			  GLsizei height,
			  GLenum format,
			  GLenum type,
			  const GLvoid *pixels )=0;
void (*lib_glReadPixels)( GLint x,
			  GLint y,
			  GLsizei width,
			  GLsizei height,
			  GLenum format,
			  GLenum type,
			  GLvoid *pixels )=0;
void (*lib_glMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points)=0;
void (*lib_glMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)=0;
void (*lib_glMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points)=0;
void (*lib_glMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points)=0;
GLboolean (*lib_glIsEnabled) (GLenum cap)=0;


/* X stuff */
Window (*lib_XCreateWindow)(Display *display, Window parent, int x, int y,
 			   unsigned int width, unsigned int height,
 			   unsigned int border_width, int depth,
 			   unsigned int class, Visual *visual,
 			   unsigned long valuemask, XSetWindowAttributes *attributes)=0;
int (*lib_XDestroyWindow)( Display * disp, Window parent)=0;
Display* (*lib_XOpenDisplay)(char*)=0;
int (*lib_XCloseDisplay)(Display * dis)=0;
int (*lib_XSetStandardProperties)(
					 Display*		/* display */,
					 Window		/* w */,
					 _Xconst char*	/* window_name */,
					 _Xconst char*	/* icon_name */,
					 Pixmap		/* icon_pixmap */,
					 char**		/* argv */,
					 int			/* argc */,
					 XSizeHints*		/* hints */
					 )=0;
int (*lib_XFree) (void *)=0;
int (*lib_XMapRaised)(Display*,Window)=0;
Colormap (*lib_XCreateColormap)(Display*,Window,Visual*,int)=0;

/* XF86vm stuff */
Bool (*lib_XF86VidModeQueryVersion)(Display*,int*,int*)=0;
Bool (*lib_XF86VidModeGetAllModeLines)(Display*,int,int*,XF86VidModeModeInfo***)=0;
Bool (*lib_XF86VidModeSwitchToMode)(Display*,int,XF86VidModeModeInfo*)=0;
Bool (*lib_XF86VidModeSetViewPort)(Display*,int,int,int)=0;

/* libc stuff */
void * (*lib_strstr)(char*,char*)=0;





/*
 * Load GLX & X library and find glXSwapBuffers() & other function
 */
void lib_funcs_init()
{
	static int loaded=0;
	if (loaded)
		return;
	loaded=1;

	/* open library (NOTE: you may need to change this filename!) */
	lib_handle_libGL = dlopen("/usr/lib/libGL.so", RTLD_LAZY);

	if (!lib_handle_libGL){perror("lib");exit(0);}

	/* intercept library GL function */
	lib_glXGetProcAddressARB = dlsym(lib_handle_libGL, "glXGetProcAddressARB");
	lib_glXSwapBuffers = dlsym(lib_handle_libGL, "glXSwapBuffers");
	lib_glXCreateWindow = dlsym(lib_handle_libGL, "glXCreateWindow");   
	lib_glXQueryServerString = dlsym(lib_handle_libGL, "glXQueryServerString");
	/* FIXME */
	lib_glXChooseFBConfig = lib_glXGetProcAddressARB("glXChooseFBConfig");
	lib_glXGetFBConfigAttrib = dlsym(lib_handle_libGL, "glXGetFBConfigAttrib");
	lib_glXCreatePbuffer = dlsym(lib_handle_libGL, "glXCreatePbuffer");
	lib_glXCreateNewContext = dlsym(lib_handle_libGL, "glXCreateNewContext");
	lib_glXIsDirect = dlsym(lib_handle_libGL, "glXIsDirect");
	lib_glXMakeCurrent = dlsym(lib_handle_libGL, "glXMakeCurrent");
	lib_glXChooseVisual = dlsym(lib_handle_libGL, "glXChooseVisual");
	lib_glXQueryVersion = dlsym(lib_handle_libGL, "glXQueryVersion");
	lib_glXCreateContext = dlsym(lib_handle_libGL, "glXCreateContext");

	/* intercept library GL function */
	lib_glBindTexture = dlsym(lib_handle_libGL, "glBindTexture");
	lib_glGenTextures = dlsym(lib_handle_libGL, "glGenTextures");    
	lib_glFrustum = dlsym(lib_handle_libGL, "glFrustum"); 
	lib_glOrtho = dlsym(lib_handle_libGL, "glOrtho"); 
	lib_glNewList= dlsym(lib_handle_libGL, "glNewList");
	lib_glGenLists= dlsym(lib_handle_libGL, "glGenLists");
	lib_glCallList= dlsym(lib_handle_libGL, "glCallList");
	lib_glCallLists= dlsym(lib_handle_libGL, "glCallLists");
	lib_glCopyPixels= dlsym(lib_handle_libGL, "glCopyPixels");
	lib_glCopyTexImage1D= dlsym(lib_handle_libGL, "glCopyTexImage1D");
	lib_glCopyTexImage2D= dlsym(lib_handle_libGL, "glCopyTexImage2D");
	lib_glCopyTexSubImage1D= dlsym(lib_handle_libGL, "glCopyTexSubImage1D");
	lib_glCopyTexSubImage2D= dlsym(lib_handle_libGL, "glCopyTexSubImage2D");
	lib_glDeleteTextures= dlsym(lib_handle_libGL, "glDeleteTextures");
	lib_glFlush= dlsym(lib_handle_libGL, "glFlush");
	lib_glFinish= dlsym(lib_handle_libGL, "glFinish");
	lib_glGetIntegerv=dlsym(lib_handle_libGL, "glGetIntegerv");
	lib_glGetString=dlsym(lib_handle_libGL, "glGetString");
	lib_glViewport=dlsym(lib_handle_libGL, "glViewport");
	lib_glRasterPos2f = dlsym(lib_handle_libGL, "glRasterPos2f");
	lib_glRasterPos2i = dlsym(lib_handle_libGL, "glRasterPos2i");
	lib_glDrawPixels = dlsym(lib_handle_libGL, "glDrawPixels");
	lib_glReadPixels = dlsym(lib_handle_libGL, "glReadPixels");  
	lib_glMatrixMode = dlsym(lib_handle_libGL, "glMatrixMode"); 
	lib_glPushMatrix = dlsym(lib_handle_libGL, "glPushMatrix"); 
	lib_glPopMatrix = dlsym(lib_handle_libGL, "glPopMatrix"); 
	lib_glLoadIdentity = dlsym(lib_handle_libGL, "glLoadIdentity"); 

	/*les extensions*/
	lib_glBindTextureEXT = dlsym(lib_handle_libGL, "glBindTextureEXT");    
	lib_glGenTexturesEXT = dlsym(lib_handle_libGL, "glGenTexturesEXT");
	lib_glGenQueries = dlsym(lib_handle_libGL, "glGenQueries"); 
	lib_glBindBuffer = dlsym(lib_handle_libGL, "glBindBuffer");    
	lib_glGenBuffers = dlsym(lib_handle_libGL, "glGenBuffers");
	lib_glBindProgramARB = dlsym(lib_handle_libGL, "glBindProgramARB");    
	lib_glGenProgramsARB = dlsym(lib_handle_libGL, "glGenProgramsARB");
	lib_glBindBufferARB = dlsym(lib_handle_libGL, "glBindBufferARB");    
	lib_glGenBuffersARB = dlsym(lib_handle_libGL, "glGenBuffersARB");
	lib_glGenQueriesARB = dlsym(lib_handle_libGL, "glGenQueriesARB"); 
	lib_glGenFencesNV = dlsym(lib_handle_libGL, "glGenFencesARB"); 
	lib_glBindProgramNV = dlsym(lib_handle_libGL, "glBindProgramNV");    
	lib_glGenProgramsNV = dlsym(lib_handle_libGL, "glGenProgramsNV");
	lib_glGenOcclusionQueriesNV = dlsym(lib_handle_libGL, "glGenOcclusionQueriesNV");
	lib_glBindRenderbufferEXT = dlsym(lib_handle_libGL, "glBindRenderbufferEXT"); 
	lib_glGenRenderbuffersEXT = dlsym(lib_handle_libGL, "glGenRenderbuffersEXT");
	lib_glBindFramebufferEXT = dlsym(lib_handle_libGL, "glBindFramebufferEXT"); 
	lib_glGenFramebuffersEXT = dlsym(lib_handle_libGL, "glGenFramebuffersEXT");
	lib_glDeleteQueries= dlsym(lib_handle_libGL, "glDeleteQueries");
	lib_glDeleteBuffers= dlsym(lib_handle_libGL, "glDeleteBuffers");
	lib_glDeleteProgramsARB = dlsym(lib_handle_libGL, "glDeleteProgramsARB");
	lib_glDeleteBuffersARB= dlsym(lib_handle_libGL, "glDeleteBuffersARB");
	lib_glDeleteQueriesARB= dlsym(lib_handle_libGL, "glDeleteQueriesARB");
	lib_glDeleteTexturesEXT= dlsym(lib_handle_libGL, "glDeleteTexturesEXT");
	lib_glDeleteFencesNV= dlsym(lib_handle_libGL, "glDeleteFencesNV");
	lib_glDeleteProgramsNV = dlsym(lib_handle_libGL, "glDeleteProgramsNV");
	lib_glDeleteOcclusionQueriesNV = dlsym(lib_handle_libGL, "glDeleteOcclusionQueriesNV");
	lib_glDeleteRenderbuffersEXT = dlsym(lib_handle_libGL, "glDeleteRenderbuffersEXT");
	lib_glDeleteFramebuffersEXT = dlsym(lib_handle_libGL, "glDeleteFramebuffersEXT");
	lib_glMap1f = dlsym(lib_handle_libGL, "glMap1f");
	lib_glMap1d = dlsym(lib_handle_libGL, "glMap1d");
	lib_glMap2f = dlsym(lib_handle_libGL, "glMap2f");
	lib_glMap2d = dlsym(lib_handle_libGL, "glMap2d");
	lib_glIsEnabled = dlsym(lib_handle_libGL, "glIsEnabled");

	/* X11 stuff */
	lib_handle_libX11 = dlopen("/usr/lib/libX11.so", RTLD_LAZY);

	if (!lib_handle_libX11){perror("lib");exit(0);}

	lib_XSetStandardProperties = dlsym(lib_handle_libX11, "XSetStandardProperties");
	lib_XCreateWindow = dlsym(lib_handle_libX11, "XCreateWindow");
	lib_XDestroyWindow= dlsym(lib_handle_libX11, "XDestroyWindow");
	lib_XOpenDisplay = dlsym(lib_handle_libX11, "XOpenDisplay");
	lib_XCloseDisplay = dlsym(lib_handle_libX11, "XCloseDisplay");
	lib_XFree = dlsym(lib_handle_libX11, "XFree");
	lib_XMapRaised = dlsym(lib_handle_libX11, "XMapRaised");
	lib_XCreateColormap = dlsym(lib_handle_libX11, "XCreateColormap");

	/* libXxf86vm stuff */
	lib_handle_libXxf86vm = dlopen("/usr/lib/libXxf86vm.so", RTLD_LAZY);
	lib_XF86VidModeQueryVersion = dlsym(lib_handle_libXxf86vm, "XF86VidModeQueryVersion");
	lib_XF86VidModeGetAllModeLines = dlsym(lib_handle_libXxf86vm, "XF86VidModeGetAllModeLines");
	lib_XF86VidModeSwitchToMode = dlsym(lib_handle_libXxf86vm, "XF86VidModeSwitchToMode");
	lib_XF86VidModeSetViewPort = dlsym(lib_handle_libXxf86vm, "XF86VidModeSetViewPort");

	/* libc stuff */
	lib_handle_libC = dlopen("/lib/libc.so", RTLD_LAZY);  
	lib_strstr=dlsym(lib_handle_libC, "strstr");
}




