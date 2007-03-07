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



int fenetreactive=0;

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



	createAllFen();

	// initially everyone has the same load
	// FIXME : make this a shm ?
	client_load=(int*)malloc(nbcarte*sizeof(int));
	for(i=0;i<nbcarte;i++)
		client_load[i]=10;

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
	if (client_num<nbcarte)
		exit(0);
} 

