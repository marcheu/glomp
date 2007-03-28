#include"overrides.h"

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
static void (*lib_glOrtho) ( GLdouble left,
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
static void (*lib_glBindTextureEXT) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenTexturesEXT) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glGenQueries) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glBindBuffer) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenBuffers) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glBindProgramARB) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenProgramsARB) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glBindBufferARB) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenBuffersARB) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glGenQueriesARB) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glGenFencesNV) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glBindProgramNV) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenProgramsNV) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glGenOcclusionQueriesNV) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glBindRenderbufferEXT) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenRenderbuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glBindFramebufferEXT) ( GLenum p0 , GLuint p1 )=0;
static void (*lib_glGenFramebuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteTextures) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glFlush) ()=0;
static void (*lib_glFinish) ()=0;
static void (*lib_glDeleteQueries) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteBuffers) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteProgramsARB) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteBuffersARB) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteQueriesARB) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteTexturesEXT) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteFencesNV) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteProgramsNV) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteOcclusionQueriesNV) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteRenderbuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glDeleteFramebuffersEXT) ( GLsizei p0 , GLuint *p1 )=0;
static void (*lib_glGetIntegerv)(GLenum p0,GLint *p1)=0;
static void (*lib_glViewport)( GLint x,GLint y,GLsizei width,GLsizei height)=0;


void (*lib_glRasterPos2i)( GLint x,GLint y)=0;
void (*lib_glRasterPos2f)( GLfloat x,GLfloat y)=0;
void (*lib_glDrawPixels)( GLsizei width,
			  GLsizei height,
			  GLenum format,
			  GLenum type,
			  const GLvoid *pixels );
void (*lib_glReadPixels)( GLint x,
			  GLint y,
			  GLsizei width,
			  GLsizei height,
			  GLenum format,
			  GLenum type,
			  GLvoid *pixels );



static Window (*lib_XCreateWindow)(Display *display, Window parent, int x, int y,
				   unsigned int width, unsigned int height,
				   unsigned int border_width, int depth,
				   unsigned int class, Visual *visual,
				   unsigned long valuemask, XSetWindowAttributes *attributes)=0;
