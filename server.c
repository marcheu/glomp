#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>


int sizeGLenum(GLenum type)
{
	int i=0;

	while(type_table[i].type!=0)
	{
		if (type==type_table[i].type)
			return type_table[i].size; 
		i++;
	}
	return 0;
}




void server_init()
{
	load_library();
}

