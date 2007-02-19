# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <semaphore.h>


extern int numero_proc;


/* Some global vars */
GLXFBConfig gFBconfig = 0;
Display *gDpy = NULL;
int gScreen = 0;
GLXPbuffer gPBuffer = 0;
