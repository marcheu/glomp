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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "config.h"
//#include "init.h"

#define DEBUG 1

#define out_c_file "wrapped.c"
#define out_h_file "wrapped.h"
#define in_c_file  "dewrapped.c"
#define in_h_file  "dewrapped.h"

int np=-1;
int fnum=0;
int i;
char * type[50];
char * return_type,* buffer;

char * nameparam[50];
//int count[50];
char * count[50];
char * variable_param[50];
char * img_width[50];
char * img_height[50];
char * img_depth[50];
char * img_type[50];
char * img_format[50];
int param_attrib[50][8];   // [0]=count [1]=variable_param [2]=img_width 
			   //[3]=img_height [4]=img_depth [5]=img_type [6]=output [7]=format


typedef struct gl_type{
  char* name;
  char* default_return_value;
  int size;
}gl_type;


/*------tableau des fonction ne devant pas etre traite par le parseur xml---*/
char * noParseFunction_table[]=
  {
    "NewList",

#ifdef ENABLE_SINGLE_SCREEN
    "Viewport",
#endif
    "IsQuery",
    "IsBuffer",
    "IsProgramARB",
    "IsBufferARB",
    "IsQueryARB",
    "IsFenceNV",
    "IsProgramNV",
    "IsTexture",
    "IsList",
    "Ortho",
    "IsTextureEXT",
    "IsOcclusionQueryNV",
    "IsRenderbufferEXT",
    "IsFramebufferEXT",
    "BindTexture",
    "GenTextures",   
    "Frustum",
    "GenLists",
    "CallList",
    "CallLists",
    "CopyPixels",
    "CopyTexImage1D",
    "CopyTexImage2D",
    "CopyTexSubImage1D",
    "CopyTexSubImage2D",
    "Flush",
    "DeleteTextures",
    "Flush",
    "Finish",
    "BindTextureEXT",  
    "GenTexturesEXT",
    "GenQueries",
    "BindBuffer",    
    "GenBuffers",
    "BindProgramARB",    
    "GenProgramsARB",
    "BindBufferARB",
    "GenBuffersARB",
    "GenQueriesARB",
    "GenFencesARB",
    "BindProgramNV"
    "GenProgramsNV",
    "GenOcclusionQueriesNV",
    "BindRenderbufferEXT",
    "GenRenderbuffersEXT",
    "BindFramebufferEXT",
    "GenFramebuffersEXT",
    "DeleteQueries",
    "DeleteBuffers",
    "DeleteProgramsARB",
    "DeleteBuffersARB",
    "DeleteQueriesARB",
    "DeleteTexturesEXT",
    "DeleteFencesNV",
    "DeleteProgramsNV",
    "DeleteOcclusionQueriesNV",
    "DeleteRenderbuffersEXT",
    "DeleteFramebuffersEXT",
    "GetString",
    "GenProgramsNV",
    "GenFencesNV",
    "BindProgramNV",
    "Map1d",
    "Map1f",
    "Map2d",
    "Map2f",
    "IsEnabled",
    /* implement vertex arrays ourselves */
    "VertexPointer",
    "VertexPointerEXT",
    "NormalPointer",
    "NormalPointerEXT",
    "ColorPointer",
    "ColorPointerEXT",
    "IndexPointer",
    "IndexPointerEXT",
    "TexCoordPointer",
    "TexCoordPointerEXT",
    "EdgeFlagPointer",
    "EdgeFlagPointerEXT",
    "FogCoordPointer",
    "FogCoordPointerEXT",
    "SecondaryColorPointer",
    "SecondaryColorPointerEXT",
    "VertexAttribPointer",
    "VertexAttribPointerNV",
    "VertexAttribPointerARB",
    "VertexWeightPointerEXT",
    "WeightPointerARB",
    "ArrayElement",
    "ArrayElementEXT",
    "DrawElements",
    "EnableClientState",
    "DisableClientState",
    "DrawArrays",
    "DrawArraysEXT",
    "ClientActiveTexture",
    "ClientActiveTextureARB",
    /* wrap around matrix load for tiled screen projection setup */
    "LoadMatrixf",
    "LoadMatrixd",

    /*et dans wrapped pb*/
    "GetMapdv",
    "GetMapfv",
    "GetMapiv",
    "GetTexImage",
    /*"GetBooleanv",
      "GetClipPlane",
      "GetDoublev",
      "GetError",
      "GetFloatv",
      "GetIntegerv",
      "GetLightfv",
      "GetLightiv",
      "GetMapdv",
      "GetMapfv",
      "GetMapiv",
      "GetMaterialfv",
      "GetMaterialiv",
      "GetPixelMapfv",
      "GetPixelMapuiv",
      "GetPixelMapusv",
      "GetPointerv",
      "GetPolygonStipple",
      "GetString",          //pas xxx dans gl.h
      "GetTexEnvfv",
      "GetTexEnviv",
      "GetTexGendv",
      "GetTexGenfv",
      "GetTexGeniv",
      "GetTexImage",
      "GetTexLevelParameterfv",
      "GetTexLevelParameteriv",
      "GetTexParameterfv",
      "GetTexParameteriv",*/    //fin des fonction du fichier gl.h 
    NULL,
  };


