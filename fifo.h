
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <pthread.h>

#define TAILLEMEM 128*1024

extern struct{ 
  uint8_t* cmd_fifo;
  uint32_t cmd_fifo_idx;//indice
  uint32_t* idx; //tableau des indice client
}fifo;


extern int client_num;//sert a savoir dasn quel process on est

extern sem_t **semap;




ini j;//comteur

void creerFifo();





#define OUTPUT_FIFO(A,S) \

do{\
     j=0;\
     for(j;j<NBCARTE;j++){\
	  sem_wait(semap_out[j]);}\
 							      
	  /* here check there is enough room or wait or rewind or... */\
	  if(fifo.cmd_fifo_idx+S>TAILLEMEM){\
					 memcpy(&fifo.cmd_fifo[fifo.cmd_fifo_idx],A,S);\
					 fifo.cmd_fifo_idx=(fifo.cmd_fifo_idx+S)%TAILLEMEM;\
	  }
	  else{
	  fifo.cmd_fifo_idx=(fifo.cmd_fifo_idx+S)%TAILLEMEM;\
	  memcpy(&fifo.cmd_fifo[fifo.cmd_fifo_idx],A,S);\
	  }\

																		   
	  j=0;\
	  for(j;j<NBCARTE;j++)\
	  sem_post(semap_in[j]);\

							      
       }while(0)

     
     
#define INPUT_FIFO(A,S) \
	do{\

 sem_wait(semap_in[client_num]);\

 
 /* here check there is enough room or wait or rewind or... */\
  if(fifo.cmd_fifo_idx+S>TAILLEMEM){\
      memcpy(A,&fifo.cmd_fifo[fifo.idx[client_num]],S);\
      fifo.idx[client_num]=(fifo.idx[client_num]+S)%TAILLEMEM;\
										      }
  else{
    fifo.cmd_fifo_idx=(fifo.cmd_fifo_idx+S)%TAILLEMEM;\
					      memcpy(A,&fifo.cmd_fifo[fifo.idx[client_num]],S);\
									   }\
		

    sem_post(semap_out[client_num]);\		
   }while(0)
     
     
     
