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

#include "dewrapped.h"
#include "overrides.h"
#include "fifo.h"

typedef struct gl_pixeltype{
  int type;
  int size;
}
gl_pixeltype;

gl_pixeltype pixeltype_table[]=
  {
    {GL_UNSIGNED_BYTE,1},
    {GL_BYTE,1},
    {GL_BITMAP,0},
    {GL_UNSIGNED_SHORT,2},
    {GL_SHORT,2},
    {GL_UNSIGNED_INT,4},
    {GL_INT,4},
    {GL_FLOAT,sizeof(float)},
    {GL_UNSIGNED_BYTE_3_3_2,1},
    {GL_UNSIGNED_BYTE_2_3_3_REV,1},
    {GL_UNSIGNED_SHORT_5_6_5,2},
    {GL_UNSIGNED_SHORT_5_6_5_REV,2},
    {GL_UNSIGNED_SHORT_4_4_4_4,2},
    {GL_UNSIGNED_SHORT_4_4_4_4_REV,2},
    {GL_UNSIGNED_SHORT_5_5_5_1,2},
    {GL_UNSIGNED_SHORT_1_5_5_5_REV,2},
    {GL_UNSIGNED_INT_8_8_8_8,4},
    {GL_UNSIGNED_INT_8_8_8_8_REV,4},
    {GL_UNSIGNED_INT_10_10_10_2,4},
    {GL_UNSIGNED_INT_2_10_10_10_REV,4},
    {GL_HALF_FLOAT_ARB,2},
    {0,0},
  };

int size_pixel(GLenum type)
{
  int i=0;
  int j=0;

  while(pixeltype_table[i].type!=0)
    {
      if (type==pixeltype_table[i].type)
	return pixeltype_table[i].size; 
      i++;
    }
  return 0;
}


/*
  void unpack( )
  {
  int func;
  int flags;
  fifo_input(&cmd_fifo,&func,4);
  fifo_input(&cmd_fifo,&flags,4);
  if(func<OVERRIDE_BASE)
  functable[func]();
  else if(func==OVERRIDE_BASE)
  fglFrustum();
  else if(func==OVERRIDE_BASE+1)
  fglGenTextures();
  else if(func==OVERRIDE_BASE+2)
  fglBindTexture();
  }*/
