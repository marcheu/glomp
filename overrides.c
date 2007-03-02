#include "overrides.h"
#include "init.h"
#include "id.h"
#include "config.h"

/* functions we implement ourselves */

static void (*lib_glXSwapBuffers)(Display *dpy, GLXDrawable drawable)=0;
static GLXWindow (*lib_glXCreateWindow) (Display *dpy, GLXFBConfig config,
                                 Window win, const int *attrib_list);

static void (*lib_glBindTexture) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenTextures) ( GLsizei p0 , GLuint *p1 )=0;	
static void (*lib_glFrustum) ( GLdouble left,
			       GLdouble right,
			       GLdouble bottom,
			       GLdouble top,
			       GLdouble zNear,
			       GLdouble zFar	)=0;

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

static GLuint (*lib_glGenLists)( GLsizei range )=0;
static void (*lib_glCallList) (GLuint list)=0;
static void (*lib_glCallLists) (GLsizei n, GLenum type, const GLvoid *lists)=0;
static void (*lib_glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)=0;
static void (*lib_glCopyTexImage1D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)=0;
static void (*lib_glCopyTexImage2D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)=0;
static void (*lib_glCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)=0;
static void (*lib_glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)=0;
static void (*lib_glFlush) (void);
static void (*lib_glBindTextureEXT) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenTexturesEXT) ( GLsizei p0 , GLuint *p1 )=0;


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
  lib_glGenList= dlsym(lib_handle_libGL, "glGenLists");
  lib_glCallList= dlsym(lib_handle_libGL, "glCallList");
  lib_glCallLists= dlsym(lib_handle_libGL, "glCallLists");
  lib_glCopyPixels= dlsym(lib_handle_libGL, "glCopyPixels");
  lib_glCopyTexImage1D= dlsym(lib_handle_libGL, "glCopyTexImage1D");
  lib_glCopyTexImage2D= dlsym(lib_handle_libGL, "glCopyTexImage2D");
  lib_glCopyTexSubImage1D= dlsym(lib_handle_libGL, "glCopyTexSubImage1D");
  lib_glCopyTexSubImage2D= dlsym(lib_handle_libGL, "glCopyTexSubImage2D");
  lib_glFlush= dlsym(lib_handle_libGL, "glFlush");

  /*les extensions*/
  lib_glBindTextureEXT = dlsym(lib_handle_libGL, "glBindTextureEXT");    
  lib_glGenTexturesEXT = dlsym(lib_handle_libGL, "glGenTexturesEXT");

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
	int fnum=OVERRIDE_BASE;
	int fflags=0;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_flush(&cmd_fifo);
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
	int fnum=OVERRIDE_BASE+1;
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
	GLdouble newp2;
	GLdouble newp3;
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
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}

	newp2=(p3-p2)*(double)beforeload/(double)totalload;
	newp3=(p3-p2)*(double)(beforeload+client_load[client_num])/(double)totalload;

	lib_glFrustum(p0,p1,newp2,newp3,p4,p5);
}


void glGenTextures ( GLsizei p0 , GLuint *p1 )
{
	int i;
	int fnum=OVERRIDE_BASE+2;
	int fflags=0;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_output(&cmd_fifo,&p0,4);

	for(i=0;i<p0;i++)
	{
		GLuint id=id_server_generate(id_texture);
		*p1=id;
		fifo_output(&cmd_fifo,&id,4);
		p1++;
	}

}

void fglGenTextures()
{
	int i;

	GLsizei p0;
	GLuint p1;
	fifo_input(&cmd_fifo,&p0,4);
	for(i=0;i<p0;i++)
	{
		GLuint local_id;
		fifo_input(&cmd_fifo,&p1,4);
		lib_glGenTextures ( p0 , &local_id );
		id_add(p1,local_id);
	}
}

GLboolean glIsTexture ( GLuint p0 )
{
	return id_server_test_type(p0,id_texture);
}

void glBindTexture ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+3;
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
  
  p1=id_translate(p1);
  
  lib_glBindTexture ( p0 , p1 );//on utilise le vrai bind texture
}

GLuint glGenLists ( GLsizei p0 )
{
	int i;
	int fnum=OVERRIDE_BASE+4;
	int fflags=0;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_output(&cmd_fifo,&p0,4);

	GLuint ret=0;
	for(i=0;i<p0;i++)
	{
		GLuint id=id_server_generate(id_list);
		if (i==0)
			ret=id;
		fifo_output(&cmd_fifo,&id,4);
	}

	return ret;
}

