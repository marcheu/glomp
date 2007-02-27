#include "overrides.h"
/* functions we implement ourselves */




/*
 * Load GLX & X library and find glXSwapBuffers() & other function
 */
void load_library(void)
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
  lib_glTexImage2D = dlsym(lib_handle_libGL, "glTexImage2D");
  
  /*intercepte ls fonction X*/

  if (!lib_handle_libX11){perror("lib");exit(0);}

  lib_XSetStandardProperties = dlsym(lib_handle_libX11, "XSetStandardProperties");
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
  width=hints->width;
  height=hints->height;
  
  //on les divisent et on repartie les taches entre chaques clients
  for(i=1;i<nbcarte-1;i++)
	  clientload[i]=10;
  
  //on relance la fonction
  lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}



void glFrustum ( GLdouble p0 , GLdouble p1 , GLdouble p2 , GLdouble p3 , GLdouble p4 , GLdouble p5 )
{
	int fnum=OVERRIDE_BASE;
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
  
  int totalload=0;
  int beforeload=0;
  for(i=0;i<nbcarte;i++)
  {
	  totalload+=clientload[i];
	  if (i<num_client)
		beforeload+=clientload[i];
  }
  
  newp2=(p3-p2)*(double)beforeload/(double)totalload;
  newp3=(p3-p2)*(double)(beforeload+clientload[client_num])/(double)totalload;

  lib_glFrustum(p0,p1,newp2,newp3,p4,p5);


}


void glGenTextures ( GLsizei p0 , GLuint *p1 )
{
  int i;
  
  int fnum=OVERRIDE_BASE+1;
  int fflags=0;
  
  OUTPUT_FIFO(&fnum,sizeof(fnum));
  OUTPUT_FIFO(&fflags,sizeof(fflags));
  OUTPUT_FIFO(&p0,4);
  OUTPUT_FIFO(&p1,4);

  for(i=0;i<p0;i++)
  {
     p1[i]=conteur_textures;
     conteur_textures++;
  }  


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

  realloc(tabtextures,sizeof(GLuint)*(conteur_textures+p0));

  for(i=0;i<p0;i++)
  {
        tabtextures[conteur_textures+i]=p1[i];
        conteur_textures++;
  }

}


void glBindTexture ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+2;
  int fflags=0;

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
  
  p1=tabtextures[p1];
  
  lib_glBindTexture ( p0 , p1 );//on utilise le vrai bind texture
}



/*la fonction TexImage2D va renvoyer ses info dans la fifo, et au meme moment creer un shm ou on place la texture qui va pouvoir etre recuperer*/
void glTexImage2D( GLenum p0,
		   GLint p1,
		   GLint p2,
		   GLsizei p3,
		   GLsizei p4,
		   GLint p5,
		   GLenum p6,
		   GLenum p7,
		   const GLvoid *p8 ){


  //on detruit le shm si il existe
  shmdt(shm2D);

 
  //code de glTex 
  int i;
  
  int fnum=OVERRIDE_BASE+3;
  int fflags=0;
  
  pthread_mutex_lock(mutex2D);
  shm2D=(GLvoid *)shmat( shmget(IPC_PRIVATE,p3*p4*sizeof(p7),0666|IPC_CREAT) ,0,0);
  memcpy(&shm2D[0],p8,p3*p4*sizeof(p7));//on copie la text dans le shm

  
  OUTPUT_FIFO(&fnum,sizeof(fnum));
  OUTPUT_FIFO(&fflags,sizeof(fflags));
  OUTPUT_FIFO(&p0,4);
  OUTPUT_FIFO(&p1,4);
  OUTPUT_FIFO(&p2,4);
  OUTPUT_FIFO(&p3,4);
  OUTPUT_FIFO(&p4,4);
  OUTPUT_FIFO(&p5,4);
  OUTPUT_FIFO(&p6,4);
  OUTPUT_FIFO(&p7,4);
  

  pthread_mutex_unlock(mutex2D);





}

void fglTexImage2D(){
 
  GLenum p0;
  GLint p1;
  GLint p2;
  GLsizei p3;
  GLsizei p4;
  GLint p5;
  GLenum p6;
  GLenum p7;
  const GLvoid *p8;

  INPUT_FIFO(&p0,4);
  INPUT_FIFO(&p1,4);
  INPUT_FIFO(&p2,4);
  INPUT_FIFO(&p3,4);
  INPUT_FIFO(&p4,4);
  INPUT_FIFO(&p5,4);
  INPUT_FIFO(&p6,4);
  INPUT_FIFO(&p7,4);



  pthread_mutex_lock(mutex2D);
  memcpy(p8,&shm2D[0],p3*p4*sizeof(p7));//on recupere les valeurs du shm
  pthread_mutex_unlock(mutex2D);
  //et on appelle la vrai fonction tex2D pour charger les etxtures en memeoire GPU


  lib_glTexImage2D ( p0 , p1 ,p2,p3,p4,p5,p6,p7,p8);//p8 etant recuperer par le shm



}



