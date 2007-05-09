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

