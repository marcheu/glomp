#include "client.h"
#include "lib_funcs.h"


typedef void (*func_dispatch_entry) ();

static func_dispatch_entry func_dispatch[]=
{
	GLOMPglXSwapBuffers, 		// OVERRIDE_BASE + 0
	GLOMPglFrustum,			// OVERRIDE_BASE + 1
	GLOMPglGenTextures,		// OVERRIDE_BASE + 2
	GLOMPglBindTexture,		// OVERRIDE_BASE + 3
	GLOMPglGenLists,		// OVERRIDE_BASE + 4
	GLOMPglCallList,		// OVERRIDE_BASE + 5
	GLOMPglCallLists,		// OVERRIDE_BASE + 6
	GLOMPglGenTexturesEXT,		// OVERRIDE_BASE + 7
	GLOMPglBindTextureEXT,		// OVERRIDE_BASE + 8
	GLOMPglGenQueries,		// OVERRIDE_BASE + 9
	GLOMPglGenBuffers,		// OVERRIDE_BASE + 10
	GLOMPglBindBuffer,		// OVERRIDE_BASE + 11
	GLOMPglGenProgramsARB,		// OVERRIDE_BASE + 12
	GLOMPglBindProgramARB,		// OVERRIDE_BASE + 13
	GLOMPglGenBuffersARB,		// OVERRIDE_BASE + 14
	GLOMPglBindBufferARB,		// OVERRIDE_BASE + 15
	GLOMPglGenQueriesARB,		// OVERRIDE_BASE + 16
	GLOMPglGenFencesNV,		// OVERRIDE_BASE + 17
	GLOMPglGenProgramsNV,		// OVERRIDE_BASE + 18
	GLOMPglBindProgramNV,		// OVERRIDE_BASE + 19
	GLOMPglGenOcclusionQueriesNV,	// OVERRIDE_BASE + 20
	GLOMPglGenRenderbuffersEXT,	// OVERRIDE_BASE + 21
	GLOMPglBindRenderbufferEXT,	// OVERRIDE_BASE + 22
	GLOMPglGenFramebuffersEXT,	// OVERRIDE_BASE + 23
	GLOMPglBindFramebufferEXT,	// OVERRIDE_BASE + 24
	GLOMPglFlush,			// OVERRIDE_BASE + 25
	GLOMPglFinish,			// OVERRIDE_BASE + 26
	GLOMPglDeleteTextures,		// OVERRIDE_BASE + 27
	GLOMPglDeleteQueries,		// OVERRIDE_BASE + 28
	GLOMPglDeleteBuffers,		// OVERRIDE_BASE + 29
	GLOMPglDeleteProgramsARB,	// OVERRIDE_BASE + 30
	GLOMPglDeleteBuffersARB,	// OVERRIDE_BASE + 31
	GLOMPglDeleteQueriesARB,	// OVERRIDE_BASE + 32
	GLOMPglDeleteTexturesEXT,	// OVERRIDE_BASE + 33
	GLOMPglDeleteFencesNV,		// OVERRIDE_BASE + 34
	GLOMPglDeleteProgramsNV,	// OVERRIDE_BASE + 35
	GLOMPglDeleteOcclusionQueriesNV,// OVERRIDE_BASE + 36
	GLOMPglDeleteRenderbuffersEXT,	// OVERRIDE_BASE + 37
	GLOMPglDeleteFramebuffersEXT,	// OVERRIDE_BASE + 38
	GLOMPXCreateWindow,		// OVERRIDE_BASE + 39
	GLOMPglViewport,		// OVERRIDE_BASE + 40
	GLOMPglOrtho,			// OVERRIDE_BASE + 41
	GLOMPglNewList,			// OVERRIDE_BASE + 42
	GLOMPglGetString,		// OVERRIDE_BASE + 43
	GLOMPglMap1f,			// OVERRIDE_BASE + 44
	GLOMPglMap1d,			// OVERRIDE_BASE + 45
	GLOMPglMap2f,			// OVERRIDE_BASE + 46
	GLOMPglMap2d,			// OVERRIDE_BASE + 47
	GLOMPdraw_array,		// OVERRIDE_BASE + 48
	GLOMPquit,			// OVERRIDE_BASE + 49
	GLOMPglLoadMatrixf,		// OVERRIDE_BASE + 50
	GLOMPglLoadMatrixd,		// OVERRIDE_BASE + 51
};

void client_unpack()
{
	int func;
	fifo_input(&cmd_fifo,&func,4);
	if(DEBUG){printf("CLIENT %d :fnum:%d\n",client_num,func);fflush(stdout);}
	if(func<OVERRIDE_BASE)
		functable[func]();
	else if(func-OVERRIDE_BASE<sizeof(func_dispatch)/sizeof(func_dispatch[0]))
		func_dispatch[func-OVERRIDE_BASE]();
	else
		printf("CLIENT: unpack unknown function %d\n",func);
}




