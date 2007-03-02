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

