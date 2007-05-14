
/* vertex array handling code
 *
 * Since vertex arrays cannot be migrated (there is no locking/unlocking
 * on them, so no way to ensure they don't get modified in between)
 * we have to resend the vertex data each time. We do that by sending  
 * the relevant array pieces, which is about as good as it can get.
 *
 * Shortcomings :
 * - ClientActiveTextureARB not handled, so arrays don't work with multitextures (breaks q3 lightmaps)
 * - ArrayElement not handled
 * - generic attributes not handled
 * - we screw the client opengl state a bit, but I can't see a scenario where that's an issue
 */


#include <stdio.h>
#include "glheader.h"
#include "overrides.h"
#include "lib_funcs.h"

#define MAX_ARRAY 16

typedef struct array
{
	GLsizei type;
	int32_t size;
	int32_t stride;
	const unsigned char* pointer;
	GLboolean enabled;
} array;

static int current_tex=0;
static array arrays[MAX_ARRAY];

void array_init()
{
	int i;
	for(i=0;i<MAX_ARRAY;i++)
		arrays[i].enabled=GL_FALSE;
	current_tex=0;
}

// we need those two to keep track of the current texture array
void glClientActiveTexture (GLenum texture)
{
	current_tex=texture;
}

void glClientActiveTextureARB (GLenum texture)
{
	current_tex=texture;
}

// renders a single element
/*
static inline void render_element(GLint i)
{
	void firefunc[MAX_ARRAY][]=
	{
		NULL,	NULL,			glVertex2fv,	glVertex3fv,		glVertex4fv,
		NULL,	glVertexWeightfvEXT,	NULL,		NULL,			NULL,
		NULL,	NULL,			NULL,		glNormal3fv,		NULL,
		NULL,	NULL,			NULL,		glColor3fv,		glColor4fv,
		NULL,	NULL,			NULL,		glSecondaryColor3fv,	NULL,
		NULL,	glFogCoordfvEXT,	NULL,		NULL,			NULL,
		NULL,	glIndexfv,		NULL,		NULL,			NULL,
		NULL,	glEdgeFlagv,		NULL,		NULL,			NULL,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
		NULL,	glTexCoord1fv,		glTexCoord2fv,	glTexCoord3fv,		glTexCoord4fv,
	}

		glVertexAttrib1sv,glVertexAttrib2sv,glVertexAttrib3sv,glVertexAttrib4sv,
		glVertexAttrib1fv,glVertexAttrib2fv,glVertexAttrib3fv,glVertexAttrib4fv,
		glVertexAttrib1dv,glVertexAttrib2dv,glVertexAttrib3dv,glVertexAttrib4dv,
	int j;
	for(j=MAX_ARRAY-1;j>=0;j--)
	{
		if (arrays[j].enabled)
		{
			GLfloat params[4];
			// convert parameters
			for(k=0;k<arrays[j].size;k++)
			{
				switch(arrays[j].type)
				case GL_FLOAT:
				case GL_UNSIGNED_INT:
				case GL_UNSIGNED_SHORT:
				case GL_UNSIGNED_BYTE:
			}
			// fire
			firefunc[j][arrays[j].size](params);
			
		}
	}
}
*/

static inline GLenum array_name(GLint array_number)
{
	GLenum name[]={
		GL_VERTEX_ARRAY,
		GL_WEIGHT_ARRAY_ARB,
		GL_NORMAL_ARRAY,
		GL_COLOR_ARRAY,
		GL_SECONDARY_COLOR_ARRAY,
		GL_FOG_COORDINATE_ARRAY,
		GL_INDEX_ARRAY,
		GL_EDGE_FLAG_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
		GL_TEXTURE_COORD_ARRAY,
	};
	if ((array_number<0)||(array_number>=MAX_ARRAY))
		printf("oops bug\n");
	return name[array_number];
}

