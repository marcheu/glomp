#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "fifo.h"
#include "transfertFenetre.h"

//la structure qui contient le shm servant de fifo
  uint8_t* cmd_fifo;//le shm en lui meme
  uint32_t cmd_fifo_idx;//indice du pere
  uint32_t idx; //tableau des indice client


int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarre
int nbcarte;//nombre de GPU dispo

int width,height;//taille de la zone
int * heightclient;

void **shmadr_fenetre1,**shmadr_fenetre2;
sem_t **semadrfen_in,**semadrfen_out;
int fenetreactive=0;

GLuint *tabtext;//le tableau des etxtures
GLuint *shm_text_client;//un shm pour le tableau des textures
pthread_mutex_t *mutex;//le mutex qui le protege
int tailletabtext=0;

sem_t **semap_in, **semap_out;//les semaphore pour proteger la fifo

/* library interception variables */
static void* lib_handle_libGL = 0;
static void* lib_handle_libX11 = 0;

/*les poiinteur sur les fonction à dériver*/
static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable) = 0;
static int (*lib_XSetStandardProperties)(
    Display*		/* display */,
    Window		/* w */,
    _Xconst char*	/* window_name */,
    _Xconst char*	/* icon_name */,
    Pixmap		/* icon_pixmap */,
    char**		/* argv */,
    int			/* argc */,
    XSizeHints*		/* hints */
) = 0;
static GLXWindow (*lib_glXCreateWindow)(Display *dpy, GLXFBConfig config,
			  Window win, const int *attrib_list)=0;

static void (*lib_glBindTexture) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenTextures) ( GLsizei p0 , GLuint *p1 )=0;	
static void (*lib_glFrustum) ( GLdouble left,
			       GLdouble right,
			       GLdouble bottom,
			       GLdouble top,
			       GLdouble zNear,
			       GLdouble zFar	)=0;	



/*
 * Load GLX & X library and find glXSwapBuffers() & other function
 */
inline static void load_library(void)
{
  /* open library (NOTE: you may need to change this filename!) */
  lib_handle_libGL = dlopen("/usr/lib/libGL.so", RTLD_LAZY);///charge la liib.so
  lib_handle_libX11 = dlopen("/usr/lib/libX11.so", RTLD_LAZY);
  
  if (!lib_handle_libGL){perror("lib");exit(0);}

  /* intercept library GL function */
  lib_glXSwapBuffers = dlsym(lib_handle_libGL, "glXSwapBuffers");    ///met le bon pointer dans lib_glX
  lib_glXCreateWindow = dlsym(lib_handle_libGL, "glXCreateWindow");   
  lib_glBindTexture = dlsym(lib_handle_libGL, "glBindTexture");    
  lib_glGenTextures = dlsym(lib_handle_libGL, "glGenTextures");    
  lib_glFrustum = dlsym(lib_handle_libGL, "glFrustum"); 
  
  /*intercepte ls fonction X*/

  if (!lib_handle_libX11){perror("lib");exit(0);}

  lib_XSetStandardProperties = dlsym(lib_handle_libX11, "XSetStandardProperties");
}





