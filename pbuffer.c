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

void * glomppoint;

/*fonction permettant de version courante de GLX*/
static int RuntimeQueryGLXVersion(Display *dpy, int screen)
{



   char *glxversion;
	
printf(" !!!!!\n");
   glomppoint=(void *)lib_glXGetProcAddressARB("glXGetClientString");

	printf("getprocaddr !!!!!\n");

   glxversion = (char *)((char * (*)(Display *, int))glomppoint)(dpy, GLX_VERSION);
   if (!(strstr(glxversion, "1.3") || strstr(glxversion, "1.4")))
      { printf("dansif\n"); return 0; }

   printf("Ca PASSE\n");

   glxversion = (char *) glXQueryServerString(dpy, screen, GLX_VERSION);
   if (!(strstr(glxversion, "1.3") || strstr(glxversion, "1.4")))
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
	glomppoint=(__GLXextFuncPtr)lib_glXGetProcAddressARB("glXGetFBConfigAttrib");
((int (*)(Display *,GLXFBConfig,int,int *))glomppoint)(dpy, chosenFBConfig, GLX_FBCONFIG_ID, &fbconfigid);
   printf("Chose 0x%x as fbconfigid\n", fbconfigid);



   /* Create the pbuffer using first fbConfig in the list that works. */
glomppoint=(__GLXextFuncPtr)lib_glXGetProcAddressARB("glXCreatePbuffer");
pBuffer = ((GLXPbuffer  (*)(Display *,GLXFBConfig,int *))glomppoint)(dpy, chosenFBConfig, pbAttribs);

   if (pBuffer) {
      gFBconfig = chosenFBConfig;
   }



static int (*lib_Xfree) ( void *data )=0;
  lib_handle_libX = dlopen("/usr/lib/libX11.so", RTLD_LAZY);
  lib_Xfree = dlsym(lib_handle_libX, "XFree");
   lib_Xfree(fbConfigs);

   return pBuffer;
}


/*
*La fonction creerpbuffer va creer un pbuffer vide avec makePBuffer
*celui-ci va etre renplit par ce que la GPU a calculer (ici 1/4 de l'ecran)
*avant d'etre renvoyer (evec transfertFentre) au maitre
*/

int creerpbuffer(int width,int height)
{

  GLXContext glCtx;
  char * envDisplay;
  
  sprintf(envDisplay,":0.%d",client_num);


  gDpy = XOpenDisplay(envDisplay);
  if (!gDpy) {
      printf("Error: couldn't open default X display.\n");
      return 0;
  }


  gScreen = DefaultScreen(gDpy);



  /* Test that GLX is available */
/*
   if (!RuntimeQueryGLXVersion(gDpy, gScreen)) {
     printf("Error: GLX 1.3 or 1.4 not available\n");
     XCloseDisplay(gDpy);
     return 0;
   }
	
*/

  /* Create Pbuffer */
   gPBuffer = MakePbuffer( gDpy, gScreen, width, height );
   if (gPBuffer==None) {
      printf("Error: couldn't create pbuffer\n");
      XCloseDisplay(gDpy);
      return 0;
   }


	glomppoint=(__GLXextFuncPtr)lib_glXGetProcAddressARB("glXCreateNewContext");
glCtx =((GLXContext (*)(Display * ,GLXFBConfig ,int ,GLXContext,Bool) )glomppoint)(gDpy, gFBconfig, GLX_RGBA_TYPE, NULL, True );



	glomppoint=(__GLXextFuncPtr)lib_glXGetProcAddressARB("glXIsDirect");

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




	glomppoint=(__GLXextFuncPtr)lib_glXGetProcAddressARB("glXMakeCurrent");
  /* Bind context to pbuffer */
   if (!((Bool (*)(Display * ,GLXDrawable,GLXContext) )glomppoint)(gDpy,(GLXPixmap)gPBuffer,glCtx)) {
      printf("Error: glXMakeCurrent failed\n");
      XCloseDisplay(gDpy);
      return 0;
   }




   return 1;

}