static inline GLint array_number(GLenum array_name)
{
	switch(array_name)
	{
		case GL_VERTEX_ARRAY:		return 0;
		case GL_WEIGHT_ARRAY_ARB:	return 1;
		case GL_NORMAL_ARRAY:		return 2;
		case GL_COLOR_ARRAY:		return 3;
		case GL_SECONDARY_COLOR_ARRAY:	return 4;
		case GL_FOG_COORDINATE_ARRAY:	return 5;
		case GL_INDEX_ARRAY:		return 6;
		case GL_EDGE_FLAG_ARRAY:	return 7;
		case GL_TEXTURE_COORD_ARRAY:	return 8+current_tex;
	}
	printf("oops unkown array name 0x%x\n",array_name);
	return 0;
}


static inline void output_array_begin(int nbverts,GLenum mode)
{
	int fnum=OVERRIDE_BASE+48;
	fifo_output(&cmd_fifo,&fnum,4);
	fifo_output(&cmd_fifo,&nbverts,4);
	fifo_output(&cmd_fifo,&mode,4);
}

static inline void output_array_draw_elements(
		int array_num,		// array we're outputting
		GLsizei count, 		// number of indices
		GLenum type,		// type of the indices
		const GLvoid* indices 	// index list
		)
{
	int i;
	int component_size=arrays[array_num].size*sizeGLenum(arrays[array_num].type)/8;
	int stride=arrays[array_num].stride;
	const unsigned char* data=NULL;
	int index=0;
	int jump=component_size;
	if (jump<stride)
		jump=stride;

	fifo_output(&cmd_fifo,&array_num,4);
	fifo_output(&cmd_fifo,&arrays[array_num].type,4);
	fifo_output(&cmd_fifo,&arrays[array_num].size,4);
	switch(type)
	{
		case GL_UNSIGNED_BYTE:
		{
			GLubyte* indicesb=(GLubyte*)indices;
			for(i=0;i<count;i++)
			{
				index=indicesb[i];
				data=arrays[array_num].pointer+index*jump;
				fifo_output(&cmd_fifo,data,component_size);
			}
			break;
		}
		case GL_UNSIGNED_SHORT:
		{
			GLushort* indicess=(GLushort*)indices;
			for(i=0;i<count;i++)
			{
				index=indicess[i];
				data=arrays[array_num].pointer+index*jump;
				fifo_output(&cmd_fifo,data,component_size);
			}
			break;
		}
		case GL_UNSIGNED_INT:
		{
			GLuint* indicesi=(GLuint*)indices;
			for(i=0;i<count;i++)
			{
				index=indicesi[i];
				data=arrays[array_num].pointer+index*jump;
				fifo_output(&cmd_fifo,data,component_size);
			}
			break;
		}
	}
}

static inline void output_array_draw_arrays(
		int array_num,	// array we're outputting
		GLint first,	// first element we're outputting
		GLsizei count	// number of elements we should output
		)
{
	int i;
	int component_size=arrays[array_num].size*sizeGLenum(arrays[array_num].type)/8;
	int stride=arrays[array_num].stride;
	const unsigned char* data;
	int jump=component_size;

	if (jump<stride)
		jump=stride;
	
	data=arrays[array_num].pointer+first*jump;

	fifo_output(&cmd_fifo,&array_num,4);
	fifo_output(&cmd_fifo,&arrays[array_num].type,4);
	fifo_output(&cmd_fifo,&arrays[array_num].size,4);
	for(i=first;i<first+count;i++)
	{
		fifo_output(&cmd_fifo,data,component_size);
		data+=component_size+jump;
	}
}

static inline void output_array_end()
{
	int end=MAX_ARRAY;
	fifo_output(&cmd_fifo,&end,4);
}

