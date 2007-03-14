#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "dewrapped.h"
#include "overrides.h"
#include "fifo.h"
#include "pbuffer.h"
#include "init.h"

extern PFNGLXGETPROCADDRESSARBPROC lib_glXGetProcAddressARB;


extern void GLOMPunpack();
extern void client_init();
//extern void GLOMPclient_run();


#endif

