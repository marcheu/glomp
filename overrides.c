#define _GNU_SOURCE
#include <string.h>
#include "overrides.h"
#ifdef ENABLE_SINGLE_SCREEN
#include "single_screen.h"
#else
#include "multi_screen.h"
#endif

/* functions we implement ourselves */




/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)
{
	int fnum=OVERRIDE_BASE;

	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_flush(&GLOMPcmd_fifo);
#ifdef ENABLE_SINGLE_SCREEN
	GLOMP_single_screen_swap(dpy,drawable);
#else
	GLOMP_multi_screen_swap(dpy,drawable);
#endif
}

void GLOMPglXSwapBuffers()
{ 
#ifdef ENABLE_SINGLE_SCREEN
	GLOMP_single_screen_swap(NULL,0);
#else
	GLOMP_multi_screen_swap(NULL,0);
#endif
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
	printf("XSetStandardProperties called\n");
	return lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}

GLXWindow XCreateWindow(Display *display, Window parent, int x, int y,
			       unsigned int width2, unsigned int height2, unsigned int border_width, int depth, unsigned int class, Visual *visual,
			       unsigned long valuemask, XSetWindowAttributes *attribute)
{  
	int fnum=OVERRIDE_BASE+39;

	width=width2;
	height=height2;
	printf("XCreateWindow %d %d\n",width,height);
#ifdef ENABLE_SINGLE_SCREEN
	GLOMP_single_screen_init_window();
#else
	GLOMP_multi_screen_init_window();
#endif

	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&width,sizeof(width));
	fifo_output(&GLOMPcmd_fifo,&height,sizeof(height));

	return lib_XCreateWindow(display, parent, x, y,width, height, border_width,depth,class,visual,valuemask, attribute);
}

void GLOMPXCreateWindow()
{
	fifo_input(&GLOMPcmd_fifo,&width,4);
	fifo_input(&GLOMPcmd_fifo,&height,4);

#ifdef ENABLE_SINGLE_SCREEN
	GLOMP_single_screen_init_window();
#else
	GLOMP_multi_screen_init_window();
#endif
}

GLXContext glXCreateNewContext(Display *dpy, GLXFBConfig config,
                                      int render_type, GLXContext share_list,
                                      Bool direct)
{
	printf("glXCreateNewContext called !!!!!!!\n");
	return lib_glXCreateNewContext(dpy,config,render_type,share_list,direct);
}


/*bugged! please use XCreateWindows*/
GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, const int *attrib_list)
{
	printf("glXCreateWindow called !!!!!!!\n");
	return lib_glXCreateWindow(dpy,config,win,attrib_list);
}


GLXContext glXCreateContext(Display *dpy, XVisualInfo *vis,
                                   GLXContext share_list, Bool direct)
{
	printf("glXCreateContext called !!!!!!!\n");
	return lib_glXCreateContext(dpy,vis,share_list,direct);
}


void glFrustum ( GLdouble p0 , GLdouble p1 , GLdouble p2 , GLdouble p3 , GLdouble p4 , GLdouble p5 )
{
	int fnum=OVERRIDE_BASE+1;
	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&p0,8);
	fifo_output(&GLOMPcmd_fifo,&p1,8);
	fifo_output(&GLOMPcmd_fifo,&p2,8);
	fifo_output(&GLOMPcmd_fifo,&p3,8);
	fifo_output(&GLOMPcmd_fifo,&p4,8);
	fifo_output(&GLOMPcmd_fifo,&p5,8);

#ifdef ENABLE_SINGLE_SCREEN
	int totalload=0;
	int beforeload=0;
	int i;
	GLdouble newp2;
	GLdouble newp3;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}
	newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
	newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);
	lib_glFrustum(p0,p1,newp2,newp3,p4,p5);
#else
	tile_screen_glFrustum(client_num,p0,p1,p2,p3,p4,p5);
#endif
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

#ifdef ENABLE_SINGLE_SCREEN
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
#else
	tile_screen_glFrustum(client_num,p0,p1,p2,p3,p4,p5);
#endif
}


void glGenTextures ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+2;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_texture);
      GLuint myid;
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenTextures ( 1 , &myid );
      id_add(id,myid);
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

