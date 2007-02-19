#include "tranfertFenetre.h"

int i;

//creer le segment de memoire partage
char * creershm_fenetre()
{

  int shmid;
  char * shmadr;

   shmid = shmget(IPC_PRIVATE,W*H*4,0666|IPC_CREAT);
   shmadr = shmat(shmid,0,0);
   return shmadr;

}

//lecture de la fenetre pour copie vers le segment de memoire
void lire_fenetre()
{
  GLvoid * pointeur;

    sem_wait(semadrfen_out[numero_proc]);
    if(fenetreactive==0)
      glReadPixels(0,0,W,H,GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre1[numero_proc]);
    else glReadPixels(0,0,W,H,GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre1[numero_proc]);
    sem_post(semadrfen_in[numero_proc]);
    fenetreactive=(fenetreactive+1)%2;

} 


//lecrture depuis le segment de memoire pour ecriture dans le buffer d'affichage
void ecirre_fenetre()
{
   GLvoid * pointeur;
   
   if(fenetreactive==0)  
     for(i=0;i<client_num;i++)
     {
        glRasterPos2i(0,0);
        sem_wait(semadrfen_in[i]);
        glDrawPixels(W,H,GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre1[i]);
        sem_post(semadrfen_out[i]);
     } 
   else for(i=0;i<client_num;i++)
        {
          glRasterPos2i(0,0);
          sem_wait(semadrfen_in[i]);
          glDrawPixels(W,H,GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre2[i]);
          sem_post(semadrfen_out[i]);
        } 

   fenetreactive=(fenetreactive+1)%2;
}  