static void call_vertex_array(int array_num,GLenum mode,GLenum type,GLsizei size,unsigned char* data)
{
	// now enable this array
	if (array_num>=8)
		lib_glClientActiveTexture(GL_TEXTURE0+(array_num-8));	// FIXME here we screw the opengl state !!!
	lib_glEnableClientState(array_name(array_num));

	// and give opengl the array
	switch(array_num)
	{
		case 0:lib_glVertexPointer(size,type,0,data);break;
		case 1:lib_glWeightPointerARB(size,type,0,data);break;
		case 2:lib_glNormalPointer(type,0,data);break;
		case 3:lib_glColorPointer(size,type,0,data);break;
		case 4:lib_glSecondaryColorPointer(size,type,0,data);break;
		case 5:lib_glFogCoordPointer(type,0,data);break;
		case 6:lib_glIndexPointer(type,0,data);break;
		case 7:lib_glEdgeFlagPointer(0,data);break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		       lib_glTexCoordPointer(size,type,0,data);break;
	}
}

void GLOMPdraw_array()
{
	int array_enabled[MAX_ARRAY];
	unsigned char* data[MAX_ARRAY];
	int i=0;
	int vtx_count;
	GLenum mode;

	for(i=0;i<MAX_ARRAY;i++)
		array_enabled[i]=0;

	// read the fifo data
	fifo_input(&cmd_fifo,&vtx_count,4);
	fifo_input(&cmd_fifo,&mode,4);

	// read the arrays
	int r;
	fifo_input(&cmd_fifo,&r,4);

	while(r<MAX_ARRAY)
	{
		int type,size;

		array_enabled[r]=1;
		fifo_input(&cmd_fifo,&type,4);
		fifo_input(&cmd_fifo,&size,4);
		int component_size=size*sizeGLenum(type)/8;
		data[r]=(unsigned char*)malloc(component_size*vtx_count);
		// get the data from the fifo
		for(i=0;i<vtx_count;i++)
			fifo_input(&cmd_fifo,data[r]+i*component_size,component_size);

		// and point opengl to the array data
		call_vertex_array(r,mode,type,size,data[r]);

		fifo_input(&cmd_fifo,&r,4);
	}

	// draw !
	lib_glDrawArrays(mode,0,vtx_count);

	// free the data
	for(i=0;i<MAX_ARRAY;i++)
		if (array_enabled[i])
			free(data[i]);

	// disable used arrays
	for(i=0;i<MAX_ARRAY;i++)
	{
		if (array_enabled[i])
		{
			// FIXME here we screw the opengl state !!!
			if (i>=8)
				lib_glClientActiveTexture(GL_TEXTURE0+(i-8));
			lib_glDisableClientState(array_name(i));
		}
	}
}

void glEnableClientState(GLenum array)
{
	if(DEBUG){printf("serveur glEnableClientState\n");}
	arrays[array_number(array)].enabled=GL_TRUE;
	lib_glEnableClientState(array);
}
void glEnableClientStateARB(GLenum array)
{
	if(DEBUG){printf("serveur glEnableClientStateARB\n");}
	arrays[array_number(array)].enabled=GL_TRUE;
	lib_glEnableClientState(array);
}

void glDisableClientState(GLenum array)
{
	if(DEBUG){printf("serveur glDisableClientState\n");}
	arrays[array_number(array)].enabled=GL_FALSE;
	lib_glDisableClientState(array);
}
void glDisableClientStateARB(GLenum array)
{
	if(DEBUG){printf("serveur glDisableClientStateARB\n");}
	arrays[array_number(array)].enabled=GL_FALSE;
	lib_glDisableClientState(array);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	if(DEBUG){printf("serveur glDrawElements\n");}
	int i;
	output_array_begin(count,mode);
	for(i=0;i<MAX_ARRAY;i++)
		if (arrays[i].enabled)
			output_array_draw_elements(i,count,type,indices);
	output_array_end();
	lib_glDrawElements(mode,count,type,indices);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	if(DEBUG){printf("serveur glDrawArrays\n");}
	int i;
	output_array_begin(count,mode);
	for(i=0;i<MAX_ARRAY;i++)
		if (arrays[i].enabled)
		{
			output_array_draw_arrays(i,first,count);
		}
	output_array_end();
	lib_glDrawArrays(mode,first,count);
}
void glDrawArraysEXT (GLenum mode, GLint first, GLsizei count)
{
	glDrawArrays(mode,first,count);
}