void glNewList(GLuint list,GLenum mode)
{
	int fnum=OVERRIDE_BASE+42;

	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&list,4);
	fifo_output(&GLOMPcmd_fifo,&mode,4);

	lib_glNewList (id_translate(list),mode);
}

void GLOMPglNewList()
{
	GLuint list;
	GLenum mode;
	fifo_input(&GLOMPcmd_fifo,&list,4);
	fifo_input(&GLOMPcmd_fifo,&mode,4);
	lib_glNewList (id_translate(list),mode);
}

GLboolean glIsTexture ( GLuint p0 )
{
  return id_server_test_type(p0,id_texture);
}

void glBindTexture ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+3;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
  p1=id_translate(p1);

  lib_glBindTexture ( p0 , p1 );//on utilise le vrai bind texture
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  GLuint ret=0;
  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_list);
      if (i==0)
	ret=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      myid=lib_glGenLists(1);
      id_add(id,myid);
      printf("added (%d,%d)\n",id,myid);
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
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  lib_glCallList(id_translate(p0));
}

void GLOMPglCallList ()
{
  GLuint p0;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  lib_glCallList(id_translate(p0));
}

const GLubyte* glGetString( GLenum name )
{
	const GLubyte* vendor="Stephane Marchesin";
 	const GLubyte* renderer="Glomp on ";
	static GLubyte return_renderer[4096];
	const GLubyte* version="1.2 Glomp";
	const GLubyte* extensions=NULL;
	static GLubyte return_extensions[4096];
	GLubyte* white_space=" ";

	switch(name)
	{
		case GL_VENDOR:
			return vendor;
		case GL_RENDERER:
		{
			/* we gather the renderers from all the cards */
			int fnum=OVERRIDE_BASE+43;
			int c;
			fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
			fifo_output(&GLOMPcmd_fifo,&name,4);
			fifo_flush(&GLOMPcmd_fifo);
			char** client_renderer=(char**)malloc(sizeof(char*)*nbcarte);
			strcpy(return_renderer,renderer);
			strcat(return_renderer,"[");
			strcat(return_renderer,lib_glGetString(name));
			// wait for the clients to answer
			for(c=1;c<nbcarte;c++)
			{
				strcat(return_renderer,"] [");
				sem_wait(&semadr[c]);
				client_renderer[c]=shmadr+4096*c;
				strcat(return_renderer,client_renderer[c]);
			}
			strcat(return_renderer,"]");
			free(client_renderer);
			return return_renderer;
		}
		case GL_VERSION:
			return version;
		case GL_EXTENSIONS:
		{
			/* blacklist extensions that can't be auto genereted and aren't written yet */
			char* blacklisted_extensions = "GL_EXT_vertex_array GL_EXT_compiled_vertex_array GL_NV_vertex_array_range GL_NV_vertex_array_range2";
			/* we have different cards, but need to expose a single extension string
			 * so we query each card for its extensions and intersect */
			int fnum=OVERRIDE_BASE+43;
			int i,j,c;
			fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
			fifo_output(&GLOMPcmd_fifo,&name,4);
			fifo_flush(&GLOMPcmd_fifo);
			extensions=lib_glGetString(name);
			char** client_extensions=(char**)malloc(sizeof(char*)*nbcarte);
			// wait for the clients to answer
			for(c=1;c<nbcarte;c++)
			{
				sem_wait(&semadr[c]);
				client_extensions[c]=shmadr+4096*c;
			}

			i=0;
			return_extensions[0]=0;
			do
			{
				// find the next extension string
				j=0;
				while((i<strlen(extensions))&&(extensions[i]==' '))
					i++;
				while((i+j<strlen(extensions))&&(extensions[i+j]!=' '))
					j++;

				// check that this extension is available on all the clients
				int found=1;
				for(c=1;c<nbcarte;c++)
				{
					if (!memmem(client_extensions[c],strlen(client_extensions[c]),&extensions[i],j))
						found=0;
				}

				// found the extension in all clients, expose it
				if ((!memmem(blacklisted_extensions,strlen(blacklisted_extensions),&extensions[i],j))&&(found))
				{
					strncat(return_extensions,&extensions[i],j);
					strncat(return_extensions,white_space,1);
				}

				i+=j;
			}
			while(i<strlen(extensions));
			free(client_extensions);
		}
		return return_extensions;
	}
	return NULL;
}

