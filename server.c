#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>


typedef struct gl_pixeltype{
	int type;
	int size;
}
gl_pixeltype;


static gl_pixeltype pixeltype_table[]=
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




void server_init()
{
	load_library();
}

