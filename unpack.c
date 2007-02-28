#include "dewrapped.h"
#include "overrides.h"
#include "fifo.h"

void unpack( )
{
	int func;
	int flags;
	fifo_input(cmd_fifo,&func,4);
	fifo_input(cmd_fifo,&flags,4);
        if(func<OVERRIDE_BASE)
	   functable[func]();
        else if(func==OVERRIDE_BASE)
              fglFrustum();
        else if(func==OVERRIDE_BASE+1)
              fglGenTextures();
        else if(func==OVERRIDE_BASE+2)
              fglBindTexture();
}