void glop_init(){
 
  int i;
  int varfork;
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


  //initailisation de tout les tableau      
  heightclient=malloc(sizeof(int)*nbcarte);

  shmadr_fenetre1=malloc(sizeof(void *)*nbcarte);
  shmadr_fenetre2=malloc(sizeof(void *)*nbcarte);
  semadrfen_in=malloc(sizeof(sem_t *)*nbcarte);
  semadrfen_out=malloc(sizeof(sem_t *)*nbcarte);

  
  //initialisation des semaphore
  for(i=0;i<nbcarte;i++)
    { 
      if(sem_init(semadrfen_in[i], 0,0)==-1);
      { 
	printf("impossible de creer le semaphores in\n");
	  exit(-1);
      }
      if(sem_init(semadrfen_out[i], 0,2)==-1);
      { 
	printf("impossible de creer le semaphores out\n");
	  exit(-1);
      }
    }
  

  /*creation de la shm des fenetre des differentes cartes*/
  for(i=0;i<nbcarte;i++)
    {
      shmadr_fenetre1[i]=creershm_fenetre();
      shmadr_fenetre2[i]=creershm_fenetre();
    }
  
  //creation du shm, la creation d un shm alloue la memoire (donc pas de malloc)
  shm_text_client=(GLuint *)shmat( shmget(IPC_PRIVATE,1024,0666|IPC_CREAT) ,0,0);  //1024 textures max




  /*creationt du tableau des sem*/
  semap_in = malloc(nbcarte*sizeof(sem_t *));
  semap_out = malloc(nbcarte*sizeof(sem_t *));  

  for(i;i<nbcarte;i++){
    sem_init(semap_in[i],0,0);
    sem_init(semap_out[i],0,128);
  }//on creer un decalage entre les semaphores pour faire un effet de fifo


  /*chaque client utilise un seul semaphore du tableau */
  /*le bonn semaphore correspondant a num_client*/
  


  //on creer un pere et nbcarte fils
  
  client_num=nbcarte;
  //avant le fork, on creer le shm
  creerFifo();
  //maintenant qu'on a creer  la fifo on creer le tab contenant les indice des consommateurs dans la structure
  //fifo.idx=(uint32_t *)malloc(nbcarte*sizeof(uint32_t));
  

  
  /*boucle de creation des process*/
  
  i=0;

  varfork=1;//pour ne pas executer la partie de code fils au 1er tour
  
  for(i;i<nbcarte;i++){
    if(varfork==0){
      //on est dans un des processus fils
      varfork=-2;//pour ne pas y repasser au prochain increment
      client_num=i-1;
         
      if (creerpbuffer(width,heightclient[client_num])) {//chaque fils doit crer son pbuffer
	printf("Error:couldn't create pbuffer");
	exit(0);
      }
      tabtext=malloc(sizeof(GLuint)*1024);//alloue le tableau des textures sur chaques client
      idx=0;

    }
    else{
      if(varfork>0){
	//on est dasn le pere
	varfork=fork();//le pere restera sup a 0 et va donc y repasser, et creer d'autre fils
	//varfork des fils sera  a 0 et ils passeront donc par le 1er if
	if(varfork==-1){perror("fork"); exit(0);}
      }
    }

    //fifo.idx=0;
    cmd_fifo_idx=0;

  }
  /*process creer*/


  if(varfork==0){
    client_num=i-1;
    varfork=-2;
  }//on s'occupe du derrneir fils vu qu'on ne l'a pas fait avant;
  else if(varfork>0)
    load_library();
       
  


} 




/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)///changement de la func swap
{
  int i;

  if(client_num<nbcarte) 
    lire_fenetre();/*on utilise lire_fenetre pour remplire des buffer*/
  else {
    ecirre_fenetre();//si on est dans le maitre, on recupere les buffers
    lib_glXSwapBuffers(dpy, drawable);//et on utilise la vrai fonction swapbuffer
  }
  
}

 

/*on recupere les proprite de la fenetre (taille) pour les diviser par le nbr de cartes, afin de repartire les taches,puis on la lance*/
int XSetStandardProperties(
    Display*            dpy,
    Window	        w,
    _Xconst char*	name,
    _Xconst char*	icon_string,
    Pixmap		icon_pixmap,
    char**		argv,
    int			argc,
    XSizeHints*		hints
                                    )
{

  int i;
  //recupere les tailles
  memcpy(&width,hints,sizeof(int));
  memcpy(&height,&hints+sizeof(int),sizeof(int));
  
  //on les divisent et on repartie les taches entre chaques clients
  for(i=1;i<nbcarte-1;i++)
    heightclient[i]=height/nbcarte;
  heightclient[i]=height-height*nbcarte;
  
  //on relance la fonction
  lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}



