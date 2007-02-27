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




/*si on ne veux pas utiliser la macro, on peux utiliser les fonctions*/
void outpout_fifo(void * data, int taille){//void * data marche peux etre pas ... void * type trop generique
  j=0;
  for(j;j<nbcarte;j++){
    sem_wait(semap_out[j]);
  }
  if(cmd_fifo_idx+taille>TAILLEMEM){
    memcpy(&cmd_fifo[cmd_fifo_idx],data,taille);
    cmd_fifo_idx=(cmd_fifo_idx+taille)%TAILLEMEM;
  }
  else{
    cmd_fifo_idx=(cmd_fifo_idx+taille)%TAILLEMEM;
    memcpy(&cmd_fifo[cmd_fifo_idx],data,taille);
  }
  j=0;
  for(j;j<nbcarte;j++)
    sem_post(semap_in[j]);
}


void input_fifo(void * buff,int taille ){
  sem_wait(semap_in[client_num]);
  if(cmd_fifo_idx+taille>TAILLEMEM){
    memcpy(buff,&cmd_fifo[idx],taille);
    idx=(idx+taille)%TAILLEMEM;
  }
  else{
    cmd_fifo_idx=(cmd_fifo_idx+taille)%TAILLEMEM;
    memcpy(buff,&cmd_fifo[idx],taille);
  }
  sem_post(semap_out[client_num]);
  
  
}

