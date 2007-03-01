#ifndef _OVERRIDES_H_
#define _OVERRIDES_H_

#include"init.h"

#define OVERRIDE_BASE 8000


void load_library(void);
//void glXSwapBuffers(Display, GLXDrawable);
//int XSetStandardProperties(Display*,Window,_Xconst char*,_Xconst char*,Pixmap ,char**,int,XSizeHints*);
//void glFrustum ( GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
void fglFrustum();
//void glGenTextures ( GLsizei, GLuint*);
void fglGenTextures();
//void glBindTexture ( GLenum, GLuint);
void fglBindTexture();
//void glTexImage2D( GLenum ,GLint ,GLint ,GLsizei,GLsizei,GLint ,GLenum ,GLenum ,const GLvoid * );
void fglTexImage2D();
//void glTexSubImage2D(GLenum ,GLint ,GLint ,GLint ,GLsizei,GLsizei,GLenum ,GLenum ,const GLvoid *)
void fglTexSubImage2D();
#endif