gl_type type_table[]=
  {
    {"","",0},
    {"void","",0},
    {"GLsizeiptr","NULL",sizeof(GLsizeiptr)},
    {"GLsizeiptrARB","NULL",sizeof(GLsizeiptrARB)},
    {"GLintptr","NULL",sizeof(GLintptr)},
    {"GLintptrARB","NULL",sizeof(GLintptrARB)},
    {"const GLubyte *","NULL",sizeof(GLubyte)},

    {"GLvoid","",sizeof(GLvoid)},
    {"GLvoid *","NULL",sizeof(GLvoid)},
    {"GLvoid **","NULL",sizeof(GLvoid)},

    {"GLboolean","GL_TRUE",sizeof(GLboolean)},
    {"GLboolean *","NULL",sizeof(GLboolean)},
    {"GLboolean **","NULL",sizeof(GLboolean)},

    {"GLenum","0",sizeof(GLenum)},
    {"GLenum *","NULL",sizeof(GLenum)},

    {"GLhandleARB","0",sizeof(GLhandleARB)},
    {"GLhandleARB *","NULL",sizeof(GLhandleARB)},

    {"GLint","0",sizeof(GLint)},
    {"GLint *","NULL",sizeof(GLint)},

    {"GLuint","0",sizeof(GLuint)},
    {"GLuint *","NULL",sizeof(GLuint)},

    {"GLubyte","0",sizeof(GLubyte)},
    {"GLubyte *","NULL",sizeof(GLubyte)},

    {"GLchar","0",sizeof(GLchar)},
    {"GLchar *","NULL",sizeof(GLchar)},
    {"GLchar **","NULL",sizeof(GLchar)},

    {"GLcharARB","0",sizeof(GLcharARB)},
    {"GLcharARB *","NULL",sizeof(GLcharARB)},
    {"GLcharARB **","NULL",sizeof(GLcharARB)},

    {"GLbyte","0",sizeof(GLbyte)},
    {"GLbyte *","NULL",sizeof(GLbyte)},

    {"GLshort","0",sizeof(GLshort)},
    {"GLshort *","NULL",sizeof(GLshort)},

    {"GLushort","0",sizeof(GLushort)},
    {"GLushort *","NULL",sizeof(GLushort)},

    {"GLfloat","0.0",sizeof(GLfloat)},
    {"GLfloat *","NULL",sizeof(GLfloat)},

    {"GLdouble","0.0",sizeof(GLdouble)},
    {"GLdouble *","NULL",sizeof(GLdouble)},

    {"GLsizei","0",sizeof(GLsizei)},
    {"GLsizei *","NULL",sizeof(GLsizei)},

    {"GLclampf","0.0",sizeof(GLclampf)},
    {"GLclampf *","NULL",sizeof(GLclampf)},

    {"GLclampd","0.0",sizeof(GLclampd)},
    {"GLclampd *","NULL",sizeof(GLclampd)},

    {"GLhalfNV","0.0",sizeof(GLhalfNV)},
    {"GLhalfNV *","NULL",sizeof(GLhalfNV)},

    {"GLbitfield", "0", sizeof(GLbitfield)},

    {"GLint64EXT *","NULL",sizeof(int64_t)},

    {"GLuint64EXT *","NULL",sizeof(int64_t)},

    {NULL,NULL,0},
  };


