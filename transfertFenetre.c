#include "transfertFenetre.h"

/*
*Les fonction de transfert de fenetre, associer avec la recuperation des donnees GPU via pBuffer
* vont nous permettre de gerer efficacement les evenements qui surviennent lors de l'appel swapBuffer
*en effet, nous ne voulons pas que nos GPU affichent leur resultats,
*mais qu'elle l'envoie au processus maitre qui lui va les afficher 
*/
int i;
void **shmadr_fenetre1,**shmadr_fenetre2;
sem_t **semadrfen_in;
sem_t **semadrfen_out;

int* client_load;//pour calculer la charge de travaille de chaque GPU ...

//creer le segment de memoire partage qui va contenir toute l'image
//c'est a dire les nbcarte pBuffer
//DEBUGGER !
void createAllFen(){
	/*creation de la shm des fenetre des differentes cartes*/
	shmadr_fenetre1=malloc(sizeof(void *)*nbcarte);
	shmadr_fenetre2=malloc(sizeof(void *)*nbcarte);
	semadrfen_in=malloc(sizeof(sem_t *)*nbcarte);
	semadrfen_out=malloc(sizeof(sem_t *)*nbcarte);
	for(i=0;i<nbcarte;i++){
	semadrfen_in[i]=malloc(sizeof(sem_t ));
	semadrfen_out[i]=malloc(sizeof(sem_t ));
	
	  
	}
	


	client_load=malloc(sizeof(int)*nbcarte);

	for(i=0;i<nbcarte;i++)
	{
		shmadr_fenetre1[i]=creershm_fenetre();
		shmadr_fenetre2[i]=creershm_fenetre();
 
 sem_init(semadrfen_in[i],0,0);
 
 sem_init(semadrfen_out[i],0,2);


	}
  
}


void * creershm_fenetre()
{

  int shmid;
  void * shmadr;
   shmid = shmget(IPC_PRIVATE,width*height*4,0666|IPC_CREAT);
   shmadr = shmat(shmid,0,0);
   return shmadr;

}

//lecture de la fenetre pour copie vers le segment de memoire
//vu qu'il y a une recipoie vers un shm, ceci doit etre proteger (ici avec un semaphore)
void lire_fenetre()
{
  if(DEBUG){printf("JE COMMENCE LIRE FENETRE %d!!\n",client_num);}
  sem_wait(semadrfen_in[client_num]);
    int totalload=0;
    for(i=0;i<nbcarte;i++)
	client_load[i]=1;
	    //totalload+=client_load[i];
    int heightclient=(double)client_load[client_num]/(double)totalload*height;

    if(fenetreactive==0)
      glReadPixels(0,0,width,heightclient,GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre1[client_num]);
    else
      glReadPixels(0,0,width,heightclient,GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre2[client_num]);
    sem_post(semadrfen_out[client_num]);
    fenetreactive=(fenetreactive+1)%2;
    
}


//lecrture depuis le segment de memoire pour ecriture dans le buffer d'affichage

void ecrire_fenetre()
{
int * heightclient;
heightclient = malloc(sizeof(int)*nbcarte);
   if(fenetreactive==0)  
     for(i=0;i<nbcarte;i++)
     {
        lib_glRasterPos2i(0,heightclient[i]);

	if(DEBUG){printf("J SUIS AVANT LE SEM WAIT !! %d\n",client_num);}
	sem_wait(semadrfen_in[i]);
	if(DEBUG){printf("J vais faire  UN DRAW !!\n");}
        lib_glDrawPixels(width,heightclient[i],GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre1[i]);
	if(DEBUG){printf("J AI FAIT UN DRAW !!\n");}
	sem_post(semadrfen_out[i]);
     } 
   else for(i=0;i<nbcarte;i++)
        {
	  //if(DEBUG){printf("COUCOU!! %d \n",client_num);}

          lib_glRasterPos2i(0,heightclient[i]);
          sem_wait(semadrfen_in[i]);
          lib_glDrawPixels(width,heightclient[i],GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre2[i]);
          sem_post(semadrfen_in[i]);
        } 

   fenetreactive=(fenetreactive+1)%2;
   free(heightclient);
	if(DEBUG){printf("fin ECRIREFEN !!\n");}
}  

