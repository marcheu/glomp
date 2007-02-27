#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include "init.h"



int j;//compteur pour la macro

void creerFifo();
void outpout_fifo(void *, int);
void input_fifo(void *,int);

/*
 *Voici de macro permettant de lire et d'ecrire dans la fifo, en gerant les protections
 *Par contre cette macro est tres sensible a la casse, et chaque ligne dit finir par un \ 
 *et ne pas comprendre d'espace suplementraire apres
 *les commentaire doivent aussi etre suivi de \
*/

/*cette premiere macro, n'est utilise que par le pere et ecrit dans la shm (la fifo)
 *en ayant prealablemant bloquer tous les semaphores, et donc l'acces a la fifo au fils
*/




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

     
/*la deuxieme macro fait l'inverse et bien sur du coup ne bloque pas tous les semaphores*/
     
#define INPUT_FIFO(A,S) \
	do{\
 sem_wait(semap_in[client_num]);\
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
     
     
