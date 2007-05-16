#ifndef _LIB_FUNCS_H_
#define _LIB_FUNCS_H_

#include "glheader.h"

extern void lib_funcs_init();

/* GLX */
extern PFNGLXGETPROCADDRESSARBPROC lib_glXGetProcAddressARB;
extern void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable);
extern GLXWindow (*lib_glXCreateWindow) (Display *dpy, GLXFBConfig config,
 				 Window win, const int *attrib_list);
extern char* (*lib_glXGetClientString)(Display * dpy, int name);
extern const char* (*lib_glXQueryServerString)(Display *dpy, int screen, int name);
extern GLXFBConfig* (*lib_glXChooseFBConfig)(Display*,int,int*,int*);
extern int (*lib_glXGetFBConfigAttrib)(Display *dpy, GLXFBConfig config,
                                int attribute, int *value);
extern GLXPbuffer (*lib_glXCreatePbuffer)(Display *dpy, GLXFBConfig config,
                                   const int *attrib_list);
extern GLXContext (*lib_glXCreateNewContext)(Display *dpy, GLXFBConfig config,
                                      int render_type, GLXContext share_list,
                                      Bool direct);
extern Bool (*lib_glXIsDirect)(Display *dpy, GLXContext ctx);
extern Bool (*lib_glXMakeCurrent)(Display *dpy, GLXDrawable drawable,
                           GLXContext ctx);
extern XVisualInfo* (*lib_glXChooseVisual)(Display *dpy, int screen,int *attrib_list);
extern Bool (*lib_glXQueryVersion)(Display *dpy, int *major, int *minor);
extern GLXContext (*lib_glXCreateContext)(Display *dpy, XVisualInfo *vis,GLXContext share_list, Bool direct);
extern void (*lib_glXDestroyContext)(Display *dpy, GLXContext ctx);
extern void (*lib_glXCopyContext)(Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);

/* GL */
extern void (*lib_glBindTexture) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenTextures) ( GLsizei p0 , GLuint *p1 );	
extern void (*lib_glFrustum) ( GLdouble left,
			       GLdouble right,
			       GLdouble bottom,
			       GLdouble top,
			       GLdouble zNear,
			       GLdouble zFar	);
extern void (*lib_glOrtho) ( GLdouble left,
			     GLdouble right,
			     GLdouble bottom,
			     GLdouble top,
			     GLdouble zNear,
			     GLdouble zFar	);
extern GLuint (*lib_glGenLists)( GLsizei range );
extern void (*lib_glNewList) (GLuint list,GLenum mode);
extern void (*lib_glCallList) (GLuint list);
extern void (*lib_glCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
extern void (*lib_glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern void (*lib_glCopyTexImage1D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
extern void (*lib_glCopyTexImage2D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void (*lib_glCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void (*lib_glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (*lib_glBindTextureEXT) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenTexturesEXT) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glGenQueries) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glBindBuffer) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenBuffers) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glBindProgramARB) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenProgramsARB) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glBindBufferARB) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenBuffersARB) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glGenQueriesARB) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glGenFencesNV) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glBindProgramNV) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenProgramsNV) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glGenOcclusionQueriesNV) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glBindRenderbufferEXT) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenRenderbuffersEXT) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glBindFramebufferEXT) ( GLenum p0 , GLuint p1 );
extern void (*lib_glGenFramebuffersEXT) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteTextures) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glFlush) ();
extern void (*lib_glFinish) ();
extern void (*lib_glDeleteQueries) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteBuffers) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteProgramsARB) ( GLsizei p0 , const GLuint *p1 );
extern void (*lib_glDeleteBuffersARB) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteQueriesARB) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteTexturesEXT) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteFencesNV) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteProgramsNV) ( GLsizei p0 , const GLuint *p1 );
extern void (*lib_glDeleteOcclusionQueriesNV) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteRenderbuffersEXT) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glDeleteFramebuffersEXT) ( GLsizei p0 , GLuint *p1 );
extern void (*lib_glGetIntegerv)(GLenum p0,GLint *p1);
extern const GLubyte* (*lib_glGetString)( GLenum name );
extern void (*lib_glMatrixMode) (GLenum mode);
extern void (*lib_glPushMatrix) (void);
extern void (*lib_glPopMatrix) (void);
extern void (*lib_glLoadIdentity) (void);
extern void (*lib_glLoadMatrixf) (const GLfloat* m);
extern void (*lib_glLoadMatrixd) (const GLdouble* m);
extern void (*lib_glViewport)( GLint x,GLint y,GLsizei width,GLsizei height);
extern void (*lib_glRasterPos2i)( GLint x,GLint y);
extern void (*lib_glRasterPos2f)( GLfloat x,GLfloat y);
extern void (*lib_glDrawPixels)( GLsizei width,
			  GLsizei height,
			  GLenum format,
			  GLenum type,
			  const GLvoid *pixels );
