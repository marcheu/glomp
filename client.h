#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "dewrapped.h"
#include "init.h"

extern PFNGLXGETPROCADDRESSARBPROC lib_glXGetProcAddressARB;


void (*functable_override[100])(void);

extern void GLOMPunpack();
extern void GLOMPclient_init();


#endif