int remove_getfunction(char * name)
{
  if(!strncmp(name,"Get",3))
    return 0;
  else return 1;
}



int isFonctionParse(char * name)
{
  int i=0;
	
  if(name==NULL)
    return 1; 

  while(noParseFunction_table[i]!=NULL)
    {
      if (strcmp(name,noParseFunction_table[i])==0)
	return 0;
      i++;
    }
  return 1;
}




char* type_return(char* type)
{
  int i=0;

  while(type_table[i].name!=NULL)
    {
      if (strcmp(type,type_table[i].name)==0)
	return type_table[i].default_return_value; 
      i++;
    }
  return NULL;
}

int type_size(char* type)
{
  int i=0;
  if (strncmp(type,"const",5)==0)
    type+=6;
  while(type_table[i].name!=NULL)
    {
      if (strcmp(type,type_table[i].name)==0)
	return type_table[i].size; 
      i++;
    }
  return 0;
}

char * type_remove_const(char * type)
{
  if (strncmp(type,"const",5)==0)
    type+=6;
  return type;
}

char * type_remove_etoile(char * type)
{
  char *returntype=malloc(sizeof(char)*20);
  char * typeretoile;

  strcpy(returntype,type);
  typeretoile=strchr(returntype,'*');
  if(typeretoile==NULL)
    return type;
  else typeretoile[0]='\0';
  return returntype;
}


