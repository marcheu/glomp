#ifndef _SEGMENT_H_
#define _SEGMENT_H_
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "segment.h"
#include "init.h"
char* shmadr;
extern sem_t * semadr;
extern char * semadrShm;

// creates a segment and outputs it to the fifo
extern void segment_create(char* p,int size);

// fetches a segment from the fifo and attaches it
extern char * segment_attach();

// deletes all segments
extern void segment_delete();

// creates a static segment
extern void segment_create_static();


#endif

