#include "fifo.h"

#define NBCARTE 4
#define SEMKEY  64

unit8_t* cmd_fifo;
uint32_t cmd_fifo_idx;
int client_num;



/* library interception variables */
static void* lib_handle = 0;
static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable) = 0;
static GLXWindow (*lib_glXCreateWindow)(Display *dpy, GLXFBConfig config,
			  Window win, const int *attrib_list);


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



/*
 * Load GLX library and find glXSwapBuffers() & other function
 */
inline static void load_library(void)
{
  /* open library (NOTE: you may need to change this filename!) */
  lib_handle = dlopen("/usr/lib/libGL.so", RTLD_LAZY);///charge la liib.so

  if (!lib_handle){perror("lib");exit(0);}

  /* intercept library glxSwapBuffers function */
  lib_glXSwapBuffers = dlsym(lib_handle, "glXSwapBuffers");    ///met le bon pointer dans lib_glX
  lib_glXCreateWindow = dlsym(lib_handle, "glXCreateWindow");    ///met le bon po\inter dans lib_glX

}


/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 * Load library if necessary. Then dump a frame and call the "real"
 * glXSwapBuffers function.
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)///changement de la func swap
{
  if (!lib_handle)
    {
      load_library();
    }


  lib_glXSwapBuffers(dpy, drawable);

}

/*
 * Our glXCreateWindow function that intercepts the "real" function.
 *
 * Load library if necessary. create 4 window
 */
GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config,Window win, const int *attrib_list)
{
  if (!lib_handle)
    {
      load_library();
    }



}
