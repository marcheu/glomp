#include "fifo.h"


//creer la fifo dans les variable globales
void creerFifo(){

int numshm;

 cmd_fifo_idx = 0;

 numshm = shmget(IPC_PRIVATE,TAILLEMEM,0666);
 cmd_fifo = shmat(numshm,NULL,(uint8_t)NULL);

}






