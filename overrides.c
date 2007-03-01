#include "overrides.h"
#include "init.h"
/* functions we implement ourselves */

static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable)=0;
static int (*lib_XSetStandardProperties)(
    Display*		/* display */,
    Window		/* w */,
    _Xconst char*	/* window_name */,
    _Xconst char*	/* icon_name */,
    Pixmap		/* icon_pixmap */,
    char**		/* argv */,
    int			/* argc */,
    XSizeHints*		/* hints */
)=0;

static void (*lib_glBindTexture) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenTextures) ( GLsizei p0 , GLuint *p1 )=0;	
static void (*lib_glFrustum) ( GLdouble left,
			       GLdouble right,
			       GLdouble bottom,
			       GLdouble top,
			       GLdouble zNear,
			       GLdouble zFar	)=0;
static void (*lib_glTexImage2D) ( GLenum target,
			     GLint level,
			     GLint internalformat,
			     GLsizei width,
			     GLsizei height,
			     GLint border,
			     GLenum format,
			     GLenum type,
			     const GLvoid *pixels )=0;
static void (*lib_glTexSubImage2D) (GLenum target,
				GLint level,
				GLint xoffset,
				GLint yoffset,
				GLsizei	width,
				GLsizei	height,
				GLenum format,
				GLenum type,
				const GLvoid *pixels )=0;
static void (*lib_glBitmap)( GLsizei width,
			 GLsizei height,
			 GLfloat xorig,
			 GLfloat yorig,
			 GLfloat xmove,
			 GLfloat ymove,
			     const GLubyte *bitmap )=0;




/* library interception variables */
static void* lib_handle_libGL = 0;
static void* lib_handle_libX11 = 0;






/*
 * Load GLX & X library and find glXSwapBuffers() & other function
 */
void load_library(void)
{
  /* open library (NOTE: you may need to change this filename!) */
  lib_handle_libGL = dlopen("/usr/lib/libGL.so", RTLD_LAZY);
  
  if (!lib_handle_libGL){perror("lib");exit(0);}

  /* intercept library GL function */
  lib_glXSwapBuffers = dlsym(lib_handle_libGL, "glXSwapBuffers");
  lib_glXCreateWindow = dlsym(lib_handle_libGL, "glXCreateWindow");   
  lib_glBindTexture = dlsym(lib_handle_libGL, "glBindTexture");    
  lib_glGenTextures = dlsym(lib_handle_libGL, "glGenTextures");    
  lib_glFrustum = dlsym(lib_handle_libGL, "glFrustum"); 
  lib_glTexImage2D = dlsym(lib_handle_libGL, "glTexImage2D");
  lib_glTexSubImage2D = dlsym(lib_handle_libGL, "glTexSubImage2D");
  lib_glBitmap = dlsym(lib_handle_libGL, "glBitmap");
  
  /* intercept XSetStandardProperties */
  lib_handle_libX11 = dlopen("/usr/lib/libX11.so", RTLD_LAZY);

  if (!lib_handle_libX11){perror("lib");exit(0);}

  lib_XSetStandardProperties = dlsym(lib_handle_libX11, "XSetStandardProperties");
}



/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)
{
	int fnum=OVERRIDE_BASE+6;
	int fflags=0;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_flush();
	ecrire_fenetre();//si on est dans le maitre, on recupere les buffers
	lib_glXSwapBuffers(dpy, drawable);//et on utilise la vrai fonction swapbuffer
}

void fglXSwapBufferes()
{
	lire_fenetre();
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
  
 
  //on relance la fonction
  lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}


extern GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, const int *attrib_list)
{
	init();
	lib_glXCreateWindow(dpy,config,win,attrib_list);
}

void glFrustum ( GLdouble p0 , GLdouble p1 , GLdouble p2 , GLdouble p3 , GLdouble p4 , GLdouble p5 )
{
	int fnum=OVERRIDE_BASE;
	int fflags=0;
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_output(&cmd_fifo,&p0,8);
	fifo_output(&cmd_fifo,&p1,8);
	fifo_output(&cmd_fifo,&p2,8);
	fifo_output(&cmd_fifo,&p3,8);
	fifo_output(&cmd_fifo,&p4,8);
	fifo_output(&cmd_fifo,&p5,8);
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
  fifo_input(&cmd_fifo,&p0,8);
  fifo_input(&cmd_fifo,&p1,8);
  fifo_input(&cmd_fifo,&p2,8);
  fifo_input(&cmd_fifo,&p3,8);
  fifo_input(&cmd_fifo,&p4,8);
  fifo_input(&cmd_fifo,&p5,8);
  
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
  
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);

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
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
 
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
  return ;
  
}

