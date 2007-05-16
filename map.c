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

#include "overrides.h"

void glMap1f(GLenum target,GLfloat u1,GLfloat u2,GLint stride,GLint order,const GLfloat * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+44;

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&target,4);
	fifo_output(&cmd_fifo,&u1,4);
	fifo_output(&cmd_fifo,&u2,4);
	fifo_output(&cmd_fifo,&stride,4);
	fifo_output(&cmd_fifo,&order,4);
	sizep=order*sizeof(GLfloat);
	fifo_output(&cmd_fifo,points,sizep);
	lib_glMap1f(target,u1,u2,stride,order,points);
}

void GLOMPglMap1f()
{
	GLenum target;
	GLfloat u1;
	GLfloat u2;
	GLint stride;
	GLint order;
	int sizep;
	fifo_input(&cmd_fifo,&target,4);
	fifo_input(&cmd_fifo,&u1,4);
	fifo_input(&cmd_fifo,&u2,4);
	fifo_input(&cmd_fifo,&stride,4);
	fifo_input(&cmd_fifo,&order,4);
	GLfloat  points[order];
	sizep=order*sizeof(GLfloat);
	fifo_input(&cmd_fifo,points,sizep);
	lib_glMap1f(target,u1,u2,stride,order,(GLfloat *)points);
}



void glMap1d(GLenum target,GLdouble u1,GLdouble u2,GLint stride,GLint order,const GLdouble * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+45;

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&target,4);
	fifo_output(&cmd_fifo,&u1,8);
	fifo_output(&cmd_fifo,&u2,8);
	fifo_output(&cmd_fifo,&stride,4);
	fifo_output(&cmd_fifo,&order,4);
	sizep=order*sizeof(GLdouble);
	fifo_output(&cmd_fifo,points,sizep);
	lib_glMap1d(target,u1,u2,stride,order,points);
}

void GLOMPglMap1d()
{
	GLenum target;
	GLdouble u1;
	GLdouble u2;
	GLint stride;
	GLint order;
	int sizep;
	fifo_input(&cmd_fifo,&target,4);
	fifo_input(&cmd_fifo,&u1,8);
	fifo_input(&cmd_fifo,&u2,8);
	fifo_input(&cmd_fifo,&stride,4);
	fifo_input(&cmd_fifo,&order,4);
	GLdouble  points[order];
	sizep=order*sizeof(GLdouble);
	fifo_input(&cmd_fifo,points,sizep);
	lib_glMap1d(target,u1,u2,stride,order,(GLdouble *)points);
}

void glMap2f(GLenum target,GLfloat u1,GLfloat u2,GLint ustride,GLint uorder,GLfloat v1,GLfloat v2,GLint vstride,GLint vorder,const GLfloat * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+46;

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&target,4);
	fifo_output(&cmd_fifo,&u1,4);
	fifo_output(&cmd_fifo,&u2,4);
	fifo_output(&cmd_fifo,&ustride,4);
	fifo_output(&cmd_fifo,&uorder,4);
	fifo_output(&cmd_fifo,&v1,4);
	fifo_output(&cmd_fifo,&v2,4);
	fifo_output(&cmd_fifo,&vstride,4);
	fifo_output(&cmd_fifo,&vorder,4);
	sizep=uorder*vorder*sizeof(GLfloat);
	fifo_output(&cmd_fifo,points,sizep);
	lib_glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);
}

void GLOMPglMap2f()
{
	GLenum target;
	GLfloat u1;
	GLfloat u2;
	GLint ustride;
	GLint uorder;
	GLfloat v1;
	GLfloat v2;
	GLint vstride;
	GLint vorder;
	int sizep;
	fifo_input(&cmd_fifo,&target,4);
	fifo_input(&cmd_fifo,&u1,4);
	fifo_input(&cmd_fifo,&u2,4);
	fifo_input(&cmd_fifo,&ustride,4);
	fifo_input(&cmd_fifo,&uorder,4);
	fifo_input(&cmd_fifo,&v1,4);
	fifo_input(&cmd_fifo,&v2,4);
	fifo_input(&cmd_fifo,&vstride,4);
	fifo_input(&cmd_fifo,&vorder,4);
	GLfloat  points[uorder*vorder];
	sizep=uorder*vorder*sizeof(GLfloat);
	fifo_input(&cmd_fifo,points,sizep);
	lib_glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,(GLfloat *)points);
}


void glMap2d(GLenum target,GLdouble u1,GLdouble u2,GLint ustride,GLint uorder,GLdouble v1,GLdouble v2,GLint vstride,GLint vorder,const GLdouble * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+47;

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&target,4);
	fifo_output(&cmd_fifo,&u1,8);
	fifo_output(&cmd_fifo,&u2,8);
	fifo_output(&cmd_fifo,&ustride,4);
	fifo_output(&cmd_fifo,&uorder,4);
	fifo_output(&cmd_fifo,&v1,8);
	fifo_output(&cmd_fifo,&v2,8);
	fifo_output(&cmd_fifo,&vstride,4);
	fifo_output(&cmd_fifo,&vorder,4);
	sizep=uorder*vorder*sizeof(GLdouble);
	fifo_output(&cmd_fifo,points,sizep);
	lib_glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);
}

void GLOMPglMap2d()
{
	GLenum target;
	GLdouble u1;
	GLdouble u2;
	GLint ustride;
	GLint uorder;
	GLdouble v1;
	GLdouble v2;
	GLint vstride;
	GLint vorder;
	int sizep;
	fifo_input(&cmd_fifo,&target,4);
	fifo_input(&cmd_fifo,&u1,8);
	fifo_input(&cmd_fifo,&u2,8);
	fifo_input(&cmd_fifo,&ustride,4);
	fifo_input(&cmd_fifo,&uorder,4);
	fifo_input(&cmd_fifo,&v1,8);
	fifo_input(&cmd_fifo,&v2,8);
	fifo_input(&cmd_fifo,&vstride,4);
	fifo_input(&cmd_fifo,&vorder,4);
	sizep=uorder*vorder*sizeof(GLdouble);
	GLdouble  points[uorder*vorder];
	fifo_input(&cmd_fifo,points,sizep);
	lib_glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,(GLdouble*)points);
}


