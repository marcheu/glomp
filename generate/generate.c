#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "error.h"

#define tmpfile "funclist.txt"
#define out_c_file "wrapped.c"
#define out_h_file "wrapped.h"

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


char* type_return(char* type)
{
	int i=0;
	while(type_table[i].name!=NULL)
	{
		if (strcmp(type,type_table[i].name)==0)
			return type_table[i].default_return_value;
		i++;
	}
	BUG();
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
	BUG();
	return 0;
}


int main()
{
	system("cat /usr/include/GL/gl.h |grep APIENTRY|grep -v \"#\"> "tmpfile);
	system("cat /usr/include/GL/glext.h |grep APIENTRY|grep -v APIENTRYP|grep -v typedef |grep -v \"#\">> "tmpfile);
	system("sed -i -e s/APIENTRY\\ //g "tmpfile);
	system("sed -i -e s/WINGDIAPI\\ //g "tmpfile);
	system("sed -i -e s/GLAPI\\ //g "tmpfile);
	system("sed -i -e s/extern\\ //g "tmpfile);
	//system("sed -i -e s/const\\ //g "tmpfile);
	// stick the * to their matching type
	system("sed -i -e \"s/\\ \\*/\\*/g\" "tmpfile);
	// change "*" to "* "
	system("sed -i -e \"s/\\*/\\*\\ /g\" "tmpfile);
	// change "* *" to "**"
	system("sed -i -e \"s/\\*\\ \\*/\\*\\*/g\" "tmpfile);
	// replace double spaces with simple spaces
	system("sed -i -e \"s/\\ \\ /\\ /g\" "tmpfile);
	// replace double spaces with simple spaces
	system("sed -i -e \"s/\\ \\ /\\ /g\" "tmpfile);
	// stick the , to their matching type
	system("sed -i -e \"s/,\\ /,/g\" "tmpfile);
	// replace " )" with ")"
	system("sed -i -e \"s/\\ )/)/g\" "tmpfile);

	FILE* f;
	FILE* fout_c;
	FILE* fout_h;
	fout_c=fopen(out_c_file,"wb");
	fout_h=fopen(out_h_file,"wb");
	fprintf(fout_c,"/* Auto-generated, do not edit ! */\n#include \""out_h_file"\"\n\n");
	fprintf(fout_h,"/* Auto-generated, do not edit ! */\n#include \"fifo.h\"\n#include <GL/gl.h>\n#include <GL/glext.h>\n\n");

	char s[1024];
	char ret_type[1024];
	char func_name[1024];
	char params[20][1024];
	int param_count;
	int func_num=0;
	f=fopen(tmpfile,"rb");
	while(fgets(s,1024,f))
	{
		func_num++;

		// change the first \n to a 0
		int ii=0;
		for(ii=0;ii<1024;ii++)
			if (s[ii]=='\n')
				s[ii]=0;

		int i=0;

		/* handle possible "const" keyword */
		if (strncmp(&s[i],"const ",6)==0)
		{
			memcpy(&(ret_type[i]),&s[i],6);
			i+=6;
		}
		// find the return type
		do
		{
			ret_type[i]=s[i];
			i++;
		}
		while(s[i]!=' ');
		ret_type[i]=0;

		// jump to the next non blank char
		do
		{
			i++;
		}
		while(s[i]==' ');

		int j;
		j=0;
		// find the func name
		do
		{
			func_name[j]=s[i];
			i++;
			j++;
		}
		while((s[i]!=' ')&&(s[i]!='('));
		func_name[j]=0;

		printf("=============== Function %s\n",func_name);
		// if the function is overridden, don't "implement" it
		char cmd[1024];
		sprintf(cmd,"cat overrides.h |grep %s\n",func_name);
		int r=system(cmd);
		if (!r)
		{
			printf("%s overridden\n",func_name);
			continue;
		}


		// advance to the '('
		while(s[i]!='(')
		{
			i++;
		}

		// find the parameters
		// first, count how many params we have
		j=i;
		param_count=1;
		do
		{
			if (s[j]==',')
				param_count++;
			j++;
		}
		while(s[j]!=')');

		i++;

		int currentp=0;
		// now, store the parameter types
		// TODO special-case void here ?
		while(s[i]!=')')
		{
			j=0;
			/* handle possible "const" keyword */
			if (strncmp(&s[i],"const",5)==0)
			{
				memcpy(&(params[currentp][j]),&s[i],5);
				j+=5;
				i+=6;
				params[currentp][j]=' ';
				j++;
			}

			if (isalpha(s[i]))
			{
				while(isalnum(s[i]))
				{
					params[currentp][j]=s[i];
					i++;
					j++;
				}
				if (s[i]=='*')
				{
					params[currentp][j]=s[i];
					i++;
					j++;
				}
				if (s[i]=='*')
				{
					params[currentp][j]=s[i];
					i++;
					j++;
				}
				params[currentp][j]=0;
			}

			if (s[i]==' ')
				i++;

			// advance until next param, ignore param names
			while(isalnum(s[i]))
			{
				i++;
			}

			if (s[i]==',')
			{
				i++;
			}
			currentp++;
		}

		// generate the function proto, both to the .c and the .h file
		fprintf(fout_c,"%s %s ",ret_type,func_name);
		fprintf(fout_h,"%s %s ",ret_type,func_name);
		fprintf(fout_c," (");
		fprintf(fout_h," (");
		int p;
		for(p=0;p<param_count;p++)
		{
			if (strncmp(params[p],"void",4)==0)
				fprintf(fout_c,"%s",params[p]);
			else
				fprintf(fout_c,"%s p%d",params[p],p);
			fprintf(fout_h,"%s",params[p]);
			if (p<param_count-1)
			{
				fprintf(fout_c,", ");
				fprintf(fout_h,", ");
			}
		}
		fprintf(fout_c," )\n");
		fprintf(fout_h," );\n");
		// generate the function body
		fprintf(fout_c,"{\n");
		fprintf(fout_c,"\tint fnum=%d;\n",func_num);
		fprintf(fout_c,"\tint fflags=0;\n");
		fprintf(fout_c,"\tOUTPUT_FIFO(&fnum,sizeof(fnum));\n");
		fprintf(fout_c,"\tOUTPUT_FIFO(&fflags,sizeof(fflags));\n");
		for(p=0;p<param_count;p++)
		{
			if (strncmp(params[p],"void",4)!=0)
				fprintf(fout_c,"\tOUTPUT_FIFO(&p%d,%d);\n",p,type_size(params[p]));
		}
		// generate the function return
		fprintf(fout_c,"\treturn %s;\n",type_return(ret_type));

		fprintf(fout_c,"}\n\n");

	}
	fclose(f);
	fclose(fout_c);
	fclose(fout_h);

	system("rm -f "tmpfile);
	return EXIT_SUCCESS;
}