void fglGenLists()
{
	int i;

	GLsizei p0;
	GLuint p1;
	fifo_input(&cmd_fifo,&p0,4);
	for(i=0;i<p0;i++)
	{
		GLuint local_id;
		fifo_input(&cmd_fifo,&p1,4);
		local_id=lib_glGenLists (1);
		id_add(p1,local_id);
	}
}

GLboolean glIsList (GLuint p0)
{
	return id_server_test_type(p0,id_list);
}

void glCallList (GLuint p0)
{
	int fnum=OVERRIDE_BASE+5;
	int fflags=0;
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_output(&cmd_fifo,&p0,4);
}

void fglCallList ()
{
	GLuint p0;
	fifo_input(&cmd_fifo,&p0,4);
	lib_glCallList(id_translate(p0));
}

const GLubyte* glGetString( GLenum name )
{
	const GLubyte* vendor="Vendor";
	const GLubyte* renderer="Glomp " VERSION;
	const GLubyte* version="1.2 Glomp " VERSION;
	const GLubyte* extensions="";
	switch(name)
	{
		case GL_VENDOR:
			return vendor;
		case GL_RENDERER:
			return renderer;
		case GL_VERSION:
			return version;
		case GL_EXTENSIONS:
			return extensions;
	}
}

/*pas sur d'avoir la bonne taille dasn segment_create*/
void glCallLists (GLsizei n, GLenum type, const GLvoid *lists)
{

	int fnum=OVERRIDE_BASE+6;
	int fflags=0;
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_output(&cmd_fifo,&n,4);
	fifo_output(&cmd_fifo,&type,4);
	segment_create(lists,n);

}

void fglCallLists ()
{
	GLsizei p0;
	GLenum p1;
	GLvoid *p2;
	fifo_input(&cmd_fifo,&p0,4);
	fifo_input(&cmd_fifo,&p1,4);
	p2=(GLvoid *)segment_attach();
	lib_glCallList(p0,p1,p2);
}


/*vu qu'on souhaite juste retrouver les pixel du frame buffer, on ne l'envoie pas, mais on l'utilise sur le maitre*/
void glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
  lib_glCopyPixels (x,y,width,height,type);
}

/*apres toute la serie des copyTexImage qui marche comme copyPixel (cf man) donc on ne les envoie dans la fifo*/
/*vu que l'on copi direct du read_buffer, dispo chez le maiter, les derivees des fonctions on juste besoin de les rappeler sur le maitre (pas de fifo !)*/


void glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)
{
  lib_glCopyTexImage1D(target,level,internalFormat,x,y,width,border);
}
void glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
  lib_glCopyTexImage2D(target,level,internalFormat,x,y,width,heigth,border);
}
void glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
  lib_glCopyTexSubImage1D(target,level,xoffset,x,y,width);
}
void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
  lib_glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height );
}

/*fin des copyTex*/



/*glFlush à n'utiliser que sur le maitre ? -> donc pas de fifo*/
void glFlush(void){
  glFlush();
}







/*les fonction EXT de texturing*/
void glGenTexturesEXT ( GLsizei p0 , GLuint *p1 )
{
	int i;
	int fnum=OVERRIDE_BASE+7;
	int fflags=0;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
	fifo_output(&cmd_fifo,&p0,4);

	for(i=0;i<p0;i++)
	{
		GLuint id=id_server_generate(id_texture);
		*p1=id;
		fifo_output(&cmd_fifo,&id,4);
		p1++;
	}

}

void fglGenTexturesEXT()
{
	int i;

	GLsizei p0;
	GLuint p1;
	fifo_input(&cmd_fifo,&p0,4);
	for(i=0;i<p0;i++)
	{
		GLuint local_id;
		fifo_input(&cmd_fifo,&p1,4);
		lib_glGenTexturesEXT ( p0 , &local_id );
		id_add(p1,local_id);
	}
}

GLboolean glIsTextureEXT ( GLuint p0 )
{
	return id_server_test_type(p0,id_texture);
}

void glBindTextureEXT ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+8;
  int fflags=0;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
  return ;
  
}
void fglBindTextureEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindTextureEXT ( p0 , p1 );//on utilise le vrai bind texture
}

/*fin des fonction EXt de texturing*/
