#include "server.h"


int sizeGLenum(GLenum type)
{
  int i=0;
  printf("DANS SIZEGLENUM!!!!!! \t%d\t%d\n",client_num,type);
	

  while(type_table[i].name!=0)
    {
      //printf("type1:%d type2:%d\n",type,type_table[i].name);
      if (type==type_table[i].name)
	return type_table[i].size; 
      i++;
    }
  return 0;
}




void server_init()
{
  load_library();
  
  /*	while(1){
	;
	}*/
	
}