static int (*lib_XDestroyWindow)( Display * disp, Window parent)=0;
static int (*lib_XCloseDisplay)(Display * dis)=0;

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
  lib_glOrtho = dlsym(lib_handle_libGL, "glOrtho"); 
  lib_glGenLists= dlsym(lib_handle_libGL, "glGenLists");
  lib_glCallList= dlsym(lib_handle_libGL, "glCallList");
  lib_glCallLists= dlsym(lib_handle_libGL, "glCallLists");
  lib_glCopyPixels= dlsym(lib_handle_libGL, "glCopyPixels");
  lib_glCopyTexImage1D= dlsym(lib_handle_libGL, "glCopyTexImage1D");
  lib_glCopyTexImage2D= dlsym(lib_handle_libGL, "glCopyTexImage2D");
  lib_glCopyTexSubImage1D= dlsym(lib_handle_libGL, "glCopyTexSubImage1D");
  lib_glCopyTexSubImage2D= dlsym(lib_handle_libGL, "glCopyTexSubImage2D");
  lib_glDeleteTextures= dlsym(lib_handle_libGL, "glDeleteTextures");
  lib_glFlush= dlsym(lib_handle_libGL, "glFlush");
  lib_glFinish= dlsym(lib_handle_libGL, "glFinish");
  lib_glGetIntegerv=dlsym(lib_handle_libGL, "glGetIntegerv");
  lib_glViewport=dlsym(lib_handle_libGL, "glViewport");
  lib_glRasterPos2f = dlsym(lib_handle_libGL, "glRasterPos2f");
  lib_glRasterPos2i = dlsym(lib_handle_libGL, "glRasterPos2i");
  lib_glDrawPixels = dlsym(lib_handle_libGL, "glDrawPixels");
  lib_glReadPixels = dlsym(lib_handle_libGL, "glReadPixels");  

  /*les extensions*/
  lib_glBindTextureEXT = dlsym(lib_handle_libGL, "glBindTextureEXT");    
  lib_glGenTexturesEXT = dlsym(lib_handle_libGL, "glGenTexturesEXT");
  lib_glGenQueries = dlsym(lib_handle_libGL, "glGenQueries"); 
  lib_glBindBuffer = dlsym(lib_handle_libGL, "glBindBuffer");    
  lib_glGenBuffers = dlsym(lib_handle_libGL, "glGenBuffers");
  lib_glBindProgramARB = dlsym(lib_handle_libGL, "glBindProgramARB");    
  lib_glGenProgramsARB = dlsym(lib_handle_libGL, "glGenProgramsARB");
  lib_glBindBufferARB = dlsym(lib_handle_libGL, "glBindBufferARB");    
  lib_glGenBuffersARB = dlsym(lib_handle_libGL, "glGenBuffersARB");
  lib_glGenQueriesARB = dlsym(lib_handle_libGL, "glGenQueriesARB"); 
  lib_glGenFencesNV = dlsym(lib_handle_libGL, "glGenFencesARB"); 
  lib_glBindProgramNV = dlsym(lib_handle_libGL, "glBindProgramNV");    
  lib_glGenProgramsNV = dlsym(lib_handle_libGL, "glGenProgramsNV");
  lib_glGenOcclusionQueriesNV = dlsym(lib_handle_libGL, "glGenOcclusionQueriesNV");
  lib_glBindRenderbufferEXT = dlsym(lib_handle_libGL, "glBindRenderbufferEXT"); 
  lib_glGenRenderbuffersEXT = dlsym(lib_handle_libGL, "glGenRenderbuffersEXT");
  lib_glBindFramebufferEXT = dlsym(lib_handle_libGL, "glBindFramebufferEXT"); 
  lib_glGenFramebuffersEXT = dlsym(lib_handle_libGL, "glGenFramebuffersEXT");
  lib_glDeleteQueries= dlsym(lib_handle_libGL, "glDeleteQueries");
  lib_glDeleteBuffers= dlsym(lib_handle_libGL, "glDeleteBuffers");
  lib_glDeleteProgramsARB = dlsym(lib_handle_libGL, "glDeleteProgramsARB");
  lib_glDeleteBuffersARB= dlsym(lib_handle_libGL, "glDeleteBuffersARB");
  lib_glDeleteQueriesARB= dlsym(lib_handle_libGL, "glDeleteQueriesARB");
  lib_glDeleteTexturesEXT= dlsym(lib_handle_libGL, "glDeleteTexturesEXT");
  lib_glDeleteFencesNV= dlsym(lib_handle_libGL, "glDeleteFencesNV");
  lib_glDeleteProgramsNV = dlsym(lib_handle_libGL, "glDeleteProgramsNV");
  lib_glDeleteOcclusionQueriesNV = dlsym(lib_handle_libGL, "glDeleteOcclusionQueriesNV");
  lib_glDeleteRenderbuffersEXT = dlsym(lib_handle_libGL, "glDeleteRenderbuffersEXT");
  lib_glDeleteFramebuffersEXT = dlsym(lib_handle_libGL, "glDeleteFramebuffersEXT");

  /* intercept XSetStandardProperties */
  lib_handle_libX11 = dlopen("/usr/lib/libX11.so", RTLD_LAZY);

  if (!lib_handle_libX11){perror("lib");exit(0);}

  lib_XSetStandardProperties = dlsym(lib_handle_libX11, "XSetStandardProperties");
  lib_XCreateWindow = dlsym(lib_handle_libX11, "XCreateWindow");
  lib_XDestroyWindow= dlsym(lib_handle_libX11, "XDestroyWindow");
  lib_XCloseDisplay = dlsym(lib_handle_libX11, "XCloseDisplay");
}



/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)
{
  int fnum=OVERRIDE_BASE;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_flush(&GLOMPcmd_fifo);
  ecrire_fenetre();//si on est dans le maitre, on recupere les buffers
  lib_glXSwapBuffers(dpy, drawable);//et on utilise la vrai fonction swapbuffer


}

void GLOMPglXSwapBuffers()
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
  /*on la relance betement*/
  return lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}
extern GLXWindow XCreateWindow(Display *display, Window parent, int x, int y,
			       unsigned int width2, unsigned int height2, unsigned int border_width, int depth, unsigned int class, Visual *visual,
			       unsigned long valuemask, XSetWindowAttributes *attribute)
{  
  int fnum=OVERRIDE_BASE+39;
  int fflags=0;

  width=width2;
  height=height2;
  createAllFen();

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&width,sizeof(width));
  fifo_output(&GLOMPcmd_fifo,&height,sizeof(height));


  return lib_XCreateWindow(display, parent, x, y,width2, height2, border_width,depth,class,visual,valuemask, attribute);
}

