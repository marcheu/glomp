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

/* 
 * Creates a segment for fire & forget usage
 */

#include "segment.h"


sem_t * semadr;

typedef struct segmat
{
	int key;
	int size;
	char* shm;
	struct segmat* next;
}segmat;

segmat* seglist=NULL;

#define FAST_PATH_SIZE 1000

// creer le segment et ecrit dans la fifo
void segment_create(char* p,int size)
{
	int i;
	static int segkey=0xDADA;
	uint32_t snd_key;
	uint32_t zero_key=0;
	uint32_t zero_size=0;
	uint32_t snd_size;


	snd_key=segkey;
	snd_size=size;


	if (p==NULL)
	{
		// easy case : NULL pointer
		fifo_output(&cmd_fifo,&zero_key,4);
		fifo_output(&cmd_fifo,&zero_size,4);
	}
	else if (size<FAST_PATH_SIZE)
	{
		// fast path : copy the data to the fifo
		fifo_output(&cmd_fifo,&zero_key,4);
		fifo_output(&cmd_fifo,&snd_size,4);
		fifo_output(&cmd_fifo,p,size);
	}
	else
	{
		// default path : create an shm
		// for each client, create a segment, attach it, copy the contents, detach
		for(i=1;i<nbcarte;i++)
		{
			char* shm=shmat(shmget(segkey,size,0666|IPC_CREAT),0,0);
			segkey++;

			memcpy(shm,p,size);

			shmdt(shm);
		}

		fifo_output(&cmd_fifo,&snd_key,4);
		fifo_output(&cmd_fifo,&snd_size,4);
	}
	//printf("[serveur] cree segment key %x size %d\n",p?snd_key:zero_key,snd_size);
	//fflush(stdout);
}

// recupere un segment dans la fifo et l'attache
char* segment_attach()
{
	uint32_t key,size;
	fifo_input(&cmd_fifo,&key,4);
	fifo_input(&cmd_fifo,&size,4);
	//printf("[%d] fifo input ok key %x size %d\n",client_num,key,size);

	if (size==0)
	{
		// easy : no data
		return NULL;
	}
	else if (size<FAST_PATH_SIZE)
	{
		// get data from the fifo
		segmat* s=(segmat*)malloc(sizeof(segmat));
		s->key=0;
		s->shm=(char*)malloc(size);
		fifo_input(&cmd_fifo,s->shm,size);
		return s->shm;
	}
	else
	{
		// attach the shm
		segmat* s=(segmat*)malloc(sizeof(segmat));
		s->key=key;
		s->size=size;
		s->shm=shmat(shmget(s->key+client_num-1,s->size,0666),0,0);
		s->next=seglist;
		seglist=s;
		return s->shm;
	}
}

// delete all the segments
void segment_delete()
{
	segmat* s=seglist;
	segmat* olds;

	while(s!=NULL)
	{
		if (s->shm!=NULL){
			if (s->key!=0)
			{
				// delete the segment
				shmctl(shmget(s->key+client_num-1,s->size,0666), IPC_RMID, NULL);
				shmdt(s->shm);
			}
			else
			{
				free(s->shm);
			}

			olds=s;
			s=s->next;
			free(olds);
		}
		seglist=NULL;
	}
}



void segment_create_static()
{
	int i;

	shmadr=shmat(shmget(IPC_PRIVATE,4096*nbcarte,0666|IPC_CREAT),0,0);
	semadr=(sem_t *)shmat(shmget(IPC_PRIVATE,sizeof(sem_t)*nbcarte,0666),0,0);
	for(i=0;i<nbcarte;i++)
		sem_init(semadr+i,0,0);
}



