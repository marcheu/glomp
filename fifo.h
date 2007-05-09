#ifndef _FIFO_H_
#define _FIFO_H_


#include <sys/msg.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>
#include "init.h"

#define FIFO_SIZE (128*1024)
#define PACKET_SIZE 4080

typedef struct fifo
{
	int idx;
	int size;
	struct {
		long mtype;
		char mtext[PACKET_SIZE];
	} msgbuf;
	int* message_queue;	// sysV message queue id
}
fifo;

/*variable globale au programme la fifo de commande de GLOMP :  GLOMPcmd_fifo*/
extern fifo cmd_fifo;

extern void fifo_init(fifo* f);
extern void fifo_output(fifo* f,const void * buffer, int size);
extern void fifo_input(fifo* f,void * buffer,int size);
extern void fifo_flush(fifo* f);
    
#endif

