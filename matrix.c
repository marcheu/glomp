
#include "lib_funcs.h"
#include "overrides.h"
#include "tile_screen.h"


void glLoadMatrixf(const GLfloat * m)
{
	int sizep;
	int fnum=OVERRIDE_BASE+50;
	GLint mode;

	if(DEBUG){printf("serveur glLoadMatrixf\n");}

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	sizep=4*16;
	fifo_output(&cmd_fifo,m,sizep);

	lib_glGetIntegerv(GL_MATRIX_MODE,&mode);
	if (mode==GL_PROJECTION)
		tile_screen_glLoadMatrixf(client_num,m);
	else
		lib_glLoadMatrixf(m);
	return ;
}

void GLOMPglLoadMatrixf()
{
	int sizep;
	GLfloat m[16];
	GLint mode;

	sizep=4*16;
	fifo_input(&cmd_fifo,m,sizep);

	lib_glGetIntegerv(GL_MATRIX_MODE,&mode);
	if (mode==GL_PROJECTION)
		tile_screen_glLoadMatrixf(client_num,m);
	else
		lib_glLoadMatrixf(m);
}

void glLoadMatrixd(const GLdouble * m)
{
	int sizep;
	int fnum=OVERRIDE_BASE+51;
	GLint mode;

	if(DEBUG){printf("serveur glLoadMatrixd\n");}

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	sizep=8*16;
	fifo_output(&cmd_fifo,m,sizep);

	lib_glGetIntegerv(GL_MATRIX_MODE,&mode);
	if (mode==GL_PROJECTION)
		tile_screen_glLoadMatrixd(client_num,m);
	else
		lib_glLoadMatrixd(m);
	return ;
}

void GLOMPglLoadMatrixd()
{
	int sizep;
	GLdouble m[16];
	GLint mode;

	sizep=8*16;
	fifo_input(&cmd_fifo,m,sizep);

	lib_glGetIntegerv(GL_MATRIX_MODE,&mode);
	if (mode==GL_PROJECTION)
		tile_screen_glLoadMatrixd(client_num,m);
	else
		lib_glLoadMatrixd(m);
}


