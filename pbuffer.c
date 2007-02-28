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


/*fonction permettant de version courante de GLX*/
int RuntimeQueryGLXVersion(Display *dpy, int screen)
{
   char *glxversion;
 
   glxversion = (char *) glXGetClientString(dpy, GLX_VERSION);
   if (!(strstr(glxversion, "1.3") || strstr(glxversion, "1.4")))
      return 0;

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
GLXPbuffer MakePbuffer( Display *dpy, int screen, int width, int height )
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

   fbConfigs = glXChooseFBConfig(dpy, screen, fbAttribs, &nConfigs);

   if (0 == nConfigs || !fbConfigs) {
      printf("Error: glxChooseFBConfig failed\n");
      XCloseDisplay(dpy);
      return 0;
   }

   chosenFBConfig = fbConfigs[0];

   glXGetFBConfigAttrib(dpy, chosenFBConfig, GLX_FBCONFIG_ID, &fbconfigid);
   printf("Chose 0x%x as fbconfigid\n", fbconfigid);

   /* Create the pbuffer using first fbConfig in the list that works. */
   pBuffer = glXCreatePbuffer(dpy, chosenFBConfig, pbAttribs);

   if (pBuffer) {
      gFBconfig = chosenFBConfig;
   }

   XFree(fbConfigs);

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
  char * envDysplay;
  
  

  sprintf(envDysplay,"DISPLAY=0.%d",client_num);
  putenv(envDysplay);

  gDpy = XOpenDisplay(NULL);
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

   glCtx = glXCreateNewContext(gDpy, gFBconfig, GLX_RGBA_TYPE, NULL, True);
   if (glCtx) {
      if (!glXIsDirect(gDpy, glCtx)) {
         printf("Warning: using indirect GLXContext\n");
      }
   }
   else {
      printf("Error: Couldn't create GLXContext\n");
      XCloseDisplay(gDpy);
      return 0;
   }

  /* Bind context to pbuffer */
   if (!glXMakeCurrent(gDpy, gPBuffer, glCtx)) {
      printf("Error: glXMakeCurrent failed\n");
      XCloseDisplay(gDpy);
      return 0;
   }

   return 1;

}







