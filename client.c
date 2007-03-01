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
              fglFrustum();
        else if(func==OVERRIDE_BASE+1)
              fglGenTextures();
        else if(func==OVERRIDE_BASE+2)
              fglBindTexture();
}

void client_init()
{
	// create the pbuffer
	if (creerpbuffer(width,height)) {
		printf("Error:couldn't create pbuffer");
		exit(0);
	}
	// allocate texture table
	tabtext=malloc(sizeof(GLuint)*1024);
	creertabfunc();
	idx=0;
}
	
void client_run()
{
	// main loop
	while(1)
		unpack();
}