void fglBindTexture()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
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

 
  
  int fnum=OVERRIDE_BASE+3;
  int fflags=0;
  
  pthread_mutex_lock(mutex2D);
  shm2D=(GLvoid *)shmat( shmget(IPC_PRIVATE,p3*p4*sizeof(p7),0666|IPC_CREAT) ,0,0);
  memcpy(&shm2D[0],p8,p3*p4*sizeof(p7));//on copie la text dans le shm

  
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
  fifo_output(&cmd_fifo,&p2,4);
  fifo_output(&cmd_fifo,&p3,4);
  fifo_output(&cmd_fifo,&p4,4);
  fifo_output(&cmd_fifo,&p5,4);
  fifo_output(&cmd_fifo,&p6,4);
  fifo_output(&cmd_fifo,&p7,4);
  

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

  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  fifo_input(&cmd_fifo,&p2,4);
  fifo_input(&cmd_fifo,&p3,4);
  fifo_input(&cmd_fifo,&p4,4);
  fifo_input(&cmd_fifo,&p5,4);
  fifo_input(&cmd_fifo,&p6,4);
  fifo_input(&cmd_fifo,&p7,4);



  pthread_mutex_lock(mutex2D);
  memcpy(p8,&shm2D[0],p3*p4*sizeof(p7));//on recupere les valeurs du shm
  pthread_mutex_unlock(mutex2D);
  //et on appelle la vrai fonction tex2D pour charger les etxtures en memeoire GPU


  lib_glTexImage2D ( p0 , p1 ,p2,p3,p4,p5,p6,p7,p8);//p8 etant recuperer par le shm



}


void glTexSubImage2D(GLenum p0,GLint p1,GLint p2,GLint p3,GLsizei p4,GLsizei p5,GLenum p6,GLenum p7,const GLvoid * p8){
  //on detruit le shm si il existe
  shmdt(shmSub2D);


  
  int fnum=OVERRIDE_BASE+4;
  int fflags=0;
  
  pthread_mutex_lock(mutexSub2D);
  shmSub2D=(GLvoid *)shmat( shmget(IPC_PRIVATE,p5*p6*sizeof(p7),0666|IPC_CREAT) ,0,0);
  memcpy(&shmSub2D[0],p8,p3*p4*sizeof(p7));//on copie la text dans le shm

  
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
  fifo_output(&cmd_fifo,&p2,4);
  fifo_output(&cmd_fifo,&p3,4);
  fifo_output(&cmd_fifo,&p4,4);
  fifo_output(&cmd_fifo,&p5,4);
  fifo_output(&cmd_fifo,&p6,4);
  fifo_output(&cmd_fifo,&p7,4);
  

  pthread_mutex_unlock(mutexSub2D);
}

void fglTexSubImage2D(){
 
  GLenum p0;
  GLint p1;
  GLint p2;
  GLint p3;
  GLsizei p4;
  GLsizei p5;
  GLenum p6;
  GLenum p7;
  const GLvoid *p8;

  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  fifo_input(&cmd_fifo,&p2,4);
  fifo_input(&cmd_fifo,&p3,4);
  fifo_input(&cmd_fifo,&p4,4);
  fifo_input(&cmd_fifo,&p5,4);
  fifo_input(&cmd_fifo,&p6,4);
  fifo_input(&cmd_fifo,&p7,4);



  pthread_mutex_lock(mutexSub2D);
  memcpy(p8,&shmSub2D[0],p3*p4*sizeof(p7));//on recupere les valeurs du shm
  pthread_mutex_unlock(mutexSub2D);
  //et on appelle la vrai fonction tex2D pour charger les etxtures en memeoire GPU


  lib_glTexSubImage2D ( p0 , p1 ,p2,p3,p4,p5,p6,p7,p8);//p8 etant recuperer par le shm



}


void glBitmap( GLsizei p0,GLsizei p1,GLfloat p2,GLfloat p3,GLfloat p4,GLfloat p5,const GLubyte * p6){

  //on detruit le shm si il existe
  shmdt(shmBitmap);


  
  int fnum=OVERRIDE_BASE+5;
  int fflags=0;
  
  pthread_mutex_lock(mutexBitmap);
  shmBitmap=(GLvoid *)shmat( shmget(IPC_PRIVATE,p0*p1,0666|IPC_CREAT) ,0,0);
  memcpy(&shmBitmap[0],p6,p0*p1);//on copie la text dans le shm

  
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
  fifo_output(&cmd_fifo,&p2,4);
  fifo_output(&cmd_fifo,&p3,4);
  fifo_output(&cmd_fifo,&p4,4);
  fifo_output(&cmd_fifo,&p5,4);
  

  pthread_mutex_unlock(mutexBitmap);

}

void fglBitmap(){

  GLsizei p0;
  GLsizei p1;
  GLfloat p2;
  GLfloat p3;
  GLfloat p4;
  GLfloat p5;
  const GLubyte *p6;

  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  fifo_input(&cmd_fifo,&p2,4);
  fifo_input(&cmd_fifo,&p3,4);
  fifo_input(&cmd_fifo,&p4,4);
  fifo_input(&cmd_fifo,&p5,4);



  pthread_mutex_lock(mutexBitmap);
  memcpy(p6,&shmBitmap[0],p0*p1);//on recupere les valeurs du shm
  pthread_mutex_unlock(mutexBitmap);
  //et on appelle la vrai fonction bitmap

  lib_glBitamp ( p0 , p1 ,p2,p3,p4,p5,p6);//p6 etant recuperer par le shm

}
