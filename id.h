#ifndef _ID_H_
#define _ID_H_

#include <GL/gl.h>

typedef enum
{
	id_invalid=0,
	id_texture,
	id_list,
	id_program,
	id_occlusion_query,
	id_buffer,
	id_fence,
	id_fragment
}id_type;

// adds an id to the table
void id_add(int value,int translated_value);
// translates an id
int id_translate(int value);
// generates an id
int id_server_generate(id_type type);
// tests if the id is of a given type
GLboolean id_server_test_type(GLuint id,id_type type);

#endif

