#include "init.h"
#include "pbuffer.h"
#include "transfertFenetre.h"




int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarre
int nbcarte;//nombre de GPU dispo

int width,height;//taille de la zone

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

static void init_client()
{
	// create the pbuffer
	if (creerpbuffer(width,height)) {
		printf("Error:couldn't create pbuffer");
		exit(0);
	}
	// allocate texture table
	tabtext=malloc(sizeof(GLuint)*1024);
	idx=0;
}

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
			init_client();
			break;
		}
	}

	load_library();
       
} 

