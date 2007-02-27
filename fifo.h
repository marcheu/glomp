#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include "init.h"



int j;//compteur pour la macro

void fifo_init();
void fifo_outpout(void *, int);
void fifo_input(void *,int);
    
#endif
