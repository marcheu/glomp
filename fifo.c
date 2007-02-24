#include "fifo.h"


/*Cette fonction alloue la memoire pour la fifo
*les variables utile pour la fifo sont globale
*ici nous allons juste allouer la memoire partage
*cette fonction ne doit etre appellle qu'un fois, au debut de init
*/
void creerFifo(){

int numshm;

 cmd_fifo_idx = 0;

 numshm = shmget(IPC_PRIVATE,TAILLEMEM,0666);
 cmd_fifo = shmat(numshm,NULL,(uint8_t)NULL);

}






