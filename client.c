#include "dewrapped.h"
#include "overrides.h"
#include "fifo.h"

static void unpack( )
{
	int func;
	int flags;
	fifo_input(&cmd_fifo,&func,4);
	fifo_input(&cmd_fifo,&flags,4);
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


}

void client_init()
{
	// create the pbuffer
	if (creerpbuffer(width,height)) {
		printf("Error:couldn't create pbuffer");
		exit(0);
	}
	creertabfunc();
}
	
void client_run()
{
	// main loop
	while(1)
		unpack();
}

