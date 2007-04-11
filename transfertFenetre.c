#include "transfertFenetre.h"

/*
 *Les fonction de transfert de fenetre, associer avec la recuperation des donnees GPU via pBuffer
 * vont nous permettre de gerer efficacement les evenements qui surviennent lors de l'appel swapBuffer
 *en effet, nous ne voulons pas que nos GPU affichent leur resultats,
 *mais qu'elle l'envoie au processus maitre qui lui va les afficher 
 */
static int i;
void **shmadr_fenetre1,**shmadr_fenetre2;


/*2 tableau contenant des clef pour les shm (initalise dans createAllFen)*/
static int * tabKey1;
static int * tabKey2;




/*Creation et initilalisation d'un tableau de nbcartes semaphores
semadrfen_in[nbcarte] et semadrfen_out[nbcarte], variables globales a tout le prog
servant a proteger la lecture et l ecriture dans la fifo
focntion appellee dans init */
void GLOMPcreateSem()
{  
  int shmid;

  /*les semaphores doivent etre places dans des segment de mem partage
    pour eter paratges entre process*/
  shmid = shmget(IPC_PRIVATE,sizeof(sem_t)*nbcarte,0666);
  semadrfen_in=(sem_t *)shmat(shmid,0,0);
  shmid = shmget(IPC_PRIVATE,sizeof(sem_t)*nbcarte,0666);
  semadrfen_out=(sem_t *)shmat(shmid,0,0);

  for(i=0;i<nbcarte;i++)
    { 
      sem_init(&semadrfen_in[i],10,0);
      sem_init(&semadrfen_out[i],10,2);
    }
}

/*initialise le tableau des clef de semaphore (sert a pouvoir les eattacher entre process)*/
void GLOMPinitTabKey()
{
  /*pour creer les segment, nous avons besoin de 2 tableaux contenant des clef pour les shm*/
  int j=0;  

  tabKey1=(int *)malloc(sizeof(void *)*nbcarte);
  tabKey2=(int *)malloc(sizeof(void *)*nbcarte);
  
  for(i=0;i<nbcarte;i++){
    tabKey1[i]=9000+j;
    tabKey2[i]=9000+j+1;
    j=j+2; 
  }   
}


//creer le segment de memoire partage qui va contenir toute l'image
//c'est a dire les nbcarte pBuffer
//DEBUGGER 
void createAllFen(){
  

  /*creation de la shm des fenetre des differentes cartes*/
  shmadr_fenetre1=malloc(sizeof(void *)*nbcarte);
  shmadr_fenetre2=malloc(sizeof(void *)*nbcarte);
  
  for(i=0;i<nbcarte;i++)
    {
      shmadr_fenetre1[i]=creershm_fenetre(tabKey1[i]);
      shmadr_fenetre2[i]=creershm_fenetre(tabKey2[i]);
            
    }
  
}

void * creershm_fenetre(int clef)
{
  int shmid;
  void * shmadr;
  shmid = shmget(clef,width*height*4,0666|IPC_CREAT);
  shmadr = shmat(shmid,0,0);
  return shmadr;

}

static void screen_dump(char* filename)
{
  /*
  int i;
  //GLint port[4];
  //lib_glGetIntegerv(GL_VIEWPORT,port);
  FILE* f=fopen(filename,"wb");
  fprintf(f,"P6\n# CREATOR : Volren\n%d %d\n255\n",width,height);
  fflush(f);
  fwrite(shmadr_fenetre1[3],width*4*height,1,f);
  fclose(f);
  */
  int i;
  int width=500, height=400;
  char* mem=(char*)malloc(sizeof(char)*width*3);
  FILE* f=fopen(filename,"wb");
  fprintf(f,"P6\n# CREATOR : Volren\n%d %d\n255\n",width,height);
  for(i=height-1;i>=0;i--)
  	{
	  lib_glReadPixels(0,i,width,1,GL_RGB,GL_UNSIGNED_BYTE,mem);
	  fwrite(mem,width*3,1,f);
  	}
  fclose(f);
  free(mem);
}

//lecture de la fenetre pour copie vers le segment de memoire
//vu qu'il y a une recipoie vers un shm, ceci doit etre proteger (ici avec un semaphore)
void lire_fenetre()
{
  

  sem_wait(&semadrfen_out[client_num]);
  
  int totalload=0;
  int beforeload=0;
  int startload=0;
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
      if (i<client_num)
	beforeload+=client_load[i];
    }
  
  int heightclient=(double)client_load[client_num]/(double)totalload*height;
  startload=height*beforeload/totalload;
  

//  char nom[20];
//  sprintf(nom,"file%d.pnm",client_num);
//  screen_dump(nom);
  
  if(fenetreactive==0){
    lib_glReadPixels(0,startload,width,heightclient,GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre1[client_num]);
  
  }else {
  lib_glReadPixels(0,startload,width,heightclient,GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, shmadr_fenetre2[client_num]);
  }
  sem_post(&semadrfen_in[client_num]);
  fenetreactive=(fenetreactive+1)%2;
//  int aa;
//  sem_getvalue(&semadrfen_in[client_num],&aa);
  
}

//lecrture depuis le segment de memoire pour ecriture dans le buffer d'affichage

void ecrire_fenetre()
{
  int * heightclient;
  heightclient = malloc(sizeof(int)*nbcarte);
  float position=0.f;
  int totalload=0;
 
  lib_glMatrixMode( GL_PROJECTION );
  lib_glPushMatrix();
  lib_glLoadIdentity();
  lib_glOrtho( 0.0, width, 0.0, height, 0.0, 2.0 );
  lib_glMatrixMode( GL_MODELVIEW );
  lib_glPushMatrix();
  lib_glLoadIdentity();
  lib_glViewport( 0, 0, width, height );
 
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
    }
  
  for(i=0;i<nbcarte;i++)
    {
      heightclient[i]=((double)client_load[i]/(double)totalload)*height;
    }
  
  position=heightclient[0];
  if(fenetreactive==0)  
	  for(i=1;i<nbcarte;i++)
	  {
		  lib_glRasterPos2f(0.f,position);
		  sem_wait(&semadrfen_in[i]);
		  lib_glDrawPixels(width,heightclient[i],GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre1[i]);	
//		  screen_dump("./toto.pnm");
		  sem_post(&semadrfen_out[i]);
		  position=position+heightclient[i];
	  } 
  else 
	  for(i=1;i<nbcarte;i++)
	  {
		  lib_glRasterPos2f(0.f,position);
		  sem_wait(&semadrfen_in[i]);
		  lib_glDrawPixels(width,heightclient[i],GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,shmadr_fenetre2[i]);
		  sem_post(&semadrfen_out[i]);
		  position=position+heightclient[i];
	  }

  lib_glMatrixMode( GL_PROJECTION );
  lib_glPopMatrix();
  lib_glMatrixMode( GL_MODELVIEW );
  lib_glPopMatrix();
  // XXX FIXME replace /nbcarte with load balancing
  lib_glViewport(0,0,width,height/nbcarte);

  fenetreactive=(fenetreactive+1)%2;
  free(heightclient);
  //if(DEBUG){printf("fin ECRIREFEN !!\n");}
}  

