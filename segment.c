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

// creer le segment et ecrit dans la fifo
int segment_create(char* p,int size)
{
  int i;
  static int segkey=0xDADA;
  uint32_t snd_key;
  uint32_t zero_key=0;
  uint32_t snd_size;


  snd_key=segkey;
  snd_size=size;
  

  // for each client, create a segment, attach it, copy the contents, detach
  if (p)
  for(i=0;i<nbcarte;i++)
    {
      
      char* shm=shmat(shmget(segkey,size,0666|IPC_CREAT),0,0);
      segkey++;
      
      memcpy(shm,p,size);
      
      shmdt(shm);
    }
  
  if (p)  
    fifo_output(&GLOMPcmd_fifo,&snd_key,4);
  else
    fifo_output(&GLOMPcmd_fifo,&zero_key,4);
  fifo_output(&GLOMPcmd_fifo,&snd_size,4);
  printf("[serveur] cree segment key %x size %d\n",snd_key,snd_size);
}

// recupere un segment dans la fifo et l'attache
char* segment_attach()
{
  segmat* s=(segmat*)malloc(sizeof(segmat));
  fifo_input(&GLOMPcmd_fifo,&s->key,4);
  fifo_input(&GLOMPcmd_fifo,&s->size,4);
printf("[%d] fifo input ok key %x size %d\n",client_num,s->key, s->size);

  if (s->key)  
    s->shm=shmat(shmget(s->key+client_num,s->size,0666),0,0);
  else
    s->shm=NULL;
printf("[%d] attach ok %x\n",client_num,s->shm);

  
  s->next=seglist;
  seglist=s;
printf("[%d] attach ok ok\n",client_num);
  return s->shm;
}

// delete all the segments
void segment_delete()
{
  segmat* s=seglist;
  segmat* olds;

  while(s!=NULL)
    {
      if (s->shm){
	// delete the segment
	shmctl(shmget(s->key+client_num,s->size,0666), IPC_RMID, NULL);
	shmdt(s->shm);
      }
      
      olds=s;
      s=s->next;
      free(olds);
    }
  seglist=NULL;
}



int GLOMPsegment_create_retour()
{
  int shmid;
  
  shmadr=shmat(shmget(IPC_PRIVATE,4080,0666|IPC_CREAT),0,0);
  semadr=(sem_t *)shmat(shmget(IPC_PRIVATE,sizeof(sem_t),0666),0,0);
  sem_init(semadr,0,0);
  
  

}



