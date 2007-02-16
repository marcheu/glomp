#include "fifo.h"


#define SEMKEY  64


struct{
  uint8_t* cmd_fifo;
  uint32_t cmd_fifo_idx;//indice
  uint32_t* idx; //tableau des indice client
}fifo;


int client_num;
int nbcarte;


sem_t **semap;
/* library interception variables */
static void* lib_handle = 0;
static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable) = 0;
static GLXWindow (*lib_glXCreateWindow)(Display *dpy, GLXFBConfig config,
			  Window win, const int *attrib_list);


	



void glop_init(){
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



  //creation d'un pere et de 4 fils 
  //le pere etant le producteur dans le fifo
  //les fils serotn 4 consomateurs



  /*creationt du tableau des sem*/
  *semap = (sem_t *)malloc(NBCARTE*sizeof(sem_t));
  int i;
  for(i;i<nbcarte;i++)
    sem_init(*semap[i],1,0);

  /*chaque client utilise un seul semaphore du tableau */
  /*le bonn semaphore correspondant a num_client*/








  client_num=nbcarte;
  //avant le fork, on creer le shm
  creerFifo();
  //maintenant qu'on a creer  la fifo on creer le tab contenant les indice des consommateurs dans la structure
  fifo.idx=(uint32_t *)malloc(nbcarte*sizeof(uint32_t));



  /*boucle de creation des process*/

    int i;
    i=0;
    int varfork;
    varfork=1;

    for(i;i<nbcarte;i++){
      if(varfork==0){
	//on est dans un des processus fils
	varfork=1;//pour ne pas y repasser au prochain increment
	client_num=i-1;
      }
      else{
	//on est dasn le pere
	varfork=fork();
	if(varfork==-1){perror("fork");return -1;}
      }

    }
    /*process creer*/


    if(varfork==0){
      client_num=i-1;
    }//on s'occupe du derrneir fils vu qu'on ne l'a pas fait avant;

  


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
  lib_glXCreateWindow = dlsym(lib_handle, "glXCreateWindow");    ///met le bon pointer dans lib_glX

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


}
