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

typedef struct GLOMPfifo
{
	int idx;
	int size;
	struct {
		long mtype;
		char mtext[PACKET_SIZE];
	} msgbuf;
	int* message_queue;	// sysV message queue id
}
GLOMPfifo;

/*variable globale au programme la fifo de commande de GLOMP :  GLOMPcmd_fifo*/
extern GLOMPfifo GLOMPcmd_fifo;

extern void GLOMPfifo_init(GLOMPfifo* f);
extern void fifo_output(GLOMPfifo* f,const void * buffer, int size);
extern void fifo_input(GLOMPfifo* f,void * buffer,int size);
extern void fifo_flush(GLOMPfifo* f);
    
#endif

