#ifndef _SEGMENT_H_
#define _SEGMENT_H_
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "segment.h"
#include "init.h"
char* shmadr;
sem_t * semadr;

// creer le segment et ecrit dans la fifo
extern int segment_create(char* p,int size);

// recupere un segment dans la fifo et l'attache
extern char * segment_attach();

// detruit tous les segments
extern void segment_delete();


#endif