void GLOMPglGetString()
{
	GLenum name;
	fifo_input(&GLOMPcmd_fifo,&name,4);
	const char* s=(const char*)lib_glGetString(name);
	strcpy(shmadr+4096*client_num,s);
	sem_post(&semadr[client_num]);
}

/*pas sur d'avoir la bonne taille dasn segment_create*/
void glCallLists (GLsizei n, GLenum type, const GLvoid *lists)
{
	printf("ooooooooops implement glCallLists\n");
// XXX BROKEN
/*
  int fnum=OVERRIDE_BASE+6;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&n,4);
  fifo_output(&GLOMPcmd_fifo,&type,4);
  segment_create(lists,n);
  for(i=0;i<n;i++)
  {
    
  }
  lib_glCallLists(n,type,lists);
*/
}

void GLOMPglCallLists ()
{
/*  GLsizei p0;
  GLenum p1;
  GLvoid *p2;
  fifo_input(&GLOMPcmd_fifo,&p0,4);
  fifo_input(&GLOMPcmd_fifo,&p1,4);
  p2=(GLvoid*)(segment_attach());
  for(i=0;i<p0;i++)
  {
    lib_glCallList(p0,p1,p2);
  }
  segment_delete();*/
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
  GLuint myid;
  
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_texture);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenTexturesEXT ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenTexturesEXT ( 1 , &id );
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindTextureEXT ( p0 , p1 );//on utilise le vrai bind texture
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenQueries ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenQueries ( 1 , &id );
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenBuffers ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenBuffers ( 1 , &id );
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
 
  p1=id_translate(p1);
  
  lib_glBindBuffer ( p0 , p1 );//on utilise le vrai bind buffer
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_program);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenProgramsARB ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenProgramsARB ( 1 , &id );
      id_add(p1,id);
    }
}

void glBindProgramARB ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+13;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);
 
  p1=id_translate(p1);
  
  lib_glBindProgramARB ( p0 , p1 );//on utilise le vrai bind program
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenBuffersARB ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenBuffersARB ( 1 , &id );
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindBufferARB ( p0 , p1 );//on utilise le vrai bind bufferARB
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenQueriesARB ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenQueriesARB ( 1 , &id );
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_fence);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenFencesNV ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenFencesNV ( 1 , &id );
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_program);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenProgramsNV ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenProgramsNV ( 1 , &id );
      id_add(p1,id);
    }
}

void glBindProgramNV ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+19;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindProgramNV ( p0 , p1 );//on utilise le vrai bind program
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenOcclusionQueriesNV ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenOcclusionQueriesNV ( 1 , &id );
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenRenderbuffersEXT ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenRenderbuffersEXT ( 1 , &id );
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindRenderbufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
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
  GLuint myid;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&GLOMPcmd_fifo,&id,4);
      lib_glGenFramebuffersEXT ( 1 , &myid );
      id_add(id,myid);
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
      lib_glGenFramebuffersEXT ( 1 , &id );
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

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);
  fifo_output(&GLOMPcmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindFramebufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
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
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  lib_glFlush();
}

void GLOMPglFlush()
{
  lib_glFlush();
}

/*glFlush à n'utiliser que sur le maitre ? -> donc pas de fifo*/
void glFinish(void){
  int fnum=OVERRIDE_BASE+26;
  fifo_flush(&GLOMPcmd_fifo);
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  lib_glFinish();
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
  GLuint id;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,p1,4);
      id=id_translate(*p1);
      lib_glDeleteTextures ( 1 , &id);
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
      lib_glDeleteTextures ( 1 , &id);
		
    }
}
/*deleteQuery*/
void glDeleteQueries (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+28;
  GLuint id;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,p1,4);
      id=id_translate(*p1);
      lib_glDeleteQueries ( 1 , &id);
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
      lib_glDeleteQueries ( 1 , &id);
		
    }
}
void glDeleteBuffers (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+29;
  GLuint id;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,p1,4);
      id=id_translate(*p1);
      lib_glDeleteBuffers ( 1 , &id);
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
      lib_glDeleteBuffers ( 1 , &id);
		
    }
}
void glDeleteProgramsARB (GLsizei p0, const GLuint * p1)
{
  int i;
  int id;
  int fnum=OVERRIDE_BASE+30;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteProgramsARB ( 1 , &id);
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
      lib_glDeleteProgramsARB ( 1 , &id);
		
    }
}


