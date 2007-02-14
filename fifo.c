
#include "fifo.h"


uint8_t* cmd_fifo;
uint32_t cmd_fifo_idx;



//creer la fifo dasn les variable globales
void creerFifo(){

int numshm;

 cmd_fifo_idx = 0;

 numshm = shmget(IPC_PRIVATE,TAILLEMEM,0666);
 cmd_fifo = shmat(numshm,NULL,(uint8_t)NULL);

}