void GLOMPXCreateWindow()
{
  int i;


  fifo_input(&GLOMPcmd_fifo,&width,4);
  fifo_input(&GLOMPcmd_fifo,&height,4);

  createAllFen();


  if (!creerpbuffer(width,height)) {
    printf("Error:couldn't create pbuffer %s %s\n",width,height);
    exit(0);
  }



}




/*bugged! please use XCreateWindows*/
extern GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, const int *attrib_list)
{
  return lib_glXCreateWindow(dpy,config,win,attrib_list);
}


void glFrustum ( GLdouble p0 , GLdouble p1 , GLdouble p2 , GLdouble p3 , GLdouble p4 , GLdouble p5 )
{
  int fnum=OVERRIDE_BASE+1;
  int fflags=0;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,8);
  fifo_output(&GLOMPcmd_fifo,&p1,8);
  fifo_output(&GLOMPcmd_fifo,&p2,8);
  fifo_output(&GLOMPcmd_fifo,&p3,8);
  fifo_output(&GLOMPcmd_fifo,&p4,8);
  fifo_output(&GLOMPcmd_fifo,&p5,8);
}


/*on a besoin de gerer la perspective donc on interceptent les info de glfrumstun*/
void GLOMPglFrustum()
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
  fifo_input(&GLOMPcmd_fifo,&p0,8);
  fifo_input(&GLOMPcmd_fifo,&p1,8);
  fifo_input(&GLOMPcmd_fifo,&p2,8);
  fifo_input(&GLOMPcmd_fifo,&p3,8);
  fifo_input(&GLOMPcmd_fifo,&p4,8);
  fifo_input(&GLOMPcmd_fifo,&p5,8);

  int totalload=0;
  int beforeload=0;
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
      if (i<client_num)
	beforeload+=client_load[i];
    }


  newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
  newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);
  printf("[%d] les anciennes %f %f --- les nexs %f %f\n",client_num,p2,p3,newp2,newp3);
 
  lib_glFrustum(p0,p1,newp2,newp3,p4,p5);
}


void glGenTextures ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+2;
  int fflags=0;
  GLuint id;
  printf("je genere une texture %d\n",id);
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_texture);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenTextures()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenTextures ( 1 , &id );
      id_add(p1,id);
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  printf("id du server:%d\n",p1);
  return ;

}

void GLOMPglBindTexture()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);

  p1=id_translate(p1);

  lib_glBindTexture ( p0 , p1 );//on utilise le vrai bind texture
}

GLuint glGenLists ( GLsizei p0 )
{
  int i;
  int fnum=OVERRIDE_BASE+4;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  GLuint ret=0;
  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_list);
      if (i==0)
	ret=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
    }

  return ret;


}

void GLOMPglGenLists()
{

  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;



  fifo_input(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {

      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=lib_glGenLists(1);
      id_add(p1,id);
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
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
}

void GLOMPglCallList ()
{
  GLuint p0;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  lib_glCallList(id_translate(p0));
}

const GLubyte* glGetString( GLenum name )
{
  const GLubyte* vendor="Vendor";
  const GLubyte* renderer="Glomp ";
  const GLubyte* version="1.2 Glomp " ;
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
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&n,4);
  fifo_output(&GLOMPcmd_fifo,&type,4);
  segment_create(lists,n);

}

void GLOMPglCallLists ()
{
  GLsizei p0;
  GLenum p1;
  GLvoid *p2;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  p2=(GLvoid*)(segment_attach());
  lib_glCallLists(p0,p1,p2);
  segment_delete();
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
  lib_glCopyTexImage2D(target,level,internalFormat,x,y,width,height,border);
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



/*les fonction EXT de texturing*/
void glGenTexturesEXT ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+7;
  int fflags=0;
  GLuint id;
  
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_texture);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenTexturesEXT()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenTexturesEXT ( p0 , &id );
      id_add(p1,id);
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindTextureEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindTextureEXT ( p0 , p1 );//on utilise le vrai bind texture
}

/*fin des fonction EXt de texturing*/

void glGenQueries ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+9;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenQueries()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenQueries ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsQuery ( GLuint p0 )
{
  return id_server_test_type(p0,id_occlusion_query);
}