int main()
{
  char * docname="gl_API.xml";
	
  xmlDocPtr doc;
  xmlNodePtr cur;
  xmlNodePtr retour,retourcategory;
  xmlChar *attrib;
  xmlChar *func_name;

  FILE * fout_c=fopen(out_c_file,"wb");
  FILE * fout_h=fopen(out_h_file,"wb");
  FILE * fout_c2=fopen("fout_c2","w+b");

  FILE * fin_c=fopen(in_c_file,"wb");
  FILE * fin_h=fopen(in_h_file,"wb");
  FILE * ftmpc=fopen("tmpc","w+b");
  FILE * ftmpc2=fopen("tmpc2","w+b");


  for(i=0;i<50;i++)
    {
      type[i]=malloc(sizeof(char)*50);
	
      nameparam[i]=malloc(sizeof(char)*50);
      img_width[i]=malloc(sizeof(char)*50);
      img_height[i]=malloc(sizeof(char)*50);
      img_depth[i]=malloc(sizeof(char)*50);
      img_type[i]=malloc(sizeof(char)*50);
      count[i]=malloc(sizeof(char)*50);
      variable_param[i]=malloc(sizeof(char)*50);
      img_format[i]=malloc(sizeof(char)*50);
    }

  return_type=malloc(sizeof(char)*50);
  buffer=malloc(sizeof(char)*2048);
	


  doc = xmlParseFile(docname);
	
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully. \n");
    return;
  }

  cur = xmlDocGetRootElement(doc);
	
  if (cur == NULL) {
    fprintf(stderr,"empty document\n");
    xmlFreeDoc(doc);
    return;
  }
	
  if (xmlStrcmp(cur->name, (const xmlChar *) "OpenGLAPI")) {
    fprintf(stderr,"document of the wrong type, root node != story");
    xmlFreeDoc(doc);
    return;
  }


  fprintf(fout_c,"/* Auto-generated, do not edit ! */\n#include \""out_h_file"\"\n\n");
  fprintf(fout_c,"gl_type type_table[]=\n{\n");
  fprintf(fout_h,"/* Auto-generated, do not edit ! */\n\n#ifndef _WRAPPED_H_\n#define _WRAPPED_H_\n\n#include \"fifo.h\"\n#include \"segment.h\"\n#include \"glheader.h\"\n#include \"server.h\"\n#include \"overrides.h\"\n\n");
  fprintf(fout_h,"typedef struct gl_type{\n\tint name;\n\tint size;\n}gl_type;\n\n\n");

  fprintf(fout_h,"\nextern gl_type type_table[];\n\n\n");	

  
  fprintf(fin_c,"/* Auto-generated, do not edit ! */\n#include \""in_h_file"\"\n\n");
  fprintf(fin_c,"void initPointers()\n{\n");

  fprintf(fin_h,"/* Auto-generated, do not edit ! */\n#ifndef _DEWRAPPED_H_\n#define _DEWRAPPED_H_\n\n\n#include \"fifo.h\"\n#include \"segment.h\"\n#include \"glheader.h\"\n#include <GL/glx.h>\n#include <GL/glxext.h>\n#include \"server.h\"\n#include \"wrapped.h\" \n#include \"client.h\"\n\n");
  
  fprintf(fin_h,"void creertabfunc();\n\n");//provient de init  (a l'orrigine)

  fprintf(ftmpc2,"void creertabfunc()\n{\n");

         
  cur = cur->xmlChildrenNode;
  cur = cur->next;

  while (cur != NULL) 
    {
      retourcategory = cur;
      if( (!xmlStrcmp(cur->name, (const xmlChar *)"category")) )
	{
	  cur = cur->xmlChildrenNode;
	  cur = cur->next;		

	  while (cur != NULL)
	    {
	      retour=cur;
	      if( (!xmlStrcmp(cur->name, (const xmlChar *)"enum")) )
		{
		  attrib=xmlGetProp(cur, "count");
		  if(attrib!=NULL)
		    {
		      attrib=xmlGetProp(cur, "value");
		      fprintf(fout_c,"\t{%s,",attrib);
		      attrib=xmlGetProp(cur, "count");
		      fprintf(fout_c,"%d},\n",atoi(attrib)*8);
		    }
		}

	      else if ( (!xmlStrcmp(cur->name, (const xmlChar *)"function")) && 
			isFonctionParse(xmlGetProp(cur,"name")) 
			/*remove_getfunction(xmlGetProp(cur,"name"))*/) 
		{
		  cur = cur->xmlChildrenNode;
		  if(cur->next!=NULL)
		    cur = cur->next;

		  while((!xmlStrcmp(cur->name, (const xmlChar *)"param")))
		    {
		      if(cur->next->next==NULL)
			break;
		      cur = cur->next->next;
		    }			

		  if(!xmlStrcmp(cur->name, (const xmlChar *)"return"))
		    {
		      attrib = xmlGetProp(cur, "type");
		      strcpy(return_type,attrib);
		      fprintf(fout_c2,"%s ",attrib);
		      fprintf(fout_h,"%s ",attrib);
		    }
		  else
		    {
		      fprintf(fout_c2,"void ");
		      fprintf(fout_h,"void ");
		      strcpy(return_type,"void");
		    }
		  cur = retour;

		  func_name = xmlGetProp(cur, "name");
		  fprintf(fout_c2,"gl%s(",func_name);
		  fprintf(fout_h,"gl%s(",func_name);
		  fprintf(fin_c,"\tglfunctable[%d]=(__GLXextFuncPtr)lib_glXGetProcAddressARB(\"gl%s\");\n",fnum,func_name);
		  fprintf(ftmpc,"void GLOMPgl%s()\n{\n",func_name);
		  //if(DEBUG)fprintf(ftmpc,"printf(\"jsuis client fnum=%d\\n\");",fnum);
		  fprintf(ftmpc2,"\tfunctable[%d]=&GLOMPgl%s;\n",fnum,func_name);
		  fprintf(fin_h,"void GLOMPgl%s();\n",func_name);
                  
		  cur = cur->xmlChildrenNode;
		  if(cur->next!=NULL)
		    cur = cur->next; 
		  attrib = xmlGetProp(cur, "name");
			
		  np=-1;                    

			
		  if(!xmlStrcmp(cur->name, (const xmlChar *)"return"))
		    cur = cur->next->next;

		  /*---parametre de la fonction---*/
		  while((!xmlStrcmp(cur->name, (const xmlChar *)"param")))
		    {
		      np++;

		      attrib = xmlGetProp(cur, "name");
		      strcpy(nameparam[np],attrib);

		      attrib = xmlGetProp(cur, "type");
		      fprintf(fout_c2,"%s %s,",attrib,nameparam[np]);
		      fprintf(fout_h,"%s,",attrib);
		      strcpy(type[np],attrib);

				
		      /*verification de presence de l'attribut count*/
		      attrib=xmlGetProp(cur, "count");
		      if( attrib!=NULL)
			{
			  strcpy(count[np],attrib);
			  param_attrib[np][0]=1;
			}
		      else param_attrib[np][0]=0;


		      /*verification de presence de l'attribut variable_param*/
		      attrib=xmlGetProp(cur, "variable_param");
		      if( attrib!=NULL)
			{
			  strcpy(variable_param[np],attrib);
			  param_attrib[np][1]=1;
			}
		      else param_attrib[np][1]=0;


		      /*verification de presence de l'attribut img_width*/
		      attrib=xmlGetProp(cur, "img_width");
		      if( attrib!=NULL)
			{
			  strcpy(img_width[np],attrib);
			  param_attrib[np][2]=1;
			}
		      else param_attrib[np][2]=0;

		      /*verification de presence de l'attribut img_height*/
		      attrib=xmlGetProp(cur, "img_height");
		      if( attrib!=NULL)
			{
			  strcpy(img_height[np],attrib);
			  param_attrib[np][3]=1;
			}
		      else param_attrib[np][3]=0;

		      /*verification de presence de l'attribut img_depth*/
		      attrib=xmlGetProp(cur, "img_depth");
		      if( attrib!=NULL)
			{
			  strcpy(img_depth[np],attrib);
			  param_attrib[np][4]=1;
			}
		      else param_attrib[np][4]=0;

		      /*verification de presence de l'attribut img_type*/
		      attrib=xmlGetProp(cur, "img_type");
		      if( attrib!=NULL)
			{
			  strcpy(img_type[np],attrib);
			  param_attrib[np][5]=1;
			}
		      else param_attrib[np][5]=0;

		      /*verification de presence de l'attribut output*/
		      attrib=xmlGetProp(cur, "output");
		      if( attrib!=NULL)
			{
			  param_attrib[np][6]=1;
			}
		      else param_attrib[np][6]=0;

		      /*verification de presence de l'attribut img_format*/
		      attrib=xmlGetProp(cur, "img_format");
		      if( attrib!=NULL)
			{
			  param_attrib[np][7]=1;
			  strcpy(img_format[np],attrib);
			}
		      else param_attrib[np][7]=0;


		      /*if(param_attrib[np][0]==1)
			fprintf(ftmpc,"\t%s %s[%d];\n",type_remove_etoile(type_remove_const(type[np])),
			nameparam[np],count[np]);
			else fprintf(ftmpc,"\t%s %s;\n",type_remove_const(type[np]),nameparam[np]);*/

		      if( (param_attrib[np][0]==0 && param_attrib[np][1]==0) ||  
			  param_attrib[np][2]==1 || param_attrib[np][6]==1)
			fprintf(ftmpc,"\t%s %s;\n",type_remove_const(type[np]),nameparam[np]);

		      if(cur->next->next==NULL)
			break;
		      cur = cur->next->next;
		    }      //fin de detection des parametres de la fonction




		  if(np!=-1)
		    {
		      fseek(fout_c2,-1,SEEK_CUR);
		      fseek(fout_h,-1,SEEK_CUR);
		    }
		  fprintf(fout_c2,")\n{\n");
			
		  // find if the function is master only
		  int master_only=0;
		  for(i=0;i<=np;i++)
		    if(param_attrib[i][6]==1)
		  	master_only=1;


		// generate the function body
		  for(i=0;i<=np;i++)
		    {
		      if(param_attrib[i][2]==1 || param_attrib[i][6]==1)
			{
			  fprintf(fout_c2,"\tint sizep;\n");
			  break;
			}
		      else if(param_attrib[i][1]==1 || param_attrib[i][0]==1) 
			{
			  fprintf(fout_c2,"\tint sizep;\n");
			  fprintf(ftmpc,"\tint sizep;\n");
			  break;
			}
		    }
		  if (!master_only)
		  {
		  fprintf(fout_c2,"\tint fnum=%d;\n",fnum);
		  fprintf(fout_c2,"\n\tif(DEBUG){printf(\"serveur fnum = %%d\\n\",fnum);}\n");
//		  fprintf(fout_c2,"\tint fflags=0;\n");
		  fprintf(fout_c2,"\tfifo_output(&cmd_fifo,&fnum,sizeof(fnum));\n");
//		  fprintf(fout_c2,"\tfifo_output(&cmd_fifo,&fflags,sizeof(fflags));\n");
		  }

		  fprintf(fout_h,");\n");
		  for(i=0;i<=np;i++)
		    {
				
		      if(param_attrib[i][6]==1)
			{
			  if(param_attrib[i][2]==1)
			    {
			      if(param_attrib[i][3]==1)
				{
				  if(param_attrib[i][4]==1)
				    fprintf(fout_c2,"\tsizep=(%s*%s*%s*sizeGLenum(%s)+7)/8;\n",img_width[i],img_height[i],
					    img_depth[i],img_type[i]);
				  else fprintf(fout_c2,"\tsizep=(%s*%s*sizeGLenum(%s)+7)/8;\n",img_width[i],
					       img_height[i],img_type[i]);
				}
			      else fprintf(fout_c2,"\tsizep=(%s*sizeGLenum(%s)+7)/8;\n",img_width[i],img_type[i]);

			      //fprintf(ftmpc,"\twrite_segment(sizep);\n",nameparam[i],type_remove_const(type[np]));
			    }
			  else if(param_attrib[i][0]==1)
			    {   
			      fprintf(fout_c2,"\tsizep=%d*%s;\n",type_size(type[i]),count[i]);
			      //fprintf(ftmpc,"\tsizep=%d*%s;\n",type_size(type[i]),count[i]);
			    }
			  else if(param_attrib[i][1]==1)
			    {   
			      fprintf(fout_c2,"\tsizep=(sizeGLenum(%s)*%d+7)/8;\n",variable_param[i],type_size(type[i]));
			      //fprintf(ftmpc,"\tsizep=(sizeGLenum(%s)*%d+7)/8;\n",variable_param[i],type_size(type[i]));
			    }

			}
			

		      else if(param_attrib[i][2]==1)
			{
			  if(param_attrib[i][3]==1)
			    {
			      if(param_attrib[i][4]==1)
				fprintf(fout_c2,"\tsizep=(%s*%s*%s*sizeGLenum(%s)+7)/8;\n",img_width[i],img_height[i],
					img_depth[i],img_type[i]);
			      else fprintf(fout_c2,"\tsizep=(%s*%s*sizeGLenum(%s)+7)/8;\n",img_width[i],
					   img_height[i],img_type[i]);
			    }
			  else fprintf(fout_c2,"\tsizep=(%s*sizeGLenum(%s)+7)/8;\n",img_width[i],img_type[i]);

			  if(param_attrib[i][7]==1)
				fprintf(fout_c2,"\tsizep=(sizep*sizeGLenum(%s)+7)/8;\n",img_format[i]);

			  fprintf(fout_c2,"\tsegment_create((char *)%s,sizep);\n",nameparam[i]);
			  fprintf(ftmpc,"\t%s=(%s)segment_attach();\n",nameparam[i],type_remove_const(type[i]));
			}
	
		      else if(param_attrib[i][0]==1)
			{   
			  fprintf(ftmpc,"\t%s %s[%s];\n",type_remove_etoile(type_remove_const(type[i])),
				  nameparam[i],count[i]);
			  fprintf(fout_c2,"\tsizep=%d*%s;\n",type_size(type[i]),count[i]);
			  fprintf(ftmpc,"\tsizep=%d*%s;\n",type_size(type[i]),count[i]);

			  if (!master_only)
				  fprintf(fout_c2,"\tfifo_output(&cmd_fifo,%s,sizep);\n",nameparam[i]);
			  fprintf(ftmpc,"\tfifo_input(&cmd_fifo,%s,sizep);\n",nameparam[i]);
			}
		      else if(param_attrib[i][1]==1)
			{   
			  fprintf(ftmpc,"\t%s %s[(sizeGLenum(%s)+7)/8];\n",type_remove_etoile(type_remove_const(type[i])),
				  nameparam[i],variable_param[i]);
			  fprintf(fout_c2,"\tsizep=(sizeGLenum(%s)*%d+7)/8;\n",variable_param[i],type_size(type[i]));
			  fprintf(ftmpc,"\tsizep=(sizeGLenum(%s)*%d+7)/8;\n",variable_param[i],type_size(type[i]));				


			  if (!master_only)
				  fprintf(fout_c2,"\tfifo_output(&cmd_fifo,%s,sizep);\n",nameparam[i]);		     
			  fprintf(ftmpc,"\tfifo_input(&cmd_fifo,%s,sizep);\n",nameparam[i]);
			}
		      else
			{
				if (!master_only)
					fprintf(fout_c2,"\tfifo_output(&cmd_fifo,&%s,%d);\n",nameparam[i],type_size(type[i]));
			  fprintf(ftmpc,"\tfifo_input(&cmd_fifo,&%s,%d);\n",nameparam[i],type_size(type[i]));
			}
		    }
	
		  // call the function for the first node
		  fprintf(fout_c2,"\t((%s (*)(",return_type);
		  for(i=0;i<=np;i++)
		    fprintf(fout_c2,"%s,",type[i]);
		  if(np==-1)
		    fprintf(fout_c2,"void");
		  else fseek(fout_c2,-1,SEEK_CUR);
		  fprintf(fout_c2,"))glfunctable[%d])(",fnum);                
		  for(i=0;i<=np;i++)
		    {
/*		      if(param_attrib[i][0]==1 || param_attrib[i][1]==1 || param_attrib[i][2]==1 || 
			 param_attrib[i][6]==1   )
			fprintf(fout_c2,"(%s)",type[i]);*/
		      fprintf(fout_c2,"%s,",nameparam[i]);
		    }

		  if(np!=-1)
		    fseek(fout_c2,-1,SEEK_CUR);
		  fprintf(fout_c2,");\n");
		  

		  fprintf(fout_c2,"\treturn %s;\n",type_return(return_type));
		  fprintf(fout_c2,"}\n\n");
	

		  fprintf(ftmpc,"\t((%s (*)(",return_type);
		  for(i=0;i<=np;i++)
		    fprintf(ftmpc,"%s,",type_remove_const(type[i]));
		  if(np==-1)
		    fprintf(ftmpc,"void");
		  else fseek(ftmpc,-1,SEEK_CUR);
		  fprintf(ftmpc,"))glfunctable[%d])(",fnum);                
		  for(i=0;i<=np;i++)
		    {
		      if(param_attrib[i][0]==1 || param_attrib[i][1]==1 || param_attrib[i][2]==1)
		      {
			fprintf(ftmpc,"(%s)",type_remove_const(type[i]));
		        fprintf(ftmpc,"%s,",nameparam[i]);
		      }
		      else fprintf(ftmpc,"%s,",nameparam[i]);
		    }

		  if(np!=-1)
		    fseek(ftmpc,-1,SEEK_CUR);
		  fprintf(ftmpc,");\n");


		  for(i=0;i<=np;i++)
		    if(param_attrib[i][2]==1)
		      fprintf(ftmpc,"\tsegment_delete();\n");

		  fprintf(ftmpc,"}\n\n");

	
		  fnum++;
		}//fin if function
	      cur = retour;
	      cur = cur->next;
	    } //fin while function
	}//fin if category
      cur=retourcategory;
      cur = cur->next;
    }//fin while final

	
  fprintf(fout_c,"};\n\n");
  fprintf(fin_c,"}\n\n");

  fprintf(fin_h,"\n__GLXextFuncPtr glfunctable[%d];\n",fnum);
  fprintf(fin_h,"\nvoid (*functable[%d])(void);\n",fnum);

  fprintf(fin_h,"\n#endif\n");
  fprintf(fout_h,"\n#endif\n");

  fprintf(ftmpc2,"}\n");

  rewind(ftmpc);
  rewind(ftmpc2);  
  rewind(fout_c2);
  
  while(fgets(buffer,2048,fout_c2)!=NULL)
    fputs(buffer,fout_c);



  while(fgets(buffer,2048,ftmpc)!=NULL)
    fputs(buffer,fin_c); 


  while(fgets(buffer,2048,ftmpc2)!=NULL)
    fputs(buffer,fin_c);
  fputc('\n',fin_h);

  fclose(fout_c);
  fclose(fout_c2);
  fclose(fout_h);
  fclose(fin_c);
  fclose(fin_h);
  fclose(ftmpc);
  fclose(ftmpc2);

  free(return_type);
  free(buffer);
  for(i=0;i<50;i++)
    free(type[i]);

  system("rm -f tmpc tmpc2 fout_c2");

  return;
}
   
