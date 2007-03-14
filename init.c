#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "init.h"
#include "pbuffer.h"
#include "client.h"
#include "server.h"
#include "overrides.h"
#include "transfertFenetre.h"


// FIXME deplacer ca dans dewrapped.h
//extern void creertabfunc();


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
  segment_create_retour();
  
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


  //  int func;
  //  fifo_input(&cmd_fifo,&func,4);


  //GLOMPclient_run(); 
  // initialize
  if (client_num==nbcarte){
    
      server_init();
  }
	  
  else{  
    client_init();
    printf("sort\n");
    GLOMPclient_run();    
  }


} 








/* Modify the __start function by overiding this function */

int __libc_start_main(int(*main_fct)(int, char **), int argc, 
		      char ** ubp_av,void (*init) (void), void (*fini) (void), 
		      void (*rtld_fini) (void),void (* stack_end)) {

  int (*func)(int(*)(int, char **), int, char **,
	      void (*) (void), void (*) (void), 
	      void (*) (void), void(*));
 int result;
  
  if(!func) {
    func  = dlsym(((void *) -1l), "__libc_start_main");
  }
  
  printf("Starting MAIN\n");
  atexit(initGlobal);
  //initGlobal();
  
  result = func(main_fct, argc, ubp_av, init, fini, rtld_fini, stack_end);
  printf("bloque ?%d\n",client_num);
  
  return result;
}


