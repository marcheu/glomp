#ifndef _OVERRIDES_H_
#define _OVERRIDES_H_
#include <stdlib.h>

#include "init.h"
#include "id.h"
#include "single_screen.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

#define OVERRIDE_BASE 8000


void load_library(void);

void GLOMPglFrustum();
void GLOMPglGenTextures();
void GLOMPglBindTexture();
void GLOMPglXSwapBuffers();
void GLOMPglGenLists();
void GLOMPglCallList();
void GLOMPglGetString();
void GLOMPglCallLists();
void GLOMPglBindTextureEXT();
void GLOMPglGenTexturesEXT();
void GLOMPglGenQueries();
void GLOMPglGenBuffers();
void GLOMPglBindBuffer();
void GLOMPglGenProgramsARB();
void GLOMPglBindProgramARB();
void GLOMPglGenBuffersARB();
void GLOMPglBindBufferARB();
void GLOMPglGenQueriesARB();
void GLOMPglGenFencesNV();
void GLOMPglGenProgramsNV();
void GLOMPglBindProgramNV();
void GLOMPglGenOcclusionQueriesNV();
void GLOMPglGenRenderbuffersEXT();
void GLOMPglBindRenderbufferEXT();
void GLOMPglGenFramebuffersEXT();
void GLOMPglBindFramebufferEXT();
void GLOMPglDeleteTextures();
void GLOMPglFlush();
void GLOMPglFinish();
void GLOMPglDeleteQueries();
void GLOMPglDeleteBuffers();
void GLOMPglDeleteProgramARB();
void GLOMPglDeleteProgramsARB();
void GLOMPglDeleteBuffersARB();
void GLOMPglDeleteQueriesARB();
void GLOMPglDeleteTexturesEXT();
void GLOMPglDeleteFencesNV();
void GLOMPglDeleteProgramsNV();
void GLOMPglDeleteOcclusionQueriesNV();
void GLOMPglDeleteRenderbuffersEXT();
void GLOMPglDeleteFramebuffersEXT();
void GLOMPXCreateWindow();
void GLOMPglViewport();
void GLOMPglOrtho();
void GLOMPglNewList();
void GLOMPglMap1f();
void GLOMPglMap1d();
void GLOMPglMap2f();
void GLOMPglMap2d();
void GLOMPdraw_array();
void GLOMPquit();
#endif
