#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define out_c_file "wrapped.c"
#define out_h_file "wrapped.h"
#define in_c_file  "dewrapped.c"
#define in_h_file  "dewrapped.h"

int np=-1;
int fnum=0;
int i;
char * typenoconst;
char * type[50];
int count[50];
char * return_type;



typedef struct gl_type{
	char* name;
	char* default_return_value;
	int size;
}
gl_type;

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


int main()
{
	char * docname="gl_API.xml";
	
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlNodePtr retour,retourcategory;
	xmlChar *attrib;

        FILE * fout_c=fopen(out_c_file,"wb");
	FILE * fout_h=fopen(out_h_file,"wb");
	FILE * fin_c=fopen(in_c_file,"wb");
	FILE * fin_h=fopen(in_h_file,"wb");
	FILE * ftmpc=fopen("tmpc","w+b");
	FILE * ftmpc2=fopen("tmph","w+b");


        for(i=0;i<50;i++)
		type[i]=malloc(sizeof(char)*50);

	return_type=malloc(sizeof(char)*50);
	typenoconst=malloc(sizeof(char)*50);


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
	fprintf(fout_h,"/* Auto-generated, do not edit ! */\n#include \"fifo.h\"\n#include <GL/gl.h>\n#include <GL/glext.h>\n\n");
  
	fprintf(fin_c,"/* Auto-generated, do not edit ! */\n#include \""in_h_file"\"\n\n");
	fprintf(fin_c,"void init()\n{\n");

	fprintf(fin_h,"/* Auto-generated, do not edit ! */\n#include \"fifo.h\"\n#include <GL/gl.h>\n#include <GL/glext.h>\n#include <GL/glx.h>\n#include <GL/glxext.h>\n\n");
  
	fprintf(ftmpc2,"void creertabfunc()\n{\n");

         
	cur = cur->xmlChildrenNode;
        cur = cur->next;

	while (cur != NULL) 
	{
	    retourcategory = cur;
	    if((!xmlStrcmp(cur->name, (const xmlChar *)"category")))
	    {
		cur = cur->xmlChildrenNode;
		cur = cur->next;		

		while (cur != NULL)
		{
		    retour=cur;
		    if ((!xmlStrcmp(cur->name, (const xmlChar *)"function"))) 
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
			    fprintf(fout_c,"%s ",attrib);
			    fprintf(fout_h,"%s ",attrib);
			}
			else
			{
			    fprintf(fout_c,"void ");
			    fprintf(fout_h,"void ");
			    strcpy(return_type,"void");
			}
			cur = retour;

			attrib = xmlGetProp(cur, "name");
			fprintf(fout_c,"gl%s(",attrib);
			fprintf(fout_h,"gl%s(",attrib);
			fprintf(fin_c,"\tglfunctable[%d]=(__GLXextFuncPtr)glXGetProcAddressARB(\"gl%s\");\n",fnum,attrib);
			fprintf(ftmpc,"void fgl%s()\n{\n",attrib);
			fprintf(ftmpc2,"\tfunctable[%d]=&fgl%s;\n",fnum,attrib);
	 		fprintf(fin_h,"void fgl%s();\n",attrib);
                  
			cur = cur->xmlChildrenNode;
			if(cur->next!=NULL)
			    cur = cur->next; 
			attrib = xmlGetProp(cur, "name");
			
			np=-1;                    

	 		while((!xmlStrcmp(cur->name, (const xmlChar *)"param")))
			{
			    np++;
			    attrib = xmlGetProp(cur, "type");

			    fprintf(fout_c,"%s p%d,",attrib,np);
			    fprintf(fout_h,"%s,",attrib);
			    fprintf(ftmpc,"\t%s p%d;\n",attrib,np);

			    strcpy(type[np],attrib);
			    attrib=xmlGetProp(cur, "count");
			    if( attrib!=NULL)
			    count[np]=atoi(attrib);
			    else count[np]=0;

			    if(cur->next->next==NULL)
				break;
			    cur = cur->next->next;
			}
	
			if(np!=-1)
			{
			    fseek(fout_c,-1,SEEK_CUR);
			    fseek(fout_h,-1,SEEK_CUR);
			}
			fprintf(fout_c,")\n{\n\tint fnum=%d;\n",fnum);
			fprintf(fout_c,"\tint fflags=0;\n");
			fprintf(fout_c,"\tOUTPUT_FIFO(&fnum,sizeof(fnum));\n");
			fprintf(fout_c,"\tOUTPUT_FIFO(&fflags,sizeof(fflags));\n");
			fprintf(fout_h,");\n");
			for(i=0;i<=np;i++)
			{
			    if(count[i]==0)
			    {
				fprintf(fout_c,"\tOUTPUT_FIFO(&p%d,sizeof(%d));\n",i,type_size(type[i]));
				fprintf(ftmpc,"\tINPUT_FIFO(&p%d,%d);\n",i,type_size(type[i]));
			    }
			    else
			    {    //printf("fnum:%d,  i:%d,  count:%d\n",fnum,i,count[i]);
				fprintf(fout_c,"\tOUTPUT_FIFO(p%d,sizeof(%d));\n",i,type_size(type[i])*count[i]);
				fprintf(ftmpc,"\tINPUT_FIFO(p%d,%d);\n",i,type_size(type[i])*count[i]);
			    }
			}
	
			fprintf(fout_c,"\treturn %s;\n",type_return(return_type));
			fprintf(fout_c,"}\n\n");
	
			fprintf(ftmpc,"\t((%s (*)(",return_type);
			for(i=0;i<=np;i++)
			    fprintf(ftmpc,"%s,",type[i]);
			if(np==-1)
			    fprintf(ftmpc,"void");
			else fseek(ftmpc,-1,SEEK_CUR);
			fprintf(ftmpc,"))glfunctable[%d])(",fnum);                
			for(i=0;i<=np;i++)
			    fprintf(ftmpc,"p%d,",i);
			if(np!=-1)
			    fseek(ftmpc,-1,SEEK_CUR);
			fprintf(ftmpc,");\n}\n\n",type[i]);
	
			fnum++;
		    }//fin if function
		    cur = retour;
		    cur = cur->next;
		} //fin while function
	    }//fin if category
	    cur=retourcategory;
	    cur = cur->next;
	}//fin while final
	return;
}
   
