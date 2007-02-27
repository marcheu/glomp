#include "init.h"




//la structure qui contient le shm servant de fifo
uint8_t* cmd_fifo;//le shm en lui meme
uint32_t cmd_fifo_idx;//indice du pere
uint32_t idx; //tableau des indice client


int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarre
int nbcarte;//nombre de GPU dispo

int width,height;//taille de la zone
int * heightclient;

void **shmadr_fenetre1,**shmadr_fenetre2;
sem_t **semadrfen_in,**semadrfen_out;
int fenetreactive=0;

GLuint *tabtext;//le tableau des etxtures
GLuint *shm_text_client;//un shm pour le tableau des textures
int tailletabtext=0;

sem_t **semap_in, **semap_out;//les semaphore pour proteger la fifo
GLuint conteur_textures=0;
GLuint * tabtextures;
	
pthread_mutex_t *mutex2D;//le mutex pour proteger les texture 2D
void *shm2D;

void glop_init(){
 
  int i;
  int varfork;
  /*tout d abord il faut ouvir les fichier de config*/
  
  /*trouver les display, et donc le nomnbre de carte graphique*/
  Display* dpy = XOpenDisplay("");
  nbcarte=ScreenCount(dpy);

  // potentially override the number of GPUs
  char* force=getenv("FORCE_GPU");
  if (force)
    {
      printf("forcing %d GPU\n",atoi(force));
      nbcarte=atoi(force);
    }


  //initailisation de tout les tableau      
  heightclient=malloc(sizeof(int)*nbcarte);

  shmadr_fenetre1=malloc(sizeof(void *)*nbcarte);
  shmadr_fenetre2=malloc(sizeof(void *)*nbcarte);
  semadrfen_in=malloc(sizeof(sem_t *)*nbcarte);
  semadrfen_out=malloc(sizeof(sem_t *)*nbcarte);

  
  //initialisation des semaphore
  for(i=0;i<nbcarte;i++)
    { 
      if(sem_init(semadrfen_in[i], 0,0)==-1);
      { 
	printf("impossible de creer le semaphores in\n");
	  exit(-1);
      }
      if(sem_init(semadrfen_out[i], 0,2)==-1);
      { 
	printf("impossible de creer le semaphores out\n");
	  exit(-1);
      }
    }
  

  /*creation de la shm des fenetre des differentes cartes*/
  for(i=0;i<nbcarte;i++)
    {
      shmadr_fenetre1[i]=creershm_fenetre();
      shmadr_fenetre2[i]=creershm_fenetre();
    }
  
  //creation du shm, la creation d un shm alloue la memoire (donc pas de malloc)
  shm_text_client=(GLuint *)shmat( shmget(IPC_PRIVATE,1024,0666|IPC_CREAT) ,0,0);  //1024 textures max




  /*creationt du tableau des sem*/
  semap_in = malloc(nbcarte*sizeof(sem_t *));
  semap_out = malloc(nbcarte*sizeof(sem_t *));  

  for(i;i<nbcarte;i++){
    sem_init(semap_in[i],0,0);
    sem_init(semap_out[i],0,128);
  }//on creer un decalage entre les semaphores pour faire un effet de fifo


  /*chaque client utilise un seul semaphore du tableau */
  /*le bonn semaphore correspondant a num_client*/
  


  //on creer un pere et nbcarte fils
  
  client_num=nbcarte;
  //avant le fork, on creer le shm
  fifo_init();
  //maintenant qu'on a creer  la fifo on creer le tab contenant les indice des consommateurs dans la structure
  //fifo.idx=(uint32_t *)malloc(nbcarte*sizeof(uint32_t));
  

  
  /*boucle de creation des process*/
  
  i=0;

  varfork=1;//pour ne pas executer la partie de code fils au 1er tour
  
  for(i;i<nbcarte;i++){
    if(varfork==0){
      //on est dans un des processus fils
      varfork=-2;//pour ne pas y repasser au prochain increment
      client_num=i-1;
         
      if (creerpbuffer(width,heightclient[client_num])) {//chaque fils doit crer son pbuffer
	printf("Error:couldn't create pbuffer");
	exit(0);
      }
      tabtext=malloc(sizeof(GLuint)*1024);//alloue le tableau des textures sur chaques client
      idx=0;

    }
    else{
      if(varfork>0){
	//on est dasn le pere
	varfork=fork();//le pere restera sup a 0 et va donc y repasser, et creer d'autre fils
	//varfork des fils sera  a 0 et ils passeront donc par le 1er if
	if(varfork==-1){perror("fork"); exit(0);}
      }
    }

    //fifo.idx=0;
    cmd_fifo_idx=0;

  }
  /*process creer*/


  if(varfork==0){
    client_num=i-1;
    varfork=-2;
  }//on s'occupe du derrneir fils vu qu'on ne l'a pas fait avant;
  else if(varfork>0)
    load_library();
       
} 