extern void (*lib_glReadPixels)( GLint x,
			  GLint y,
			  GLsizei width,
			  GLsizei height,
			  GLenum format,
			  GLenum type,
			  GLvoid *pixels );
extern void (*lib_glMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void (*lib_glMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void (*lib_glMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void (*lib_glMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern GLboolean (*lib_glIsEnabled) (GLenum cap);
extern void (*lib_glClientActiveTexture) (GLenum);
extern void (*lib_glEnableClientState) (GLenum array);
extern void (*lib_glDisableClientState) (GLenum array);
extern void (*lib_glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (*lib_glWeightPointerARB) (GLint, GLenum, GLsizei, const GLvoid *);
extern void (*lib_glNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (*lib_glColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (*lib_glSecondaryColorPointer) (GLint, GLenum, GLsizei, const GLvoid *);
extern void (*lib_glFogCoordPointer) (GLenum, GLsizei, const GLvoid *);
extern void (*lib_glIndexPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (*lib_glEdgeFlagPointer) (GLsizei stride, const GLvoid *pointer);
extern void (*lib_glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (*lib_glDrawArrays) (GLenum mode, GLint first, GLsizei count);
extern void (*lib_glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);



/* X */
extern Window (*lib_XCreateWindow)(Display *display, Window parent, int x, int y,
 			   unsigned int width, unsigned int height,
 			   unsigned int border_width, int depth,
 			   unsigned int class, Visual *visual,
 			   unsigned long valuemask, XSetWindowAttributes *attributes);
extern int (*lib_XDestroyWindow)( Display * disp, Window parent);
extern Display* (*lib_XOpenDisplay)(char*);
extern int (*lib_XCloseDisplay)(Display * dis);
extern int (*lib_XSetStandardProperties)(
					 Display*		/* display */,
					 Window		/* w */,
					 _Xconst char*	/* window_name */,
					 _Xconst char*	/* icon_name */,
					 Pixmap		/* icon_pixmap */,
					 char**		/* argv */,
					 int			/* argc */,
					 XSizeHints*		/* hints */
					 );
extern int (*lib_XFree) (void *);
extern int (*lib_XMapRaised)(Display*,Window);
extern Colormap (*lib_XCreateColormap)(Display*,Window,Visual*,int);
extern int (*lib_XGrabKeyboard)(
    Display*		/* display */,
    Window		/* grab_window */,
    Bool		/* owner_events */,
    int			/* pointer_mode */,
    int			/* keyboard_mode */,
    Time		/* time */
);


/* XF86vm stuff */
extern Bool (*lib_XF86VidModeQueryVersion)(Display*,int*,int*);
extern Bool (*lib_XF86VidModeGetAllModeLines)(Display*,int,int*,XF86VidModeModeInfo***);
extern Bool (*lib_XF86VidModeSwitchToMode)(Display*,int,XF86VidModeModeInfo*);
extern Bool (*lib_XF86VidModeSetViewPort)(Display*,int,int,int);

/* C */	
extern void * (*lib_strstr)(char*,char*);

#endif