void glDeleteBuffersARB (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+31;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteBuffersARB ( 1 , &id);
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
      lib_glDeleteBuffersARB ( 1 , &id);
		
    }
}


void glDeleteQueriesARB (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+32;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteQueriesARB ( 1 , &id);
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
      lib_glDeleteQueriesARB ( 1 , &id);
		
    }
}


void glDeleteTexturesEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+33;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteTexturesEXT ( 1 , &id);
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
      lib_glDeleteTexturesEXT ( 1 , &id);
		
    }
}

void glDeleteFencesNV (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+34;
  GLuint id;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteFencesNV ( 1 , &id);
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
      lib_glDeleteFencesNV ( 1 , &id);
		
    }
}

void glDeleteProgramsNV (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+35;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteProgramsNV ( 1 , &id);
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
      lib_glDeleteProgramsNV ( 1 , &id);
		
    }
}


void glDeleteOcclusionQueriesNV (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+36;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteOcclusionQueriesNV ( 1 , &id);
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
      lib_glDeleteOcclusionQueriesNV ( 1 , &id);
		
    }
}



void glDeleteRenderbuffersEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+37;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteRenderbuffersEXT ( 1 , &id);
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
      lib_glDeleteRenderbuffersEXT ( 1 , &id);
		
    }
}


void glDeleteFramebuffersEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+38;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&GLOMPcmd_fifo,&p1,4);
      id=id_translate(*p1);
      lib_glDeleteFramebuffersEXT ( 1 , &id);
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
      lib_glDeleteFramebuffersEXT ( 1 , &id);
		
    }
}

#ifdef ENABLE_SINGLE_SCREEN
void glViewport ( GLint x,GLint y,GLsizei w,GLsizei h )
{
  int i;
  int fnum=OVERRIDE_BASE+40;

  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&x,4);
  fifo_output(&GLOMPcmd_fifo,&y,4);
  fifo_output(&GLOMPcmd_fifo,&w,4);
  fifo_output(&GLOMPcmd_fifo,&h,4);

  int totalload=0;
  int beforeload=0;
  GLsizei newp3;
  GLint newp1;
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
      if (i<client_num)
	beforeload+=client_load[i];
    }
  newp1=y+h*((double)beforeload/(double)totalload);

  newp3=h*(double)client_load[client_num]/(double)totalload;

  
  printf("viewport:%d    %d %d %d %d",client_num,y,h,newp1,newp3);

  lib_glViewport(x,newp1,w,newp3);
 
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
#endif


void glOrtho ( GLdouble p0, GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5 )
{

  int fnum=OVERRIDE_BASE+41;
  fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&GLOMPcmd_fifo,&p0,8);
  fifo_output(&GLOMPcmd_fifo,&p1,8);
  fifo_output(&GLOMPcmd_fifo,&p2,8);
  fifo_output(&GLOMPcmd_fifo,&p3,8);
  fifo_output(&GLOMPcmd_fifo,&p4,8);
  fifo_output(&GLOMPcmd_fifo,&p5,8);

#ifdef ENABLE_SINGLE_SCREEN
  int totalload=0;
  int beforeload=0;
  int i;
  GLdouble newp2;
  GLdouble newp3;
  for(i=0;i<nbcarte;i++)
    {
      totalload+=client_load[i];
      if (i<client_num)
	beforeload+=client_load[i];
    }
  
  
  newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
  newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);

  //  newp2=p2;
  //newp3=p3;
  

  lib_glOrtho(p0,p1,newp2,newp3,p4,p5);
#else
  tile_screen_glOrtho(client_num,p0,p1,p2,p3,p4,p5);
#endif
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
  
