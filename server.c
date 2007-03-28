#include "server.h"

static gl_type colorcomponents[]={
	{GL_COLOR_INDEX,1},
	{ GL_RED, 1},
	{ GL_GREEN,1},
	{ GL_BLUE,1},
	{ GL_ALPHA,1},
	{ GL_RGB,3},
	{ GL_BGR,3},
	{ GL_RGBA,4},
	{ GL_BGRA,4},
	{ GL_LUMINANCE,1},
	{GL_LUMINANCE_ALPHA,2},
	{0,0},
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


  return 0;
}

