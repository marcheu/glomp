
#include "client.h"

PFNGLXGETPROCADDRESSARBPROC lib_glXGetProcAddressARB;
static void* lib_handle_libGL = 0;


void unpack( )
{
  int func;
  int flags;
  if(DEBUG){printf("!!!!!!!\t%d\n",client_num);}
  fifo_input(&cmd_fifo,&func,4);
  if(DEBUG){printf("???????\t%d\n",client_num);}
  fifo_input(&cmd_fifo,&flags,4);
  if(DEBUG)printf("CLIENT :fnum:%d\n",func);
  if(func<OVERRIDE_BASE)
    functable[func]();
  else if(func==OVERRIDE_BASE)
    fglXSwapBuffers();
  else if(func==OVERRIDE_BASE+1)
    fglFrustum();
  else if(func==OVERRIDE_BASE+2)
    fglGenTextures();
  else if(func==OVERRIDE_BASE+3)
    fglBindTexture();
  else if(func==OVERRIDE_BASE+4)
    fglGenLists();
  else if(func==OVERRIDE_BASE+5)
    fglCallList();
  else if(func==OVERRIDE_BASE+6)
    fglCallLists();
  else if(func==OVERRIDE_BASE+7)
    fglGenTexturesEXT();
  else if(func==OVERRIDE_BASE+8)
    fglBindTextureEXT();
  else if(func==OVERRIDE_BASE+9)
    fglGenQueries();
  else if(func==OVERRIDE_BASE+10)
    fglGenBuffers();
  else if(func==OVERRIDE_BASE+11)
    fglBindBuffer();
  else if(func==OVERRIDE_BASE+12)
    fglGenProgramsARB();
  else if(func==OVERRIDE_BASE+13)
    fglBindProgramARB();
  else if(func==OVERRIDE_BASE+14)
    fglGenBuffersARB();
  else if(func==OVERRIDE_BASE+15)
    fglBindBufferARB();
  else if(func==OVERRIDE_BASE+16)
    fglGenQueriesARB();
  else if(func==OVERRIDE_BASE+17)
    fglGenFencesNV();
  else if(func==OVERRIDE_BASE+18)
    fglGenProgramsNV();
  else if(func==OVERRIDE_BASE+19)
    fglBindProgramNV();
  else if(func==OVERRIDE_BASE+20)
    fglGenOcclusionQueriesNV();
  else if(func==OVERRIDE_BASE+21)
    fglGenRenderbuffersEXT();
  else if(func==OVERRIDE_BASE+22)
    fglBindRenderbufferEXT();
  else if(func==OVERRIDE_BASE+23)
    fglGenFramebuffersEXT();
  else if(func==OVERRIDE_BASE+24)
    fglBindFramebufferEXT();
  else if(func==OVERRIDE_BASE+25)
    fglFlush();
  else if(func==OVERRIDE_BASE+26)
    fglFinish();
  else if(func==OVERRIDE_BASE+27)
    fglDeleteTextures();
  else if(func==OVERRIDE_BASE+28)
    fglDeleteQueries();
  else if(func==OVERRIDE_BASE+29)
    fglDeleteBuffers();
  else if(func==OVERRIDE_BASE+30)
    fglDeleteProgramsARB();
  else if(func==OVERRIDE_BASE+31)
    fglDeleteBuffersARB();
  else if(func==OVERRIDE_BASE+32)
    fglDeleteQueriesARB();
  else if(func==OVERRIDE_BASE+33)
    fglDeleteTexturesEXT();
  else if(func==OVERRIDE_BASE+34)
    fglDeleteFencesNV();
  else if(func==OVERRIDE_BASE+35)
    fglDeleteProgramsNV();
  else if(func==OVERRIDE_BASE+36)
    fglDeleteOcclusionQueriesNV();
  else if(func==OVERRIDE_BASE+37)
    fglDeleteRenderbuffersEXT();
  else if(func==OVERRIDE_BASE+37)
    fglDeleteFramebuffersEXT();
}


void client_init()
{
  
 lib_handle_libGL = dlopen("/usr/lib/libGL.so", RTLD_LAZY);

  lib_glXGetProcAddressARB = dlsym(lib_handle_libGL, "glXGetProcAddressARB");

  //extern void (*glXGetProcAddressARB(const GLubyte *procName))(void);
  
  initPointers();

  creertabfunc();


  // create the pbuffer
  if (!creerpbuffer(width,height)) {
    printf("Error:couldn't create pbuffer");
    exit(0);
  }

  
  printf("client init OKI %d\n",client_num);
 return; 
}