void glFrustum ( GLdouble p0 , GLdouble p1 , GLdouble p2 , GLdouble p3 , GLdouble p4 , GLdouble p5 )
{
	int fnum=8000;
	int fflags=0;
	OUTPUT_FIFO(&fnum,sizeof(fnum));
	OUTPUT_FIFO(&fflags,sizeof(fflags));
	OUTPUT_FIFO(&p0,8);
	OUTPUT_FIFO(&p1,8);
	OUTPUT_FIFO(&p2,8);
	OUTPUT_FIFO(&p3,8);
	OUTPUT_FIFO(&p4,8);
	OUTPUT_FIFO(&p5,8);
	return ;

}

/*on a besoin de gerer la perspective donc on interceptent les info de glfrumstun*/
void fglFrustum()
{
  int i;

  GLdouble p0;
  GLdouble p1;
  GLdouble p2;
  GLdouble p3;
  GLdouble p4;
  GLdouble p5;
  INPUT_FIFO(&p0,8);
  INPUT_FIFO(&p1,8);
  INPUT_FIFO(&p2,8);
  INPUT_FIFO(&p3,8);
  INPUT_FIFO(&p4,8);
  INPUT_FIFO(&p5,8);
  
  for(i=0;i<client_num-1;i++)
    p2=p2+heightclient[i]; 
  p3=p2+heightclient[i]; 
  
  lib_glFrustum(p0,p1,p2,p3,p4,p5);


}


void glGenTextures ( GLsizei p0 , GLuint *p1 )
{
  int i;
  
  int fnum=8001;
  int fflags=0;
  
  
  OUTPUT_FIFO(&fnum,sizeof(fnum));
  OUTPUT_FIFO(&fflags,sizeof(fflags));
  OUTPUT_FIFO(&p0,4);
  OUTPUT_FIFO(&p1,4);
  pthread_mutex_lock(mutex);
  memcpy(p1,&shm_text_client[tailletabtext],sizeof(GLuint)*p0);//on cherche la position de la texture dans le shm (apres avoir bloquee le mutex)
  tailletabtext=tailletabtext+p0;
  
}
/*les texture pose un probleme
chaque GPU lance glggentexture pour generer un numeros de texure aleatoirement
mais nous devons etablire une correpondance entre ces valeurs
pour cela nous les stoquons dans des tableaux
et le premier*/
void fglGenTextures()
{
  int i;

  GLsizei p0;
  GLuint* p1;
  INPUT_FIFO(&p0,4);
  INPUT_FIFO(&p1,4);
 
  lib_glGenTextures ( p0 , p1 );


  memcpy(&tabtext[tailletabtext],&p1,sizeof(GLuint)*p0);
  if(client_num==0)
    {//le client 0 met la position dans el shm
      memcpy((void *)shm_text_client[tailletabtext],p1,sizeof(GLuint)*p0);
      pthread_mutex_unlock(mutex);
    }
  tailletabtext=tailletabtext+p0;
}


void glBindTexture ( GLenum p0 , GLuint p1 )
{

  GLuint i=0;
  int fnum=8002;
  int fflags=0;

  
  while( memcmp((const void *)shm_text_client[i],&p1,sizeof(GLuint)) !=0 )    
    i++;           
 
  p1=i;
	
  OUTPUT_FIFO(&fnum,sizeof(fnum));
  OUTPUT_FIFO(&fflags,sizeof(fflags));
  OUTPUT_FIFO(&p0,4);
  OUTPUT_FIFO(&p1,4);
  return ;
  
}

void fglBindTexture()
{
  GLenum p0;
  GLuint p1;
  INPUT_FIFO(&p0,4);
  INPUT_FIFO(&p1,4);
  
  p1=tabtext[p1];
  
  lib_glBindTexture ( p0 , p1 );//on utilise le vrai bind texture
}

int main()
{

}




