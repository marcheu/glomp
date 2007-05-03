#ifndef _GLHEADER_H_
#define _GLHEADER_H_

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#ifdef GL_EXT_timer_query
#else
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#endif


#endif

