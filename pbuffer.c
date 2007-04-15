#include "pbuffer.h"

/*
 *Les pbuffer vont nous permettre d'enregistrer une image dasn un buffer plutot que de l'afficher 
 *nous allons nous servir de cela lorsque nous derivons la fonction swapBuffer
 *pour enregistrer le resultat des calcul des GPU
 *et l'envoyer au maitre qui va gerer l'affichage reel
 */

/* Some global vars */
static GLXFBConfig gFBconfig = 0;
static Display *gDpy = NULL;
static int gScreen = 0;
static GLXPbuffer gPBuffer = 0;


static void* lib_handle_libX = 0;
static void* lib_handle_libG = 0;
static void* lib_handle_libC = 0;

void * glomppoint;

/*fonction permettant de version courante de GLX*/
static int RuntimeQueryGLXVersion(Display *dpy, int screen)
{

  char* (*clientstring)(Display *, int);

  char *glxversion ;
  void * lib_strstr;


  //clientstring=lib_glXGetProcAddressARB("glXGetClientString");
 
  clientstring=dlsym(lib_handle_libG, "glXGetClientString");

  glxversion = clientstring(dpy, GLX_VERSION);
  
  lib_strstr=dlsym(lib_handle_libC, "strstr");
  
  if(  ! (  (((char* (*)(char*, char*))lib_strstr)(glxversion, "1.3"))  ||  (((char* (*)(char*, char*))lib_strstr)(glxversion, "1.4"))))
    { fprintf(stderr,"dansif\n"); fflush(stdout);return 0; }

  glomppoint=dlsym(lib_handle_libG, "glXQueryServerString");
  glxversion =  (char *)((char * (*)(Display *, int,int)) glomppoint)(dpy, screen, GLX_VERSION);
  
  if(  ! (  (((char* (*)(char*, char*))lib_strstr)(glxversion, "1.3"))  ||  (((char* (*)(char*, char*))lib_strstr)(glxversion, "1.4"))))
    return 0;
  
  return 1;
}


/*
 *Creer un pBuffer de taille width * height
 *la fonction retourne le pBuffer correctemnt cr	er (avec les bon attribut)
 *mais celui-ci est vide
 */
static GLXPbuffer MakePbuffer( Display *dpy, int screen, int width, int height )
{
	
  __GLXextFuncPtr glomppoint=(__GLXextFuncPtr)lib_glXGetProcAddressARB("glXChooseFBConfig");



  GLXFBConfig *fbConfigs;
  GLXFBConfig chosenFBConfig;
  GLXPbuffer pBuffer = None;

  int nConfigs;
  int fbconfigid;

  int fbAttribs[] = {
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DEPTH_SIZE, 1,
    GLX_DRAWABLE_TYPE, GLX_PIXMAP_BIT | GLX_PBUFFER_BIT,
    None
  };

  int pbAttribs[] = {
    GLX_PBUFFER_WIDTH, 0,
    GLX_PBUFFER_HEIGHT, 0,
    GLX_LARGEST_PBUFFER, False,
    GLX_PRESERVED_CONTENTS, False,
    None
  };

  pbAttribs[1] = width;
  pbAttribs[3] = height;



  fbConfigs = ((GLXFBConfig * (*)(Display *,int,int *,int *) )glomppoint)(dpy, screen, fbAttribs, &nConfigs);


  if (0 == nConfigs || !fbConfigs) {
    printf("Error: glxChooseFBConfig failed\n");
    XCloseDisplay(dpy);
    return 0;
  }

  chosenFBConfig = fbConfigs[0];
  
  glomppoint=dlsym(lib_handle_libG, "glXGetFBConfigAttrib");
  ((int (*)(Display *,GLXFBConfig,int,int *))glomppoint)(dpy, chosenFBConfig, GLX_FBCONFIG_ID, &fbconfigid);



  /* Create the pbuffer using first fbConfig in the list that works. */
  glomppoint=dlsym(lib_handle_libG, "glXCreatePbuffer");
  pBuffer = ((GLXPbuffer  (*)(Display *,GLXFBConfig,int *))glomppoint)(dpy, chosenFBConfig, pbAttribs);

  if (pBuffer) {
    gFBconfig = chosenFBConfig;
  }



  static int (*lib_Xfree) ( void *data )=0;
  lib_Xfree = dlsym(lib_handle_libX, "XFree");
  lib_Xfree(fbConfigs);



  return pBuffer;
}


/* creates a pbuffer */
int creerpbuffer(int width,int height)
{
  lib_handle_libX = dlopen("/usr/lib/libX11.so", RTLD_LAZY);
  lib_handle_libG = dlopen("/usr/lib/libGL.so", RTLD_LAZY);
  lib_handle_libC = dlopen("/lib/libc.so", RTLD_LAZY);  

  GLXContext glCtx;
  char envDisplay[16];
 

  sprintf(envDisplay,":0.%d",client_num);
  if (getenv("FORCE_GPU"))
	  sprintf(envDisplay,":0.0");



  Display* (*lib_XOpenDisplay)(char*);
  lib_XOpenDisplay=dlsym(lib_handle_libX, "XOpenDisplay");
  gDpy = lib_XOpenDisplay(envDisplay);
  if (!gDpy) {
    printf("Error: couldn't open default X display.\n");
    return 0;
  }

  gScreen = DefaultScreen(gDpy);



  /* Test that GLX is available */
   
  if (!RuntimeQueryGLXVersion(gDpy, gScreen)) {
    printf("Error: GLX 1.3 or 1.4 not available\n");
    XCloseDisplay(gDpy);
    return 0;
  }
	
  

  /* Create Pbuffer */
  gPBuffer = MakePbuffer( gDpy, gScreen, width, height );
  if (gPBuffer==None) {
    printf("Error: couldn't create pbuffer\n");
    XCloseDisplay(gDpy);
    return 0;
  }


  glomppoint=dlsym(lib_handle_libG, "glXCreateNewContext");
  glCtx =((GLXContext (*)(Display * ,GLXFBConfig ,int ,GLXContext,Bool) )glomppoint)(gDpy, gFBconfig, GLX_RGBA_TYPE, NULL, True );


  glomppoint=dlsym(lib_handle_libG, "glXIsDirect");

 

  if (glCtx) {
    if (!((Bool (*)(Display * ,GLXContext) )glomppoint)(gDpy,glCtx)) {
      printf("Warning: using indirect GLXContext\n");
    }
  }
  else {

    printf("Error: Couldn't create GLXContext\n");
    XCloseDisplay(gDpy);
    return 0;
  }



  glomppoint=dlsym(lib_handle_libG, "glXMakeCurrent");
  /* Bind context to pbuffer */
  if (!((Bool (*)(Display * ,GLXDrawable,GLXContext) )glomppoint)(gDpy,(GLXPixmap)gPBuffer,glCtx)) {
    printf("Error: glXMakeCurrent failed\n");
    XCloseDisplay(gDpy);
    return 0;
  }


  printf("Created pbuffer (%d %d)\n", height,width);

  return 1;

}







