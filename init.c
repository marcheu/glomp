#include "init.h"

/*pointeur servant a deriver la fonction main*/
int (*func)(int(*)(int, char **), int, char **,
	    void (*) (void), void (*) (void), 
	    void (*) (void), void(*));


/********variable global a tous le prog (cf init.h)*************/
int client_num;
int nbcarte;
int fenetreactive=0;

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
static void initGLOMP()
{
  int i;

  static void* lib_handle_libX = 0;/*pointeur permettant d'ouvrir la lib*/
  /*creation des pointeurs sur les fonctions que nous allons utiliser*/
  Display* (*lib_XOpenDisplay)(char*);
  int (*lib_ScreenCount)(Display*);
  int (*lib_XCloseDisplay)(Display*);

  Display* dpy;/*pointeur sur la structure Display que nous allons ouvrir*/
  
  char* force;/*utile si on souhaite forcer un nbr de GPU avec la variable d'env FORCE_GPU (cf ~l.65)*/
  

  printf("INITIALISATION de GLOMP\n");
  

  /*les fontion X ne sont plus dispo (ouverture de lib etc)
donc il s'agit de retrouver les pointeur avec des dlopen dlsym */
  lib_handle_libX = dlopen("/usr/lib/libX11.so", RTLD_LAZY);/*ouverture de la lib X*/
  /*attachement des pointeurs sur les fonctions*/
  lib_XOpenDisplay=dlsym(lib_handle_libX, "XOpenDisplay");
  lib_XCloseDisplay=dlsym(lib_handle_libX, "XCloseDisplay");


  /*comptage des cartes, et placement definitif dans nbcartes*/
  dpy = lib_XOpenDisplay("");
  nbcarte=ScreenCount(dpy);
  lib_XCloseDisplay(dpy);
  printf("%d cartes\n",nbcarte);
  /*a moins que l'on ait creer une variable d'environnement FORCE_GPU qui le modifie de suite*/  
  /*potentially override the number of GPUs*/
  force=getenv("FORCE_GPU");
  if (force)
    {
      printf("forcing %d GPU\n",atoi(force));
      nbcarte=atoi(force);
    }
  
  GLOMPcreateSem();/*procedure de creation des semaphore (cf transfertFentre)*/
  GLOMPsegment_create_retour();/*procedure de creation d'un segmant pour le retour*/
  

  /*initilisation (bidon de client_load)*/
  /*doit servir a repartir le travaille sur les GPU*/
  /*soit parce qu'elle ont des puissances differentes*/
  /*soit perce qu'une des crates travaille sur une portion de zone + simple (avec moins de vertex par exemple)*/
  /*initially everyone has the same load*/
  client_load=(int *)malloc(nbcarte*sizeof(int));
  for(i=0;i<nbcarte;i++)
    client_load[i]=1;

	
  client_num=nbcarte;/*on met client_num a nbcarte pour que le pere ait la bonne valeur*/
  
  GLOMPfifo_init(&GLOMPcmd_fifo);/*avant le fork, on initialise la fifo*/

  load_library();/*charge les pointeurs des fonction OpenGL (cf overrides), a faire avant le fork, tous le monde en a besoin*/

  /*initialisation du tableau des clef pour les segments de memoire partage, pour que tous les proces ait le meme (cf transfertFenetre)*/
  GLOMPinitTabKey();

  /*spawn client processes*/
  for(i=0;i<nbcarte;i++)
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
#if 0
  if (client_num!=nbcarte){  
    GLOMPclient_init();/*creer les tab des fonction GLOMP, initialise les pointeur glX, ...*/
    GLOMPclient_run();/*tourne en boucle*/
    }
#else

  /*pour le debugage les segfault des client ne sont pas afficher*/
  /*ce bout de code inverse les serveur et cleint et permet donc l'affichage des segfault*/
  
    switch(client_num)
    {
    case 0:
      client_num=4;     
      //server_init();//tout le monde a besoin ! des pointeurs cf ligne 88
      
      break;
      
    case 4:
      client_num=0;
    case 1:
    case 2:
    case 3:
      GLOMPclient_init();
      GLOMPclient_run(); 
    }
#endif
  

  
}


/* Modify the __start function by overiding this function */

int __libc_start_main(int(*main_fct)(int, char **), int argc, 
		      char ** ubp_av,void (*init) (void), void (*fini) (void), 
		      void (*rtld_fini) (void),void (* stack_end)) {
 
  /*place le pointeur du main*/
  if(!func) {
    func  = dlsym(((void *) -1l), "__libc_start_main");
    
  }
  initGLOMP();/*lance l'initilisation de GLOMP*/
  func(main_fct, argc, ubp_av, init, fini, rtld_fini, stack_end);/*lance le main du prog OpenGL*/
}




