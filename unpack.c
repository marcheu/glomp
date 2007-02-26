#include "dewrapped.h"
#include "overrides.h"

void unpack( )
{
	int func;
	int flags;
	INPUT_FIFO(&func,4);
	INPUT_FIFO(&flags,4);
        if(func<OVERRIDE_BASE)
	   functable[func]();
        else if(func==OVERRIDE_BASE)
              fglFrustum();
        else if(func==OVERRIDE_BASE+1)
              fglGenTextures();
        else if(func==OVERRIDE_BASE+2)
              fglBindTexture();
}
