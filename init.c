#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "init.h"
#include "pbuffer.h"
#include "client.h"
#include "server.h"
#include "overrides.h"
#include "transfertFenetre.h"



int (*func)(int(*)(int, char **), int, char **,
	    void (*) (void), void (*) (void), 
	    void (*) (void), void(*));


int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarte
int nbcarte;//nombre de GPU dispo


int fenetreactive=0;


/**/
static void GLOMPclient_run()
{

  // main loop
  while(1){ 
    //    printf("FUCK DA FUNC !!!!!!\n");
    GLOMPunpack();
  }
}


/**/
void initGlobal()
{
  int i;

  printf("ENTER MAIN\n");
  

  // figure out the number of clients
  static void* lib_handle_libX = 0;
  Display* (*lib_XOpenDisplay)(char*);
  int (*lib_ScreenCount)(Display*);
  int (*lib_XCloseDisplay)(Display*);
  lib_handle_libX = dlopen("/usr/lib/libX11.so", RTLD_LAZY);


  lib_XOpenDisplay=dlsym(lib_handle_libX, "XOpenDisplay");

  lib_XCloseDisplay=dlsym(lib_handle_libX, "XCloseDisplay");


  Display* dpy = lib_XOpenDisplay("");

  nbcarte=ScreenCount(dpy);
  lib_XCloseDisplay(dpy);
  printf("%d cartes\n",nbcarte);
  
  // potentially override the number of GPUs
  char* force=getenv("FORCE_GPU");
  if (force)
    {
      printf("forcing %d GPU\n",atoi(force));
      nbcarte=atoi(force);
    }
  
  createSem();

  segment_create_retour();
  
  // initially everyone has the same load
  // FIXME : make this a shm ?
  client_load=(int *)malloc(nbcarte*sizeof(int));
  for(i=0;i<nbcarte;i++)
    client_load[i]=1;
	
  client_num=nbcarte;
  // avant le fork, on initialise la fifo
  fifo_init(&cmd_fifo);


  server_init();//tout le monde veux le pointer cf ligne 116


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

  //initialisation du tableau des clef pour les segments de memoire partage
  initTabKey();
  
   
  // initialize
  if (client_num==nbcarte){
    //server_init();
  }
  
  else{  
    client_init();
    GLOMPclient_run();
    }
  
  /*
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
      client_init();
      GLOMPclient_run();
     
    }
  */
  


} 








/* Modify the __start function by overiding this function */

int __libc_start_main(int(*main_fct)(int, char **), int argc, 
		      char ** ubp_av,void (*init) (void), void (*fini) (void), 
		      void (*rtld_fini) (void),void (* stack_end)) {
 
  if(!func) {
    func  = dlsym(((void *) -1l), "__libc_start_main");
    
  }
  
  printf("\nStarting MAIN\n");
  initGlobal();
  printf("\nokg MAIN\n");
  func(main_fct, argc, ubp_av, init, fini, rtld_fini, stack_end);

  //exit(0);
  
  
  printf("bloque ?%d\n",client_num);

  //return result;

  
}




