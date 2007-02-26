#include "dewrapped.h"

void unpack( )
{
	int func;
	int flags;
	INPUT_FIFO(&func,4);
	INPUT_FIFO(&flags,4);
        if(func<8000)
	 functable[func]();
        else if(func==8000)
              fglFrustum();
        else if(func==8001)
              fglGenTextures();
        else if(func==8002)
              fglBindTexture();
}