void glArrayElement(GLint i)
{
	if(DEBUG){printf("serveur glArrayElement\n");}
//	render_element(i);
}
void glArrayElementEXT (GLint i)
{
	glArrayElement(i);
}

// updates an array atributes
static inline void update_array(int array_num,GLsizei type,GLint size,GLsizei stride,const GLvoid* pointer)
{
	arrays[array_num].type=type;
	arrays[array_num].size=size;
	arrays[array_num].stride=stride;
	arrays[array_num].pointer=pointer;
}


// Attrib 0
void glVertexPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
	update_array(0,type,size,stride,pointer);
	lib_glVertexPointer(size,type,stride,pointer);
}
void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	update_array(0,type,size,stride,pointer);
	lib_glVertexPointer(size,type,stride,pointer);
}

// Attrib 1
void glVertexWeightPointerEXT (GLsizei size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(1,type,size,stride,pointer);
	lib_glWeightPointerARB(size,type,stride,pointer);
}
void glWeightPointerARB (GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(1,type,size,stride,pointer);
	lib_glWeightPointerARB(size,type,stride,pointer);
}

// Attrib 2
void glNormalPointerEXT (GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
	update_array(2,type,3,stride,pointer);
	lib_glNormalPointer(type,stride,pointer);
}
void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
	update_array(2,type,3,stride,pointer);
	lib_glNormalPointer(type,stride,pointer);
}

// Attrib 3
void glColorPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
	update_array(3,type,size,stride,pointer);
	lib_glColorPointer(size,type,stride,pointer);
}
void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	update_array(3,type,size,stride,pointer);
	lib_glColorPointer(size,type,stride,pointer);
}

// Attrib 4
void glSecondaryColorPointerEXT (GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(4,type,size,stride,pointer);
	lib_glSecondaryColorPointer(size,type,stride,pointer);
}
void glSecondaryColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(4,type,size,stride,pointer);
	lib_glSecondaryColorPointer(size,type,stride,pointer);
}

// Attrib 5
void glFogCoordPointerEXT (GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(5,type,1,stride,pointer);
	lib_glFogCoordPointer(type,stride,pointer);
}
void glFogCoordPointer (GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(5,type,1,stride,pointer);
	lib_glFogCoordPointer(type,stride,pointer);
}

// Attrib 6
void glIndexPointerEXT (GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
	update_array(6,type,1,stride,pointer);
	lib_glIndexPointer(type,stride,pointer);
}
void glIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
	update_array(6,type,1,stride,pointer);
	lib_glIndexPointer(type,stride,pointer);
}

// Attrib 7
void glEdgeFlagPointerEXT (GLsizei stride, GLsizei count, const GLboolean * pointer)
{
	update_array(7,GL_UNSIGNED_BYTE,1,stride,pointer);
	lib_glEdgeFlagPointer(stride,pointer);
}
void glEdgeFlagPointer (GLsizei stride, const GLvoid *pointer)
{
	update_array(7,GL_UNSIGNED_BYTE,1,stride,pointer);
	lib_glEdgeFlagPointer(stride,pointer);
}

// Attrib 8+n
void glTexCoordPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
	update_array(8+current_tex,type,size,stride,pointer);
	lib_glTexCoordPointer(size,type,stride,pointer);
}
void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	update_array(8+current_tex,type,size,stride,pointer);
	lib_glTexCoordPointer(size,type,stride,pointer);
}

// Attrib "index"
void glVertexAttribPointerNV (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	update_array(index,type,size,stride,pointer);
	printf("Generic vertex Attributes not working\n");
}
void glVertexAttribPointerARB (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
	update_array(index,type,size,stride,pointer);
	printf("Generic vertex Attributes not working\n");
}
void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
	update_array(index,type,size,stride,pointer);
	printf("Generic vertex Attributes not working\n");
}



