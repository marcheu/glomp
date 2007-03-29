#ifndef _OVERRIDES_H_
#define _OVERRIDES_H_
#include <stdlib.h>

#include "init.h"
#include "id.h"
#include "transfertFenetre.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#define OVERRIDE_BASE 8000


void load_library(void);
extern void (*functable_override[])(void);



#endif
