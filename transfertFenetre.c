#include "transfertFenetre.h"

int i;

//creer le segment de memoire partage
char * creershm_fenetre()
{

  int shmid;
  char * shmadr;

   shmid = shmget(IPC_PRIVATE,width*heightclient[client_num]*4,0666|IPC_CREAT);
   shmadr = shmat(shmid,0,0);
   return shmadr;

}

//lecture de la fenetre pour copie vers le segment de memoire
void lire_fenetre()
{
  GLvoid * pointeur;

    sem_wait(semadrfen_out[client_num]);
    if(fenetreactive==0)
      glReadPixels(0,0,width,heightclient[client_num],GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre1[client_num]);
    else glReadPixels(0,0,width,heightclient[client_num],GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre2[client_num]);
    sem_post(semadrfen_in[client_num]);
    fenetreactive=(fenetreactive+1)%2;

} 


//lecrture depuis le segment de memoire pour ecriture dans le buffer d'affichage
void ecirre_fenetre()
{
   GLvoid * pointeur;
   
   if(fenetreactive==0)  
     for(i=0;i<nbcarte;i++)
     {
        glRasterPos2i(0,heightclient[i]);
        sem_wait(semadrfen_in[i]);
        glDrawPixels(width,heightclient[i],GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre1[i]);
        sem_post(semadrfen_out[i]);
     } 
   else for(i=0;i<nbcarte;i++)
        {
          glRasterPos2i(0,heightclient[i]);
          sem_wait(semadrfen_in[i]);
          glDrawPixels(width,heightclient[i],GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre2[i]);
          sem_post(semadrfen_out[i]);
        } 

   fenetreactive=(fenetreactive+1)%2;
}  

