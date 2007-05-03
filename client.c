#include "client.h"
#include "lib_funcs.h"


extern void initPointers();


void GLOMPunpack()
{
	int func;
	fifo_input(&GLOMPcmd_fifo,&func,4);
	if(DEBUG){printf("CLIENT %d :fnum:%d\n",client_num,func);fflush(stdout);}
	if(func<OVERRIDE_BASE)
		functable[func]();
	else if(func==OVERRIDE_BASE)
		GLOMPglXSwapBuffers();
	else if(func==OVERRIDE_BASE+1)
		GLOMPglFrustum();
	else if(func==OVERRIDE_BASE+2)
		GLOMPglGenTextures();
	else if(func==OVERRIDE_BASE+3)
		GLOMPglBindTexture();
	else if(func==OVERRIDE_BASE+4)
		GLOMPglGenLists();
	else if(func==OVERRIDE_BASE+5)
		GLOMPglCallList();
	else if(func==OVERRIDE_BASE+6)
		GLOMPglCallLists();
	else if(func==OVERRIDE_BASE+7)
		GLOMPglGenTexturesEXT();
	else if(func==OVERRIDE_BASE+8)
		GLOMPglBindTextureEXT();
	else if(func==OVERRIDE_BASE+9)
		GLOMPglGenQueries();
	else if(func==OVERRIDE_BASE+10)
		GLOMPglGenBuffers();
	else if(func==OVERRIDE_BASE+11)
		GLOMPglBindBuffer();
	else if(func==OVERRIDE_BASE+12)
		GLOMPglGenProgramsARB();
	else if(func==OVERRIDE_BASE+13)
		GLOMPglBindProgramARB();
	else if(func==OVERRIDE_BASE+14)
		GLOMPglGenBuffersARB();
	else if(func==OVERRIDE_BASE+15)
		GLOMPglBindBufferARB();
	else if(func==OVERRIDE_BASE+16)
		GLOMPglGenQueriesARB();
	else if(func==OVERRIDE_BASE+17)
		GLOMPglGenFencesNV();
	else if(func==OVERRIDE_BASE+18)
		GLOMPglGenProgramsNV();
	else if(func==OVERRIDE_BASE+19)
		GLOMPglBindProgramNV();
	else if(func==OVERRIDE_BASE+20)
		GLOMPglGenOcclusionQueriesNV();
	else if(func==OVERRIDE_BASE+21)
		GLOMPglGenRenderbuffersEXT();
	else if(func==OVERRIDE_BASE+22)
		GLOMPglBindRenderbufferEXT();
	else if(func==OVERRIDE_BASE+23)
		GLOMPglGenFramebuffersEXT();
	else if(func==OVERRIDE_BASE+24)
		GLOMPglBindFramebufferEXT();
	else if(func==OVERRIDE_BASE+25)
		GLOMPglFlush();
	else if(func==OVERRIDE_BASE+26)
		GLOMPglFinish();
	else if(func==OVERRIDE_BASE+27)
		GLOMPglDeleteTextures();
	else if(func==OVERRIDE_BASE+28)
		GLOMPglDeleteQueries();
	else if(func==OVERRIDE_BASE+29)
		GLOMPglDeleteBuffers();
	else if(func==OVERRIDE_BASE+30)
		GLOMPglDeleteProgramsARB();
	else if(func==OVERRIDE_BASE+31)
		GLOMPglDeleteBuffersARB();
	else if(func==OVERRIDE_BASE+32)
		GLOMPglDeleteQueriesARB();
	else if(func==OVERRIDE_BASE+33)
		GLOMPglDeleteTexturesEXT();
	else if(func==OVERRIDE_BASE+34)
		GLOMPglDeleteFencesNV();
	else if(func==OVERRIDE_BASE+35)
		GLOMPglDeleteProgramsNV();
	else if(func==OVERRIDE_BASE+36)
		GLOMPglDeleteOcclusionQueriesNV();
	else if(func==OVERRIDE_BASE+37)
		GLOMPglDeleteRenderbuffersEXT();
	else if(func==OVERRIDE_BASE+38)
		GLOMPglDeleteFramebuffersEXT();
	else if(func==OVERRIDE_BASE+39)
		GLOMPXCreateWindow();
	else if(func==OVERRIDE_BASE+40)
		GLOMPglViewport();
	else if(func==OVERRIDE_BASE+41)
		GLOMPglOrtho();
	else if(func==OVERRIDE_BASE+42)
		GLOMPglNewList();
	else if(func==OVERRIDE_BASE+43)
		GLOMPglGetString();
	else if(func==OVERRIDE_BASE+44)
		GLOMPglMap1f();
	else if(func==OVERRIDE_BASE+45)
		GLOMPglMap1d();
	else if(func==OVERRIDE_BASE+46)
		GLOMPglMap2f();
	else if(func==OVERRIDE_BASE+47)
		GLOMPglMap2d();
}


/*initialise un client*/
void GLOMPclient_init()
{  
	initPointers();/*recupere toutes les add des fonction glX (cf dewarped)*/
	creertabfunc();/*creer le tableau des fonction deriver GLOMP...*/ 
	lib_funcs_init();/* initialiser tous les lib_* */
}




