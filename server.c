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

#include "server.h"

static gl_type colorcomponents[]={
	{ GL_COLOR_INDEX,8},
	{ GL_RED, 8},
	{ GL_GREEN,8},
	{ GL_BLUE,8},
	{ GL_ALPHA,8},
	{ GL_RGB,24},
	{ GL_BGR,24},
	{ GL_RGBA,32},
	{ GL_BGRA,32},
	{ GL_LUMINANCE,8},
	{ GL_LUMINANCE_ALPHA,16},
	{ GL_BITMAP,1},
	{ 0x84F9 /*GL_DEPTH_STENCIL_NV*/,32},
	{ 0x84FA /*GL_UNSIGNED_INT_24_8_NV*/,32},
	{ 0,0},
};

int sizeGLenum(GLenum type)
{
	int i=0;

	while(colorcomponents[i].name!=0)
	{
		if (type==colorcomponents[i].name){
			return colorcomponents[i].size; 
		}
		i++;
	}

	i=0;

	while(type_table[i].name!=0)
	{
		if (type==type_table[i].name){
			return type_table[i].size; 
		}
		i++;
	}

	printf("Attention sizeGLenum=0 %x!!\n",type);
	return 0;
}