/*debut des buffer de glext*/
void glGenBuffers ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+10;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenBuffers()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenBuffers ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsBuffer ( GLuint p0 )
{
  return id_server_test_type(p0,id_buffer);
}

void glBindBuffer ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+11;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindBuffer()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindBuffer ( p0 , p1 );//on utilise le vrai bind buffer
}
/*fin des buffer*/

/*debut des ProgramARB*/
void glGenProgramsARB ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+12;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_program);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenProgramsARB()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenProgramsARB ( p0 , &id );
      id_add(p1,id);
    }
}

void glBindProgramARB ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+13;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindProgramARB()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindProgramARB ( p0 , p1 );//on utilise le vrai bind program
}

GLboolean glIsProgramARB ( GLuint p0 )
{
  return id_server_test_type(p0,id_program);
}

/*fin des ProgramARB*/

/*debut des bufferARB de glext*/
void glGenBuffersARB ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+14;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenBuffersARB()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenBuffersARB ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsBufferARB ( GLuint p0 )
{
  return id_server_test_type(p0,id_buffer);
}

void glBindBufferARB ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+15;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindBufferARB()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindBufferARB ( p0 , p1 );//on utilise le vrai bind bufferARB
}
/*fin des bufferARB*/

void glGenQueriesARB ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+16;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenQueriesARB()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenQueriesARB ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsQueryARB ( GLuint p0 )
{
  return id_server_test_type(p0,id_occlusion_query);
}

/*fonction fenceNV*/

void glGenFencesNV ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+17;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_fence);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenFencesNV()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenFencesNV ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsFenceNV ( GLuint p0 )
{
  return id_server_test_type(p0,id_fence);
}



/*fin de fenceNV*/

/*programNV*/
void glGenProgramsNV ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+18;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_program);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenProgramsNV()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenProgramsNV ( p0 , &id );
      id_add(p1,id);
    }
}

void glBindProgramNV ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+19;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindProgramNV()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindProgramNV ( p0 , p1 );//on utilise le vrai bind program
}


GLboolean glIsProgramNV ( GLuint p0 )
{
  return id_server_test_type(p0,id_program);
}
/*fin programNV*/

/*NV occlusion query*/

void glGenOcclusionQueriesNV ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+20;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenOcclusionQueriesNV()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenOcclusionQueriesNV ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsOcclusionQueryNV ( GLuint p0 )
{
  return id_server_test_type(p0,id_occlusion_query);
}

/*fin des requete d occlusion NV*/



/*debut des bufferEXT*/
void glGenRenderbuffersEXT ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+21;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenRenderbuffersEXT()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenRenderbuffersEXT ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsRenderbufferEXT ( GLuint p0 )
{
  return id_server_test_type(p0,id_buffer);
}

void glBindRenderbufferEXT ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+22;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindRenderbufferEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindRenderbufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
}


void glGenFramebuffersEXT ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+23;
  int fflags=0;
  GLuint id;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      p1++;
    }

}

void GLOMPglGenFramebuffersEXT()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      lib_glGenFramebuffersEXT ( p0 , &id );
      id_add(p1,id);
    }
}

GLboolean glIsFramebufferEXT ( GLuint p0 )
{
  return id_server_test_type(p0,id_buffer);
}

void glBindFramebufferEXT ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+24;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  return ;
  
}
void GLOMPglBindFramebufferEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindFramebufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
}

/*fin des bufferEXT*/



/*glFlush à n'utiliser que sur le maitre ? -> donc pas de fifo*/
void glFlush(void){
  int fnum=OVERRIDE_BASE+25;
  int fflags=0;
  lib_glFlush();
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
}

void GLOMPglFlush()
{
  lib_glFlush();
}

/*glFlush à n'utiliser que sur le maitre ? -> donc pas de fifo*/
void glFinish(void){
  int fnum=OVERRIDE_BASE+26;
  int fflags=0;
  fifo_flush(&GLOMPcmd_fifo);
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
}

void GLOMPglFinish()
{
  lib_glFinish();
}


/*les fonctions delete*/
void glDeleteTextures (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+27;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteTextures()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteTextures ( p0 , &id);
		
    }
}
/*deleteQuery*/
void glDeleteQueries (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+28;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteQueries()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteQueries ( p0 , &id);
		
    }
}
void glDeleteBuffers (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+29;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteBuffers()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteBuffers ( p0 , &id);
		
    }
}
void glDeleteProgramsARB (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+30;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteProgramsARB()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteProgramsARB ( p0 , &id);
		
    }
}


