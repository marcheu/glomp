#include "init.h"
#include "lib_funcs.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>


/********variable global a tous le prog (cf init.h)*************/
int client_num;
int nbcarte;

sem_t * semadrfen_in;
sem_t * semadrfen_out;

int * client_load;


int width,height;/*taille de la fenetre de l'appli*/
/****************************************************************/


/*lance la boucle principale des clients qui vont desencapsuler les focntions
de la fifo avec GLOMPunpack*/
static void GLOMPclient_run()
{
	while(1){
		GLOMPunpack();
	}
}


/*initialise tout le programme
se lance avant le main du prog OpenGL
creer la fifo, les pointeurs de fonctions, ...*/
static void GLOMP_init()
{
	static int init=0;
	if (init==1)
		return;

	init=1;

	int i;
	Display* dpy;/*pointeur sur la structure Display que nous allons ouvrir*/
	char* force;/*utile si on souhaite forcer un nbr de GPU avec la variable d'env FORCE_GPU (cf ~l.65)*/

	lib_funcs_init();/*charge les pointeurs des fonction OpenGL (cf overrides), a faire avant le fork, tous le monde en a besoin*/

	/*comptage des cartes, et placement definitif dans nbcartes*/
	dpy = lib_XOpenDisplay("");
	nbcarte=ScreenCount(dpy);
	lib_XCloseDisplay(dpy);
	printf("Initializing GLOMP found %d GPUs",nbcarte);
	/*a moins que l'on ait creer une variable d'environnement FORCE_GPU qui le modifie de suite*/  
	/*potentially override the number of GPUs*/
	force=getenv("FORCE_GPU");
	if (force)
	{
		printf(" (forcing %d GPUs)",atoi(force));
		nbcarte=atoi(force);
	}
	printf("\n");

	GLOMPsegment_create_retour();/*procedure de creation d'un segmant pour le retour*/


	/*initilisation (bidon de client_load)*/
	/*doit servir a repartir le travaille sur les GPU*/
	/*soit parce qu'elle ont des puissances differentes*/
	/*soit perce qu'une des crates travaille sur une portion de zone + simple (avec moins de vertex par exemple)*/
	/*initially everyone has the same load*/
	client_load=(int *)malloc(nbcarte*sizeof(int));
	for(i=0;i<nbcarte;i++)
		client_load[i]=1;


	client_num=0;/*on met client_num a 0 pour que le pere ait la bonne valeur*/

	GLOMPfifo_init(&GLOMPcmd_fifo);/*avant le fork, on initialise la fifo*/

	/*initialisation du tableau des clef pour les segments de memoire partage, pour que tous les proces ait le meme (cf transfertFenetre)*/
#ifdef ENABLE_SINGLE_SCREEN
	GLOMP_single_screen_init();
#else
	GLOMP_multi_screen_init();
#endif

	/*spawn client processes*/
	for(i=1;i<nbcarte;i++)
	{
		pid_t pid;
		pid=fork();
		if (pid==0)
		{
			/*child process*/
			client_num=i;/*client num = num du tour de creation (de 0 a 3) */
			break;
		}
	}

	/*pour les client on les initialise et on les lances*/
	GLOMPclient_init();

#if 1
	if (client_num!=0){  
		GLOMPclient_run();/*tourne en boucle*/
	}
#else

	/*pour le debugage les segfault des client ne sont pas afficher*/
	/*ce bout de code inverse les serveur et cleint et permet donc l'affichage des segfault*/

	if (client_num==nbcarte-1)
		client_num=0;
	else if (client_num==0)
		client_num=nbcarte-1;

	if (client_num!=0) 
		GLOMPclient_run();/*tourne en boucle*/

#endif
}


/* Modify the __start function by overiding this function */

int __libc_start_main(int(*main_fct)(int, char **), int argc, 
		char ** ubp_av,void (*init) (void), void (*fini) (void), 
		void (*rtld_fini) (void),void (* stack_end)) {

	/*pointeur servant a deriver la fonction main*/
	int (*func)(int(*)(int, char **), int, char **,
			void (*) (void), void (*) (void), 
			void (*) (void), void(*));

	/*place le pointeur du main*/
	func  = dlsym(((void *) -1l), "__libc_start_main");

	GLOMP_init();/*lance l'initilisation de GLOMP*/
	int r= func(main_fct, argc, ubp_av, init, fini, rtld_fini, stack_end);/*lance le main du prog OpenGL*/
	return r;
}



