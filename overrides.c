#define _GNU_SOURCE
#include <string.h>
#include "overrides.h"
#include "glheader.h"

/* functions we implement ourselves */

__GLXextFuncPtr glXGetProcAddress (const GLubyte * procName)
{
	printf("pouet +%s+\n",procName);
	return lib_glXGetProcAddressARB(procName);
}

__GLXextFuncPtr glXGetProcAddressARB (const GLubyte *procName)
{
	printf("pouetARB +%s+\n",procName);
	return lib_glXGetProcAddressARB(procName);
}



void glGenTextures ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+2;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_texture);
      GLuint myid;
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
      fifo_input(&cmd_fifo,&p1,4);
      lib_glGenTextures ( 1 , &id );
      id_add(p1,id);
    }
}

void glNewList(GLuint list,GLenum mode)
{
	int fnum=OVERRIDE_BASE+42;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&list,4);
	fifo_output(&cmd_fifo,&mode,4);

	lib_glNewList (id_translate(list),mode);
}

void GLOMPglNewList()
{
	GLuint list;
	GLenum mode;
	fifo_input(&cmd_fifo,&list,4);
	fifo_input(&cmd_fifo,&mode,4);
	lib_glNewList (id_translate(list),mode);
}

GLboolean glIsTexture ( GLuint p0 )
{
  return id_server_test_type(p0,id_texture);
}

void glBindTexture ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+3;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
  p1=id_translate(p1);

  lib_glBindTexture ( p0 , p1 );//on utilise le vrai bind texture
  return ;

}

void GLOMPglBindTexture()
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
  GLuint myid;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  GLuint ret=0;
  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_list);
      if (i==0)
	ret=id;
      fifo_output(&cmd_fifo,&id,4);
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



  fifo_input(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {

      fifo_input(&cmd_fifo,&p1,4);
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  lib_glCallList(id_translate(p0));
}

void GLOMPglCallList ()
{
  GLuint p0;
  fifo_input(&cmd_fifo,&p0,4);
  lib_glCallList(id_translate(p0));
}

/*pas sur d'avoir la bonne taille dasn segment_create*/
void glCallLists (GLsizei n, GLenum type, const GLvoid *lists)
{
	printf("ooooooooops implement glCallLists\n");
// XXX BROKEN
/*
  int fnum=OVERRIDE_BASE+6;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&n,4);
  fifo_output(&cmd_fifo,&type,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
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
  
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_texture);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindTextureEXT ( p0 , p1 );//on utilise le vrai bind texture
}

void GLOMPglBindTextureEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindTextureEXT ( p0 , p1 );//on utilise le vrai bind texture
}

/*fin des fonction EXt de texturing*/

void glGenQueries ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+9;
  GLuint myid;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
 
  p1=id_translate(p1);
  
  lib_glBindBuffer ( p0 , p1 );//on utilise le vrai bind buffer
}

void GLOMPglBindBuffer()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_program);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      lib_glGenProgramsARB ( 1 , &id );
      id_add(p1,id);
    }
}

void glBindProgramARB ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+13;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);
 
  p1=id_translate(p1);
  
  lib_glBindProgramARB ( p0 , p1 );//on utilise le vrai bind program
}

void GLOMPglBindProgramARB()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindBufferARB ( p0 , p1 );//on utilise le vrai bind bufferARB
}

void GLOMPglBindBufferARB()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindBufferARB ( p0 , p1 );//on utilise le vrai bind bufferARB
}
/*fin des bufferARB*/

void glGenQueriesARB ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+16;
  GLuint myid;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_fence);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_program);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      lib_glGenProgramsNV ( 1 , &id );
      id_add(p1,id);
    }
}

void glBindProgramNV ( GLenum p0 , GLuint p1 )
{

  int fnum=OVERRIDE_BASE+19;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindProgramNV ( p0 , p1 );//on utilise le vrai bind program
}

void GLOMPglBindProgramNV()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_occlusion_query);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindRenderbufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
}
void GLOMPglBindRenderbufferEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindRenderbufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
}


void glGenFramebuffersEXT ( GLsizei p0 , GLuint *p1 )
{
  int i;
  int fnum=OVERRIDE_BASE+23;
  GLuint myid;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      GLuint id=id_server_generate(id_buffer);
      *p1=id;
      fifo_output(&cmd_fifo,&id,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);
  fifo_output(&cmd_fifo,&p1,4);

  p1=id_translate(p1);
  
  lib_glBindFramebufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
}

void GLOMPglBindFramebufferEXT()
{
  GLenum p0;
  GLuint p1;
  fifo_input(&cmd_fifo,&p0,4);
  fifo_input(&cmd_fifo,&p1,4);
  
  p1=id_translate(p1);
  
  lib_glBindFramebufferEXT ( p0 , p1 );//on utilise le vrai bind buffer
}

/*fin des bufferEXT*/



void glFlush(void){
  int fnum=OVERRIDE_BASE+25;
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  lib_glFlush();
}

void GLOMPglFlush()
{
  lib_glFlush();
}

void glFinish(void){
  int fnum=OVERRIDE_BASE+26;
  fifo_flush(&cmd_fifo);
  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
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

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteQueries ( 1 , &id);
		
    }
}
void glDeleteBuffers (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+29;
  GLuint id;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteBuffers ( 1 , &id);
		
    }
}
void glDeleteProgramsARB (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+30;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteProgramsARB ( 1 , &id);
		
    }
}


void glDeleteBuffersARB (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+31;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteBuffersARB ( 1 , &id);
		
    }
}


void glDeleteQueriesARB (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+32;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteQueriesARB ( 1 , &id);
		
    }
}


void glDeleteTexturesEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+33;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteTexturesEXT ( 1 , &id);
		
    }
}

void glDeleteFencesNV (GLsizei p0, const GLuint * p1)
{
  int i;
  int fnum=OVERRIDE_BASE+34;
  GLuint id;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteFencesNV ( 1 , &id);
		
    }
}

void glDeleteProgramsNV (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+35;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteProgramsNV ( 1 , &id);
		
    }
}


void glDeleteOcclusionQueriesNV (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+36;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteOcclusionQueriesNV ( 1 , &id);
		
    }
}



void glDeleteRenderbuffersEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+37;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteRenderbuffersEXT ( 1 , &id);
		
    }
}


void glDeleteFramebuffersEXT (GLsizei p0, const GLuint * p1)
{
  int i;
  GLuint id;
  int fnum=OVERRIDE_BASE+38;

  fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
  fifo_output(&cmd_fifo,&p0,4);

  for(i=0;i<p0;i++)
    {
      fifo_output(&cmd_fifo,&p1,4);
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
  fifo_input(&cmd_fifo,&p0,4);
  for(i=0;i<p0;i++)
    {
		
      fifo_input(&cmd_fifo,&p1,4);
      id=id_translate(p1);
      lib_glDeleteFramebuffersEXT ( 1 , &id);
		
    }
}

GLboolean glIsEnabled(GLenum cap)
{
	if(DEBUG){printf("serveur glIsEnabled\n");}
	return lib_glIsEnabled(cap);
}




