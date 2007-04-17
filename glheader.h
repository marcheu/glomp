#ifndef _GLHEADER_H_
#define _GLHEADER_H_

#include <GL/gl.h>
#include <GL/glext.h>

#ifdef GL_EXT_timer_query
#else
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#endif


#endif