void glDeleteBuffersARB (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+31;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteBuffersARB()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteBuffersARB ( p0 , &id);
		
    }
}


void glDeleteQueriesARB (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+32;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteQueriesARB()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteQueriesARB ( p0 , &id);
		
    }
}


void glDeleteTexturesEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+33;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteTexturesEXT()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteTexturesEXT ( p0 , &id);
		
    }
}

void glDeleteFencesNV (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+34;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteFencesNV()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteFencesNV ( p0 , &id);
		
    }
}

void glDeleteProgramsNV (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+35;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteProgramsNV()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteProgramsNV ( p0 , &id);
		
    }
}


void glDeleteOcclusionQueriesNV (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+36;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteOcclusionQueriesNV()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteOcclusionQueriesNV ( p0 , &id);
		
    }
}



void glDeleteRenderbuffersEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+37;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteRenderbuffersEXT()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteRenderbuffersEXT ( p0 , &id);
		
    }
}


void glDeleteFramebuffersEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+38;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      p1++;
		
    }  
}
void GLOMPglDeleteFramebuffersEXT()
{
  int i;
  GLuint id;
  GLsizei p0;
  GLuint p1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteFramebuffersEXT ( p0 , &id);
		
    }
}

void glViewport ( GLint x,GLint y,GLsizei w,GLsizei h )
{
  int i;
  int fnum=OVERRIDE_BASE+40;
  int fflags=0;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&x,4);
  fifo_output(&GLOMPcmd_fifo,&y,4);
  fifo_output(&GLOMPcmd_fifo,&w,4);
  fifo_output(&GLOMPcmd_fifo,&h,4);
}

void GLOMPglViewport()
{
  int i;
  
  GLsizei p2,p3,newp3;
  GLint p0,p1,newp1;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  fifo_input(&GLOMPcmd_fifo,&p2,4);
  fifo_input(&GLOMPcmd_fifo,&p3,4);

  int totalload=0;
  int beforeload=0;
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
      if (i<client_num)
	beforeload+=client_load[i];
    }
  newp1=p1+p3*((double)beforeload/(double)totalload);

  newp3=p3*(double)client_load[client_num]/(double)totalload;

  
  printf("viewport:%d    %d %d %d %d",client_num,p1,p3,newp1,newp3);

  /*
    newp2=(p3-p2)*((double)beforeload/(double)totalload-0.5);
    newp3=(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload-0.5);
  */

  
  lib_glViewport(p0,newp1,p2,newp3);
 
    
}

void glOrtho ( GLdouble p0, GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5 )
{

  int fnum=OVERRIDE_BASE+41;
  int fflags=0;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&fflags,sizeof(fflags));
  fifo_output(&GLOMPcmd_fifo,&p0,8);
  fifo_output(&GLOMPcmd_fifo,&p1,8);
  fifo_output(&GLOMPcmd_fifo,&p2,8);
  fifo_output(&GLOMPcmd_fifo,&p3,8);
  fifo_output(&GLOMPcmd_fifo,&p4,8);
  fifo_output(&GLOMPcmd_fifo,&p5,8);

}

void GLOMPglOrtho()
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
  fifo_input(&GLOMPcmd_fifo,&p0,8);
  fifo_input(&GLOMPcmd_fifo,&p1,8);
  fifo_input(&GLOMPcmd_fifo,&p2,8);
  fifo_input(&GLOMPcmd_fifo,&p3,8);
  fifo_input(&GLOMPcmd_fifo,&p4,8);
  fifo_input(&GLOMPcmd_fifo,&p5,8);
  
  int totalload=0;
  int beforeload=0;
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
      if (i<client_num)
	beforeload+=client_load[i];
    }
  
  
  printf("%f %f\n",p2,p3);
  
  newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
  newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);

  printf("new %f %f\n",newp2,newp3);
  //  newp2=p2;
  //newp3=p3;
  

  lib_glOrtho(p0,p1,newp2,newp3,p4,p5);
 
    
}

int XDestroyWindow ( Display * disp, Window parent)

{
  lib_XDestroyWindow(disp,parent);
  kill(0,SIGKILL);
  
}

int XCloseDisplay ( Display * disp) 
{
  lib_XCloseDisplay(disp);
  kill(0,SIGKILL);  
}

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
   GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;

   
   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}


