#include <sys/types.h>
#include <unistd.h>
#include "init.h"
#include "pbuffer.h"
#include "client.h"
#include "server.h"
#include "overrides.h"
#include "transfertFenetre.h"

// FIXME deplacer ca dans dewrapped.h
extern void creertabfunc();


int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarre
int nbcarte;//nombre de GPU dispo

int width,height;//taille de l'ecran

void **shmadr_fenetre1,**shmadr_fenetre2;
sem_t **semadrfen_in,**semadrfen_out;
int fenetreactive=0;

GLuint *tabtext;//le tableau des etxtures
GLuint *shm_text_client;//un shm pour le tableau des textures
int tailletabtext=0;

GLuint conteur_textures=0;
GLuint * tabtextures;
	
pthread_mutex_t *mutex2D;//le mutex pour proteger les texture 2D
void *shm2D;

pthread_mutex_t *mutexSub2D;//le mutex pour proteger les texture Sub2D
void *shmSub2D;

pthread_mutex_t *mutexBitmap;//le mutex pour proteger les texture bitmap
void *shmBitmap;

void init()
{
	int i;

	// figure out the number of clients
	Display* dpy = XOpenDisplay("");
	nbcarte=ScreenCount(dpy);

	// potentially override the number of GPUs
	char* force=getenv("FORCE_GPU");
	if (force)
	{
		printf("forcing %d GPU\n",atoi(force));
		nbcarte=atoi(force);
	}


	shmadr_fenetre1=malloc(sizeof(void *)*nbcarte);
	shmadr_fenetre2=malloc(sizeof(void *)*nbcarte);
	semadrfen_in=malloc(sizeof(sem_t *)*nbcarte);
	semadrfen_out=malloc(sizeof(sem_t *)*nbcarte);

	/*creation de la shm des fenetre des differentes cartes*/
	for(i=0;i<nbcarte;i++)
	{
		shmadr_fenetre1[i]=creershm_fenetre();
		shmadr_fenetre2[i]=creershm_fenetre();
	}

	//creation du shm, la creation d un shm alloue la memoire (donc pas de malloc)
	shm_text_client=(GLuint *)shmat( shmget(IPC_PRIVATE,1024,0666|IPC_CREAT) ,0,0);  //1024 textures max

	// initially everyone has the same load
	for(i=1;i<nbcarte-1;i++)
		clientload[i]=10;


	client_num=nbcarte;
	// avant le fork, on initialise la fifo
	fifo_init(&cmd_fifo);

	// spawn client processes
	for(i=0;i<nbcarte;i++)
	{
		pid_t pid;
		pid=fork();
		if (pid==0)
		{
			// child process
			client_num=i;
			break;
		}
	}

	// initialize
	if (client_num==nbcarte)
		server_init();
	else
		client_init();
       
	if (client_num<nbcarte)
		client_run();

	// the server exits this ; the clients don't
} 

