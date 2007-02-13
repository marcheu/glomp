#include "fifo.h"

#define NBCARTE 4
#define SEMKEY  64

unit8_t* cmd_fifo;
uint32_t cmd_fifo_idx;
int client_num;


//variable global pour le semaphore
int            sem_id ;			
struct sembuf  sem_oper_P;	/* Operation P */	
struct sembuf  sem_oper_V;	/* Operation V */

	



void glop_init(){
  /*tout d abord il faut ouvir les fichier de config*/



  //creation d'un pere et de 4 fils 
  //le pere etant le producteur dans le fifo
  //les fils serotn 4 consomateurs



  /*1ere etape, creation du semaphore, enfin de ces structures*/
  union semun 
  {
    int val;
    struct semid_ds *buf;
    ushort *array;
  } s_ctl;
  
  /* Initialisations des structures ad-hoc pour faire P et V */ 
  s_ctl.val          =  1;       /* Valeur d'initialisation du compteur */
  sem_oper_P.sem_num =  0;       
  sem_oper_P.sem_op  = -1;       /* Pour faire P */
  sem_oper_V.sem_num =  0;
  sem_oper_V.sem_op  =  1;       /* Pour faire V */

  /* Creation et initialisation du tableau de  semaphores.
   *   Ici, on utilise un seul semaphore, donc le second parametre est 
   *   positionne a 1
   */
  
  sem_id = semget (SEMKEY, 1, 0777|IPC_CREAT);
  /**/








  client_num=NBCARTE;
  //avant le fork, on creer le shm
  creerFifo();

  /*boucle de creation des process*/

    int i;
    i=0;
    int varfork;
    varfork=1;

    for(i;i<NBCARTE;i++){
      if(varfork==0){
	//on est dans un des processus fils


      }
      else{
	//on est dasn le pere
	varfork=fork();
	if(varfork==-1){perror("fork");return -1;}
      }

    }
    /*process creer*/



   
    if(varfork==0){
      
    }
    else{

    }

  


} 
