
#include "segment.h"

segment_attach(int id)
{
}

int segment_create(char* p,int size)
{
	int i;
	int id;
	id=(GLvoid *)shmat( shmget(IPC_SHARED,size,0666|IPC_CREAT) ,0,0);
	
	//
	for(i=0;i<nbcarte;i++)
	{
		
	}
}

segment_delete(int id)
{
}


