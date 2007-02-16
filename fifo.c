
#include "fifo.h"


struct{
  uint8_t* cmd_fifo;
  uint32_t cmd_fifo_idx;//indice
  uint32_t* idx; //tableau des indice client
}fifo;



//creer la fifo dasn les variable globales
void creerFifo(){

int numshm;

 fifo.cmd_fifo_idx = 0;

 numshm = shmget(IPC_PRIVATE,TAILLEMEM,0666);
 fifo.cmd_fifo = shmat(numshm,NULL,(uint8_t)NULL);


}






