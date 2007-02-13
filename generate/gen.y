%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <GL/gl.h>
#include <GL/glext.h>

extern FILE * yyin;
extern char * yytext;

#define out_c_file "wrapped.c"
#define out_h_file "wrapped.h"
#define in_c_file  "dewrapped.c"
#define in_h_file  "dewrapped.h"

FILE * fout_c;
FILE * fout_h;
FILE * fin_c;
FILE * ftmpc;
FILE * fin_h;
FILE * ftmph;

int np=-1;
int fnum=0;
int i;
int sizep[20];
char * ret_type,* type,* buffer;


char* type_return(char* type);

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
	{"const GLubyte*","NULL",sizeof(GLubyte*)},

        {"GLvoid","",sizeof(GLvoid)},
	{"GLvoid*","NULL",sizeof(GLvoid*)},
	{"GLvoid**","NULL",sizeof(GLvoid**)},

	{"GLboolean","GL_TRUE",sizeof(GLboolean)},
	{"GLboolean*","NULL",sizeof(GLboolean*)},
	{"GLboolean**","NULL",sizeof(GLboolean**)},

	{"GLenum","0",sizeof(GLenum)},
	{"GLenum*","NULL",sizeof(GLenum*)},

	{"GLhandleARB","0",sizeof(GLhandleARB)},
	{"GLhandleARB*","NULL",sizeof(GLhandleARB*)},

	{"GLint","0",sizeof(GLint)},
	{"GLint*","NULL",sizeof(GLint*)},

	{"GLuint","0",sizeof(GLuint)},
	{"GLuint*","NULL",sizeof(GLuint*)},

	{"GLubyte","0",sizeof(GLubyte)},
	{"GLubyte*","NULL",sizeof(GLubyte*)},

	{"GLchar","0",sizeof(GLchar)},
	{"GLchar*","NULL",sizeof(GLchar*)},
	{"GLchar**","NULL",sizeof(GLchar**)},

	{"GLcharARB","0",sizeof(GLcharARB)},
	{"GLcharARB*","NULL",sizeof(GLcharARB*)},
	{"GLcharARB**","NULL",sizeof(GLcharARB**)},

	{"GLbyte","0",sizeof(GLbyte)},
	{"GLbyte*","NULL",sizeof(GLbyte*)},

	{"GLshort","0",sizeof(GLshort)},
	{"GLshort*","NULL",sizeof(GLshort*)},

	{"GLushort","0",sizeof(GLushort)},
	{"GLushort*","NULL",sizeof(GLushort*)},

	{"GLfloat","0.0",sizeof(GLfloat)},
	{"GLfloat*","NULL",sizeof(GLfloat*)},

	{"GLdouble","0.0",sizeof(GLdouble)},
	{"GLdouble*","NULL",sizeof(GLdouble*)},

	{"GLsizei","0",sizeof(GLsizei)},
	{"GLsizei*","NULL",sizeof(GLsizei*)},

	{"GLclampf","0.0",sizeof(GLclampf)},
	{"GLclampf*","NULL",sizeof(GLclampf*)},

	{"GLclampd","0.0",sizeof(GLclampd)},
	{"GLclampd*","NULL",sizeof(GLclampd*)},

	{"GLhalfNV","0.0",sizeof(GLhalfNV)},
	{"GLhalfNV*","NULL",sizeof(GLhalfNV*)},

	{"GLbitfield", "0", sizeof(GLbitfield)},

	{"GLint64EXT*","NULL",sizeof(int64_t*)},

	{"GLuint64EXT*","NULL",sizeof(int64_t*)},

	{NULL,NULL,0},
};

%}


%token GLAPI2 APIENTRY2 MOT PO PF V CONST ETOILE PARAMSPE TAB
%start S

%%

S    : 	   LIGNE  { fprintf(fout_c,"\n{\n"); 
                     fprintf(fout_c,"\tint fnum=%d;\n",fnum);
                     fprintf(fout_c,"\tint fflags=0;\n");
                     fprintf(fout_c,"\tOUTPUT_FIFO(&fnum,sizeof(fnum));\n");
                     fprintf(fout_c,"\tOUTPUT_FIFO(&fflags,sizeof(fflags));\n");
                     for(i=0;i<=np;i++)
                       fprintf(fout_c,"\tOUTPUT_FIFO(&p%d,%d);\n",i,sizep[i]);
                     fprintf(fout_c,"\treturn %s;\n",ret_type);
                     fprintf(fout_c,"\n}\n\n");

                     fprintf(ftmpc,"\tglfunctable[%d](",fnum);
                     for(i=0;i<np;i++)
                        fprintf(ftmpc,"p%d,",i);
                     fprintf(ftmpc,"p%d);\n}\n\n",i);

                     np=-1;                                       
                     fnum++;                                                     } S
     |                                     
     ;


