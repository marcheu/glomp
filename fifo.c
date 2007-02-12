
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

//l'ecritu dans la fifo se fait a l'aide d ela macrot OUTPUT_FIFO

void lectureFifo(void * buff,int idx){


  memcpy(buff,&cmd_fifo[idx],sizeof(cmd_fifo[idx]));
  idx=(cmd_fifo_idx+1)%TAILLEMEM;


}