#ifdef ENABLE_SINGLE_SCREEN
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
#else
  tile_screen_glOrtho(client_num,p0,p1,p2,p3,p4,p5);
#endif
 
    
}

int XDestroyWindow ( Display * disp, Window parent)

{
  int r=lib_XDestroyWindow(disp,parent);
  kill(0,SIGKILL);
  return r;
}

int XCloseDisplay ( Display * disp) 
{
  int r=lib_XCloseDisplay(disp);
  kill(0,SIGKILL);  
  return r;
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

void glMap1f(GLenum target,GLfloat u1,GLfloat u2,GLint stride,GLint order,const GLfloat * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+44;

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&target,4);
	fifo_output(&GLOMPcmd_fifo,&u1,4);
	fifo_output(&GLOMPcmd_fifo,&u2,4);
	fifo_output(&GLOMPcmd_fifo,&stride,4);
	fifo_output(&GLOMPcmd_fifo,&order,4);
	sizep=order*sizeof(GLfloat);
	fifo_output(&GLOMPcmd_fifo,points,sizep);
	lib_glMap1f(target,u1,u2,stride,order,points);
}

void GLOMPglMap1f()
{
	GLenum target;
	GLfloat u1;
	GLfloat u2;
	GLint stride;
	GLint order;
	int sizep;
	fifo_input(&GLOMPcmd_fifo,&target,4);
	fifo_input(&GLOMPcmd_fifo,&u1,4);
	fifo_input(&GLOMPcmd_fifo,&u2,4);
	fifo_input(&GLOMPcmd_fifo,&stride,4);
	fifo_input(&GLOMPcmd_fifo,&order,4);
	GLfloat  points[order];
	sizep=order*sizeof(GLfloat);
	fifo_input(&GLOMPcmd_fifo,points,sizep);
	lib_glMap1f(target,u1,u2,stride,order,(GLfloat *)points);
}



void glMap1d(GLenum target,GLdouble u1,GLdouble u2,GLint stride,GLint order,const GLdouble * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+45;

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&target,4);
	fifo_output(&GLOMPcmd_fifo,&u1,8);
	fifo_output(&GLOMPcmd_fifo,&u2,8);
	fifo_output(&GLOMPcmd_fifo,&stride,4);
	fifo_output(&GLOMPcmd_fifo,&order,4);
	sizep=order*sizeof(GLdouble);
	fifo_output(&GLOMPcmd_fifo,points,sizep);
	lib_glMap1d(target,u1,u2,stride,order,points);
}

void GLOMPglMap1d()
{
	GLenum target;
	GLdouble u1;
	GLdouble u2;
	GLint stride;
	GLint order;
	int sizep;
	fifo_input(&GLOMPcmd_fifo,&target,4);
	fifo_input(&GLOMPcmd_fifo,&u1,8);
	fifo_input(&GLOMPcmd_fifo,&u2,8);
	fifo_input(&GLOMPcmd_fifo,&stride,4);
	fifo_input(&GLOMPcmd_fifo,&order,4);
	GLdouble  points[order];
	sizep=order*sizeof(GLdouble);
	fifo_input(&GLOMPcmd_fifo,points,sizep);
	lib_glMap1d(target,u1,u2,stride,order,(GLdouble *)points);
}

void glMap2f(GLenum target,GLfloat u1,GLfloat u2,GLint ustride,GLint uorder,GLfloat v1,GLfloat v2,GLint vstride,GLint vorder,const GLfloat * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+46;
	printf("glMap2f\n");

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&target,4);
	fifo_output(&GLOMPcmd_fifo,&u1,4);
	fifo_output(&GLOMPcmd_fifo,&u2,4);
	fifo_output(&GLOMPcmd_fifo,&ustride,4);
	fifo_output(&GLOMPcmd_fifo,&uorder,4);
	fifo_output(&GLOMPcmd_fifo,&v1,4);
	fifo_output(&GLOMPcmd_fifo,&v2,4);
	fifo_output(&GLOMPcmd_fifo,&vstride,4);
	fifo_output(&GLOMPcmd_fifo,&vorder,4);
	sizep=uorder*vorder*sizeof(GLfloat);
	fifo_output(&GLOMPcmd_fifo,points,sizep);
	lib_glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);
}

