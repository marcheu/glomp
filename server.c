#include "server.h"


int sizeGLenum(GLenum type)
{
	int i=0;

	while(type_table[i].name!=0)
	{
		if (type==type_table[i].name)
			return type_table[i].size; 
		i++;
	}
	return 0;
}




void server_init()
{
	load_library();
	//	while(1){
	//;
	//}
	
}

