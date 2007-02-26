#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include"fifo.h"

static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable)=0;
static int (*lib_XSetStandardProperties)(
    Display*		/* display */,
    Window		/* w */,
    _Xconst char*	/* window_name */,
    _Xconst char*	/* icon_name */,
    Pixmap		/* icon_pixmap */,
    char**		/* argv */,
    int			/* argc */,
    XSizeHints*		/* hints */
)=0;
static GLXWindow (*lib_glXCreateWindow)(Display *dpy, GLXFBConfig config,
			  Window win, const int *attrib_list)=0;

static void (*lib_glBindTexture) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenTextures) ( GLsizei p0 , GLuint *p1 )=0;	
static void (*lib_glFrustum) ( GLdouble left,
			       GLdouble right,
			       GLdouble bottom,
			       GLdouble top,
			       GLdouble zNear,
			       GLdouble zFar	)=0;

// the list of overridden gl function that can't be auto-generated

/* library interception variables */
static void* lib_handle_libGL = 0;
static void* lib_handle_libX11 = 0;

//la structure qui contient le shm servant de fifo
extern uint8_t* cmd_fifo;//le shm en lui meme
extern uint32_t cmd_fifo_idx;//indice du pere
extern uint32_t idx; //tableau des indice client

extern pthread_mutex_t *mutex;
extern GLuint *shm_text_client;
extern int tailletabtext;
extern GLuint *tabtext;



void load_library(void);
//void glXSwapBuffers(Display, GLXDrawable);
//int XSetStandardProperties(Display*,Window,_Xconst char*,_Xconst char*,Pixmap ,char**,int,XSizeHints*);
//void glFrustum ( GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
void fglFrustum();
//void glGenTextures ( GLsizei, GLuint*);
void fglGenTextures();
//void glBindTexture ( GLenum, GLuint);
void fglBindTexture();
