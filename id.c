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
#include <stdlib.h>
#include "id.h"
#include "types.h"

static int* id_table=NULL;
static id_table_size=1;

static id_type* id_server_table=NULL;
static id_server_table_size=1;

// adds an id to the table
void id_add(int value,int translated_value)
{
  id_table_size++;
  id_table=(int*)realloc(id_table,id_table_size*sizeof(int));
  if (value!=id_table_size-1)
    printf("oops non-ordered additions (%d,%d)\n",value,id_table_size-1);
  id_table[id_table_size-1]=translated_value;
}

// translates an id
int id_translate(int value)
{
	if ((id_table==NULL)||(value>=id_table_size))
	{
		// oops we didn't find the id in the table
		// that probably means the application doesn't use glGen*
		// in which case we don't need to handle the ids anyway
		return value;
	}
	return id_table[value];
}

// generates an id for the server
int id_server_generate(id_type type)
{
  id_server_table_size++;
  id_server_table=(id_type*)realloc(id_server_table,id_server_table_size*sizeof(id_type));
  id_server_table[id_server_table_size-1]=type;
  printf("id:generated %d\n",id_server_table_size-1);
  return id_server_table_size-1; //la aussi !!
}

GLboolean id_server_test_type(GLuint id,id_type type)
{
  // out of bounds means no such object
  if (id>=id_server_table_size)
    return GL_FALSE;
  // otherwise it has to match what we recorded in the table
  return (id_server_table[id]==type);
}