void GLOMPglMap2f()
{
	GLenum target;
	GLfloat u1;
	GLfloat u2;
	GLint ustride;
	GLint uorder;
	GLfloat v1;
	GLfloat v2;
	GLint vstride;
	GLint vorder;
	int sizep;
	fifo_input(&GLOMPcmd_fifo,&target,4);
	fifo_input(&GLOMPcmd_fifo,&u1,4);
	fifo_input(&GLOMPcmd_fifo,&u2,4);
	fifo_input(&GLOMPcmd_fifo,&ustride,4);
	fifo_input(&GLOMPcmd_fifo,&uorder,4);
	fifo_input(&GLOMPcmd_fifo,&v1,4);
	fifo_input(&GLOMPcmd_fifo,&v2,4);
	fifo_input(&GLOMPcmd_fifo,&vstride,4);
	fifo_input(&GLOMPcmd_fifo,&vorder,4);
	GLfloat  points[uorder*vorder];
	sizep=uorder*vorder*sizeof(GLfloat);
	fifo_input(&GLOMPcmd_fifo,points,sizep);
	lib_glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,(GLfloat *)points);
}


void glMap2d(GLenum target,GLdouble u1,GLdouble u2,GLint ustride,GLint uorder,GLdouble v1,GLdouble v2,GLint vstride,GLint vorder,const GLdouble * points)
{
	int sizep;
	int fnum=OVERRIDE_BASE+47;
	printf("glMap2d %d %f %f %d %d %f %f %d %d \n",target,u1,u2,ustride,uorder,v1,v2,vstride,vorder);
	int i;
	for(i=0;i<10;i++)
	printf("%f ",points[i]);
	printf("\n");

	if(DEBUG){printf("serveur fnum = %d\n",fnum);}
	fifo_output(&GLOMPcmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&GLOMPcmd_fifo,&target,4);
	fifo_output(&GLOMPcmd_fifo,&u1,8);
	fifo_output(&GLOMPcmd_fifo,&u2,8);
	fifo_output(&GLOMPcmd_fifo,&ustride,4);
	fifo_output(&GLOMPcmd_fifo,&uorder,4);
	fifo_output(&GLOMPcmd_fifo,&v1,8);
	fifo_output(&GLOMPcmd_fifo,&v2,8);
	fifo_output(&GLOMPcmd_fifo,&vstride,4);
	fifo_output(&GLOMPcmd_fifo,&vorder,4);
	sizep=uorder*vorder*sizeof(GLdouble);
	fifo_output(&GLOMPcmd_fifo,points,sizep);
	lib_glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);
}

void GLOMPglMap2d()
{
	GLenum target;
	GLdouble u1;
	GLdouble u2;
	GLint ustride;
	GLint uorder;
	GLdouble v1;
	GLdouble v2;
	GLint vstride;
	GLint vorder;
	int sizep;
	fifo_input(&GLOMPcmd_fifo,&target,4);
	fifo_input(&GLOMPcmd_fifo,&u1,8);
	fifo_input(&GLOMPcmd_fifo,&u2,8);
	fifo_input(&GLOMPcmd_fifo,&ustride,4);
	fifo_input(&GLOMPcmd_fifo,&uorder,4);
	fifo_input(&GLOMPcmd_fifo,&v1,8);
	fifo_input(&GLOMPcmd_fifo,&v2,8);
	fifo_input(&GLOMPcmd_fifo,&vstride,4);
	fifo_input(&GLOMPcmd_fifo,&vorder,4);
	sizep=uorder*vorder*sizeof(GLdouble);
	GLdouble  points[uorder*vorder];
	fifo_input(&GLOMPcmd_fifo,points,sizep);
	printf("client glMap2d %d %f %f %d %d %f %f %d %d \n",target,u1,u2,ustride,uorder,v1,v2,vstride,vorder);
	int i;
	for(i=0;i<10;i++)
	printf("%f ",points[i]);
	printf("\n");
	lib_glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,(GLdouble*)points);
}

GLboolean glIsEnabled(GLenum cap)
{
	if(DEBUG){printf("serveur glIsEnabled\n");}
	return lib_glIsEnabled(cap);
}




