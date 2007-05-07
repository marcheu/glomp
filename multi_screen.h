#ifndef _MULTI_SCREEN_H_
#define _MULTI_SCREEN_H_

#include"init.h"
#include"overrides.h"
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>



extern void GLOMP_multi_screen_init();
extern void GLOMP_multi_screen_init_window(int* glxattribs);
extern void GLOMP_multi_screen_swap(Display *dpy, GLXDrawable drawable);



#endif
