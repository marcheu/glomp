#ifndef _OVERRIDES_H_
#define _OVERRIDES_H_
#include <stdlib.h>

#include "init.h"
#include "id.h"
#include "transfertFenetre.h"

#define OVERRIDE_BASE 8000






void load_library(void);

void fglFrustum();
void fglGenTextures();
void fglBindTexture();
void fglXSwapBuffers();
void fglGenLists();
void fglCallList();
void fglCallLists();
void fglBindTextureEXT();
void fglGenTexturesEXT();
void fglGenQueries();
void fglGenBuffers();
void fglBindBuffer();
void fglGenProgramsARB();
void fglBindProgramARB();
void fglGenBuffersARB();
void fglBindBufferARB();
void fglGenQueriesARB();
void fglGenFencesNV();
void fglGenProgramsNV();
void fglBindProgramNV();
void fglGenOcclusionQueriesNV();
void fglGenRenderbuffersEXT();
void fglBindRenderbufferEXT();
void fglGenFramebuffersEXT();
void fglBindFramebufferEXT();
void fglDeleteTextures();
void fglFlush();
void fglFinish();
void fglDeleteQueries();
void fglDeleteBuffers();
void fglDeleteProgramARB();
void fglDeleteBuffersARB();
void fglDeleteQueriesARB();
void fglDeleteTexturesEXT();
void fglDeleteFencesNV();
void fglDeleteProgramsNV();
void fglDeleteOcclusionQueriesNV();
void fglDeleteRenderbuffersEXT();
void fglDeleteFramebuffersEXT();

#endif
