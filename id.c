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
  if (value>=id_table_size)
    {
      printf("oops id %d not found, table size %d  (bug in the application ?)\n",value,id_table_size);
      return 0;
    }
  return id_table[value];
}

// generates an id for the server
int id_server_generate(id_type type)
{
  id_server_table_size++;
  id_server_table=(id_type*)realloc(id_server_table,id_server_table_size*sizeof(id_type));
  id_server_table[id_server_table_size-1]=type;
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

