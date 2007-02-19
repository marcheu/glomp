#include "fifo.h"


#define SEMKEY  64


struct{
  uint8_t* cmd_fifo;
  uint32_t cmd_fifo_idx;//indice
  uint32_t* idx; //tableau des indice client
}fifo;


int client_num;
int nbcarte;

int width,height;
GLXContext glx;

char **shmadr_fenetre1,**shmadr_fenetre2;
sem_t **semadrfen_in,**semadrfen_out;


sem_t **semap_in, **semap_out;

/* library interception variables */
static void* lib_handle_libGL = 0;
static void* lib_handle_libX11 = 0;
static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable) = 0;
static void (*lib_XSetStandardProperties)( Display *dpy,
                             Window w,
                             _Xconst char *name,
                             _Xconst char *icon_string,
                             Pixmap icon_pixmap,
                             char **argv,
                             int argc,
                             XSizeHints *hints  ) = 0;
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



    shmadr_fenetre1=malloc(sizeof(char *shmadr)*nbcarte);
    shmadr_fenetre2=malloc(sizeof(char *shmadr)*nbcarte);
    semadrfenin=malloc(sizeof(sem_t *)*nbcarte);
    semadrfenout=malloc(sizeof(sem_t *)*nbcarte);


  for(i=0;i<nbcarte;i++)
  {    if(sem_init(semadrfen_in[i], 0,0)==-1);
    { 
      printf("impossible de creer le semaphores in\n")
      exit(-1);
    }
    if(sem_init(semadrfen_out[i], 0,2)==-1);
    { 
      printf("impossible de creer le semaphores out\n")
      exit(-1);
    }
  }


   /*creation de la shm des fenetre des differentes cartes*/
  for(i=0;i<nbcarte;i++)
  {
    shmadr_fenetre1[i]=creershm_fenetre();
    shmadr_fenetre2[i]=creershm_fenetre();
  }
  

  //creation d'un pere et de 4 fils 
  //le pere etant le producteur dans le fifo
  //les fils serotn 4 consomateurs





  /*creationt du tableau des sem*/
  semap_in = (sem_t *)malloc(nbcarte*sizeof(sem_t *));
  semap_out = (sem_t *)malloc(nbcarte*sizeof(sem_t *));  
  int i;
  for(i;i<nbcarte;i++){
    sem_init(semap_in[i],0,0);
    sem_init(semap_out[i],0,128);
  }


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
	  varfork=-2;//pour ne pas y repasser au prochain increment
	  client_num=i-1;
         
           if (creerpbuffer(width,height)) {
                   printf("Error:couldn't create pbuffer")
                   exit(0);
           }

      }
      else{
	if(varfork>0){
	//on est dasn le pere
	varfork=fork();
	if(varfork==-1){perror("fork");return -1;}
	}
      }

      fifo.idx[i]=0;
      fifo.cmd_fifo_idx=0;

    }
    /*process creer*/


    if(varfork==0){
      client_num=i-1;
      varfork=-2;
    }//on s'occupe du derrneir fils vu qu'on ne l'a pas fait avant;

  


} 



/*
 * Load GLX library and find glXSwapBuffers() & other function
 */
inline static void load_library(void)
{
  /* open library (NOTE: you may need to change this filename!) */
  lib_handle_libGL = dlopen("/usr/lib/libGL.so", RTLD_LAZY);///charge la liib.so

  if (!lib_handle_libGL){perror("lib");exit(0);}

  /* intercept library glxSwapBuffers function */
  lib_glXSwapBuffers = dlsym(lib_handle_libGL, "glXSwapBuffers");    ///met le bon pointer dans lib_glX
  lib_glXCreateWindow = dlsym(lib_handle_libGL, "glXCreateWindow");    ///met le bon pointer dans lib_glX


  lib_handle_libX11 = dlopen("/usr/lib/libX11.so", RTLD_LAZY);
  if (!lib_handle_libX11){perror("lib");exit(0);}

  lib_XSetStandardProperties = dlsym(lib_handle_libX11, "XSetStandardProperties");
}


/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 * Load library if necessary. Then dump a frame and call the "real"
 * glXSwapBuffers function.
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)///changement de la func swap
{

  lib_glXSwapBuffers(dpy, drawable);

}

/*
 * Our glXCreateWindow function that intercepts the "real" function.
 *
 * Load library if necessary. create 4 window
 */
GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config,Window win, const int *attrib_list)
{

}
 
void XSetStandardProperties( Display *dpy,
                             Window w,
                             _Xconst char *name,
                             _Xconst char *icon_string,
                             Pixmap icon_pixmap,
                             char **argv,
                             int argc,
                             XSizeHints *hints  )
{

  memcpy(&width,hints,sizeof(int));
  memcpy(&height,&hints+sizeof(int),sizeof(int));

  lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}



