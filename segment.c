#include "segment.h"

//char* shmadr;
//sem_t * semadr

typedef struct segmat
{
	int key;
	int size;
	char* shm;
	struct segmat* next;
}segmat;

segmat* seglist=NULL;

// creer le segment et ecrit dans la fifo
int segment_create(char* p,int size)
{
	int i;
	static int segkey=0xDADA;
	uint32_t snd_key;
	uint32_t snd_size;

	snd_key=segkey;
	snd_size=size;

	// for each client, create a segment, attach it, copy the contents, detach
	for(i=0;i<nbcarte;i++)
	{
		char* shm=shmat(shmget(segkey,size,0666|IPC_CREAT),0,0);
		segkey++;
		memcpy(shm,p,size);
		shmdt(shm);
	}
	fifo_output(&cmd_fifo,&snd_key,4);
	fifo_output(&cmd_fifo,&snd_size,4);
}

// recupere un segment dans la fifo et l'attache
char* segment_attach()
{
	segmat* s=(segmat*)malloc(sizeof(segmat));
	fifo_input(&cmd_fifo,&s->key,4);
	fifo_input(&cmd_fifo,&s->size,4);

	s->shm=shmat(shmget(s->key+client_num,s->size,0666),0,0);
	s->next=seglist;
	seglist=s;
	return s->shm;
}

// delete all the segments
void segment_delete()
{
	segmat* s=seglist;
	segmat* olds;

	while(s!=NULL)
	{
		// delete the segment
		shmctl(shmget(s->key+client_num,s->size,0666), IPC_RMID, NULL);
		shmdt(s->shm);
		olds=s;
		s=s->next;
		free(olds);
	}
	seglist=NULL;
}



int segment_create_retour()
{

	shmadr=shmat(shmget(IPC_PRIVATE,4080,0666|IPC_CREAT),0,0);
	semadr=malloc(sizeof(sem_t));
	
	sem_init(semadr,0,0);

}





