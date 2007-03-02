
#include <stdio.h>
#include <stdlib.h>
#include "id.h"

static int* id_table=NULL;
static id_table_size=1;

// adds an id to the table
void id_add(int value,int translated_value)
{
	id_table_size++;
	id_table=(int*)realloc(id_table,id_table_size*sizeof(int));
	if (value!=id_table_size-1)
		printf("oops non-ordered additions\n");
	id_table[id_table_size-1]=translated_value;
}

// translates an id
int id_translate(int value)
{
	if (id_table_size>value)
	{
		printf("oops id not found (bug in the application ?)\n");
		return 0;
	}
	return id_table[value];
}

// generates an id for the server
int id_server_generate()
{
	static int current=0;
	current++;	// start at 1
	return current;
}