LIGNE : GLAPI2  RETURN APIENTRY2 MOT {fprintf(fout_c,"%s ",yytext);
                                      fprintf(fout_h,"%s ",yytext); 
                                      fprintf(fin_c,"glfunctable.a%s=glxGetProcAddress(\"%s\");\n",yytext,yytext);
                                      fprintf(ftmpc,"f%s()\n{\n",yytext);
                                      fprintf(fin_h,"\tf%s,\n",yytext);
                                      fprintf(ftmph,"\ta%s,\n",yytext);} PO {fprintf(fout_c,"( ");
                                                                           fprintf(fout_h,"( ");} PARAMS PF {
                                                                                                    fprintf(fout_c,")");
                                                                                                    fprintf(fout_h,");\n");} 
      ;         


RETURN : C MOT { fprintf(fout_c,"%s ",yytext);
                 fprintf(fout_h,"%s ",yytext);
                 strcpy(type,yytext);          } E {ret_type=type_return(type);}
       ;  

C : CONST {fprintf(fout_c,"const ");  fprintf(fout_h,"const ");}
  | 
  ;

E :  ETOILE {fprintf(fout_c,"*"); fprintf(fout_h,"*"); type=strcat(type,"*"); } 
  |  ETOILE  ETOILE {fprintf(fout_c,"**"); fprintf(fout_h,"**"); type=strcat(type,"**");}
  |
  ;


PARAMS : PARAM V {fprintf(fout_c,", "); fprintf(fout_h,", "); } PARAMS
       | PARAM
       ;

PARAM : C MOT {  fprintf(fout_c,"%s ",yytext);
                 if(strcmp(yytext,"void")!=0) 
                   fprintf(fout_h,"%s ",yytext);
                 strcpy(type,yytext);            } E F{ if(strcmp(type,"void")!=0)
                                                        {  
                                                         np++;
                                                         fprintf(fout_c,"p%d ",np); 
                                                         sizep[np]=type_size(type);
                                                       fprintf(ftmpc,"\t%s p%d==INPUT_FIFO(%d);\n",type,np,sizep[np]); }  }
 
      | PARAMSPE {  np++; fprintf(fout_c,"%s p%d ",yytext,np); fprintf(fout_h,"%s ",yytext); sizep[np]=4; }
      ;


F :  MOT
  |  MOT TAB {fprintf(fout_c,"*"); fprintf(fout_h,"*"); type=strcat(type,"*"); }
  |
  ;

%%

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
  ret_type=malloc(sizeof(char)*50);
  type=malloc(sizeof(char)*50);
  buffer=malloc(sizeof(char)*2048);

  fout_c=fopen(out_c_file,"wb");
  fout_h=fopen(out_h_file,"wb");
  fin_c=fopen(in_c_file,"wb");
  fin_h=fopen(in_h_file,"wb");
  ftmpc=fopen("tmpc","w+b");
  ftmph=fopen("tmph","w+b");

  fprintf(fout_c,"/* Auto-generated, do not edit ! */\n#include \""out_h_file"\"\n\n");
  fprintf(fout_h,"/* Auto-generated, do not edit ! */\n#include \"fifo.h\"\n#include <GL/gl.h>\n#include <GL/glext.h>\n\n");
  
  fprintf(fin_c,"/* Auto-generated, do not edit ! */\n#include \""in_h_file"\"\n\n");
  fprintf(fin_c,"void unpack( )\n{\tint func=INPUT_FIFO(4);\n\tint flags=INPUT_FIFO(4);\n\tvoid* f=functable[func];\n\tf();\n}\n\n");
  fprintf(fin_c,"void init()\n{\n");

  fprintf(fin_h,"/* Auto-generated, do not edit ! */\n#include \"fifo.h\"\n#include <GL/gl.h>\n#include <GL/glext.h>\n\n");
  fprintf(fin_h,"void* functable[]\n{\n");
  
  fprintf(ftmph,"void* glfunctable[]\n{\n");


  yyin=fopen("/usr/include/GL/gl.h","r");
  yyparse();

  yyin=fopen("/usr/include/GL/glext.h","r");
  yyparse();


  fprintf(fin_c,"}\n\n");
  fprintf(fin_h,"}\n\n");
  fprintf(ftmph,"}\n");

  
  rewind(ftmpc);
  rewind(ftmph);  

  

  while(fgets(buffer,2048,ftmpc)!=NULL)
    fputs(buffer,fin_c); 


  while(fgets(buffer,2048,ftmph)!=NULL)
    fputs(buffer,fin_h);

  fclose(fout_c);
  fclose(fout_h);
  fclose(fin_c);
  fclose(fin_h);
  fclose(ftmpc);
  fclose(ftmph);

  system("rm -f tmpc tmph");

  return 1;
}

int yyerror(char * s)
{
        printf("error:%s\n",yytext);
	return 0;
}