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

// creer le segment et ecrit dans la fifo
extern void segment_create(char* p,int size);

// recupere un segment dans la fifo et l'attache
extern char * segment_attach();

// detruit tous les segments
extern void segment_delete();

/*procedure de creation du segment de retour*/
extern int GLOMPsegment_create_retour();


#endif

