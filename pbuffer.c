/*
 *   GLOMP - transparent multipipe OpenGL
 *   Copyright (C) 2007 the GLOMP team (see AUTHORS)
 *   
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "pbuffer.h"
#include "lib_funcs.h"

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
static int RuntimeQueryGLXVersion(Display *dpy, int screen)
{
	char *glxversion ;

/*	glxversion = lib_glXGetClientString(dpy, GLX_VERSION);
	if(  ! (  lib_strstr(glxversion, "1.3"))  ||  (lib_strstr(glxversion, "1.4")))
		return 0;

	glxversion =  lib_glXQueryServerString(dpy, screen, GLX_VERSION);

	if(  ! (  lib_strstr(glxversion, "1.3"))  ||  (lib_strstr(glxversion, "1.4")))
		return 0;*/

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

	fbConfigs = lib_glXChooseFBConfig(dpy, screen, fbAttribs, &nConfigs);

	if (0 == nConfigs || !fbConfigs) {
		printf("Error: glxChooseFBConfig failed\n");
		XCloseDisplay(dpy);
		return 0;
	}

	chosenFBConfig = fbConfigs[0];

	lib_glXGetFBConfigAttrib(dpy, chosenFBConfig, GLX_FBCONFIG_ID, &fbconfigid);

	/* Create the pbuffer using first fbConfig in the list that works. */
	pBuffer = lib_glXCreatePbuffer(dpy, chosenFBConfig, pbAttribs);

	if (pBuffer) {
		gFBconfig = chosenFBConfig;
	}

	lib_XFree(fbConfigs);

	return pBuffer;
}


/* creates a pbuffer */
int pbuffer_init(int width,int height)
{
	GLXContext glCtx;
	char envDisplay[16];

	sprintf(envDisplay,":0.%d",client_num);
	if (getenv("FORCE_GPU"))
		sprintf(envDisplay,":0.0");

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

	glCtx = lib_glXCreateNewContext(gDpy, gFBconfig, GLX_RGBA_TYPE, NULL, True );

	if (glCtx) {
		if (!(lib_glXIsDirect(gDpy,glCtx))) {
			printf("Warning: using indirect GLXContext\n");
		}
	}
	else {

		printf("Error: Couldn't create GLXContext\n");
		XCloseDisplay(gDpy);
		return 0;
	}

	/* Bind context to pbuffer */
	if (!(lib_glXMakeCurrent(gDpy,(GLXPixmap)gPBuffer,glCtx))) {
		printf("Error: glXMakeCurrent failed\n");
		XCloseDisplay(gDpy);
		return 0;
	}

	printf("Created pbuffer (%d %d)\n", height,width);

	return 1;

}


