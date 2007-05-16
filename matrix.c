/*
 *   GLOMP - transparent multipipe OpenGL
 *   Copyright (C) 2007 the GLOMP team (see AUTHORS)
 *   
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


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


