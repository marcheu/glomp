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

void (*function);



/*fonction permettant de version courante de GLX*/
/*debugger*/
static int RuntimeQueryGLXVersion(Display *dpy, int screen)
{
   printf("je plante!! \n");
   //void (*lib_glXGetClientString) (Display* p0,int p1 )=0;
     //void* lib_handle=0;
   //lib_handle = dlopen("/usr/lib/libGL.so", RTLD_LAZY);
   //lib_glXGetClientString= dlsym(lib_handle, "glXGetClientString");

   char *glxversion;
   /*glxversion = (char *)*/// lib_glXGetClientString(dpy, GLX_VERSION);
   //glxversion = glXGetClientString(dpy, GLX_VERSION);
   //if (!(strstr(glxversion, "1.3") || strstr(glxversion, "1.4"))) 
   //  return 0;
   
   
   printf("je plante \n");

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
   

   function=(__GLXextFuncPtr)glXGetProcAddressARB("glXChooseFBConfig");
   

   fbConfigs = /*glXChooseFBConfig*/((GLXFBConfig * (*)(Display *,int,int *,int *) )function)(dpy, screen, fbAttribs, &nConfigs);
   

   if (0 == nConfigs || !fbConfigs) {
      printf("Error: glxChooseFBConfig failed\n");
      XCloseDisplay(dpy);
      return 0;
   }


   chosenFBConfig = fbConfigs[0];

   //printf("je passe: %d\n",fbConfigs);
   
   function=(__GLXextFuncPtr)glXGetProcAddressARB("glXGetFBConfigAttrib");


   /*glXGetFBConfigAttrib*/
   ((int (*)(Display *,GLXFBConfig,int,int *))function)(dpy, chosenFBConfig, GLX_FBCONFIG_ID, &fbconfigid);
   printf("Chose 0x%x as fbconfigid\n", fbconfigid);




   function=(__GLXextFuncPtr)glXGetProcAddressARB("glXCreatePbuffer");
   /* Create the pbuffer using first fbConfig in the list that works. */

   pBuffer = ((GLXPbuffer  (*)(Display *,GLXFBConfig,int *))function)(dpy, chosenFBConfig, pbAttribs);
   //pBuffer = glXCreatePbuffer(dpy, chosenFBConfig, pbAttribs);

   

   if (pBuffer) {
      gFBconfig = chosenFBConfig;
   }

   //xfree bloque ...
   //XFree(fbConfigs);
   //printf("hoho\n");
   
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
  /*on s'en fout ... fallait avoir un GLX  a jour ... on est plus a la prehistoire*/
  /* Test that GLX is available */
  /* if (!RuntimeQueryGLXVersion(gDpy, gScreen)) {
     printf("Error: GLX 1.3 or 1.4 not available\n");
     XCloseDisplay(gDpy);
     return 0;
     }*/



  /* Create Pbuffer */
   gPBuffer = MakePbuffer( gDpy, gScreen, width, height );
  
   if (gPBuffer==None) {
      printf("Error: couldn't create pbuffer\n");
      XCloseDisplay(gDpy);
      return 0;
   }

  function=(__GLXextFuncPtr)glXGetProcAddressARB("glXCreateNewContext"); 
  glCtx =((GLXContext (*)(Display * ,GLXFBConfig ,int ,GLXContext,Bool) )function)(gDpy, gFBconfig, GLX_RGBA_TYPE, NULL, True );
  //glCtx = glXCreateNewContext(gDpy, gFBconfig, GLX_RGBA_TYPE, NULL, True);
  
  


  function=(__GLXextFuncPtr)glXGetProcAddressARB("glXIsDirect");
 
   if (glCtx) {
     if (!/*glXIsDirect(gDpy, glCtx)*/  ((Bool (*)(Display * ,GLXContext) )function)(gDpy,glCtx)) {
         printf("Warning: using indirect GLXContext\n");
      }
   }
   else {
      printf("Error: Couldn't create GLXContext\n");
      XCloseDisplay(gDpy);
      return 0;
   }

  function=(__GLXextFuncPtr)glXGetProcAddressARB("glXMakeCurrent");
  
  /* Bind context to pbuffer */
  if (!/*glXMakeCurrent(gDpy, gPBuffer, glCtx)*/((Bool (*)(Display * ,GLXDrawable,GLXContext) )function)(gDpy,(GLXPixmap)gPBuffer,glCtx)) {
      printf("Error: glXMakeCurrent failed\n");
      XCloseDisplay(gDpy);
      return 0;
   }

  printf("FIN DE CREERPBUFFER\n");

  return 1;

}
/*ce fichier est debugger !!!*/
/*en fait, dans ces grandes lignes*/
/*il est possible que les fonction appellé en cas d'erreur plante*/
/*par exemple ligne 191  XCloseDisplay(gDpy)*/
/*a mon avis ne marche pas trop ...*/






