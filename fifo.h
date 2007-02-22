#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>


#define TAILLEMEM 128*1024


extern  uint8_t* cmd_fifo;
extern  uint32_t cmd_fifo_idx;//indice
extern  uint32_t idx; //tableau des indice client

extern int client_num;//sert a savoir dasn quel process on est
extern sem_t **semap;

int j;//comteur

void creerFifo();


#define OUTPUT_FIFO(A,S)\
do{\
     j=0;\
     for(j;j<nbcarte;j++){\
          sem_wait(semap_out[j]);\
     }\
	  if(cmd_fifo_idx+S>TAILLEMEM){\
					 memcpy(&cmd_fifo[cmd_fifo_idx],A,S);\
					 cmd_fifo_idx=(cmd_fifo_idx+S)%TAILLEMEM;\
	  }\
	  else{\
	  cmd_fifo_idx=(cmd_fifo_idx+S)%TAILLEMEM;\
	  memcpy(&cmd_fifo[cmd_fifo_idx],A,S);\
	  }\
	  j=0;\
	  for(j;j<nbcarte;j++)\
	  sem_post(semap_in[j]);\
       }while(0)

     
     
#define INPUT_FIFO(A,S) \
	do{\
 sem_wait(semap_in[client_num]);\
 /* here check there is enough room or wait or rewind or... */\
  if(cmd_fifo_idx+S>TAILLEMEM){\
      memcpy(A,&cmd_fifo[idx],S);\
      idx=(idx+S)%TAILLEMEM;\
  }\
  else{\
    cmd_fifo_idx=(cmd_fifo_idx+S)%TAILLEMEM;\
					      memcpy(A,&cmd_fifo[idx],S);\
									   }\
    sem_post(semap_out[client_num]);\
   }while(0)
     
     
