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

#include "overrides.h"
#include "glheader.h"

const GLubyte* glGetString( GLenum name )
{
	const GLubyte* vendor="http://glomp.sf.net";
 	const GLubyte* renderer="Glomp on ";
	static GLubyte return_renderer[4096];
	static GLubyte return_version[128];
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
			fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
			fifo_output(&cmd_fifo,&name,4);
			fifo_flush(&cmd_fifo);
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
		{
			/* find the lowest version among the cards */
			int fnum=OVERRIDE_BASE+43;
			int c;
			int major,minor;
			fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
			fifo_output(&cmd_fifo,&name,4);
			fifo_flush(&cmd_fifo);
			const GLubyte* version=lib_glGetString(name);
			int r=sscanf(version,"%d.%d",&major,&minor);
			if (r!=2)
			{
				printf("unparseable GL version !\n");
				major=1;
				minor=0;
			}

			for(c=1;c<nbcarte;c++)
			{
				int mymaj, mymin;
				sem_wait(&semadr[c]);
				int r=sscanf(shmadr+4096*c,"%d.%d",&mymaj,&mymin);
				if (r!=2)
				{
					printf("unparseable GL version !\n");
					continue;
				}
				if (mymaj*1000+mymin<major*1000+minor)
				{
					major=mymaj;
					minor=mymin;
				}
			}
			sprintf(return_version,"%d.%d Glomp",major,minor);
			return return_version;
		}
		case GL_EXTENSIONS:
		{
			/* blacklist extensions that can't be auto genereted and aren't written yet */
			char* blacklisted_extensions = "GL_ARB_vertex_buffer_object GL_EXT_framebuffer_object";
			/* we have different cards, but need to expose a single extension string
			 * so we query each card for its extensions and intersect */
			int fnum=OVERRIDE_BASE+43;
			int i,j,c;
			fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
			fifo_output(&cmd_fifo,&name,4);
			fifo_flush(&cmd_fifo);
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
	fifo_input(&cmd_fifo,&name,4);
	const char* s=(const char*)lib_glGetString(name);
	strcpy(shmadr+4096*client_num,s);
	sem_post(&semadr[client_num]);
}


