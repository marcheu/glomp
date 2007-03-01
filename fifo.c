#include "fifo.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

fifo cmd_fifo;

/* initialize the fifo */
void fifo_init(fifo* f)
{
	int i;
	f->message_queue=(int*)malloc(sizeof(int)*nbcarte);

	for(i=0;i<nbcarte;i++)
	{
		struct msqid_ds buf;
		f->message_queue[i]=msgget(IPC_PRIVATE,0666);
		msgctl(f->message_queue[i], IPC_STAT, &buf);
		printf("queue size before %d bytes adjusting to %d\n",(int)buf.msg_qbytes,FIFO_SIZE);
		buf.msg_qbytes=FIFO_SIZE;
		msgctl(f->message_queue[i], IPC_SET, &buf);
	}
	f->idx=4;
	f->size=0;
}

/* master process flushes what remains in the fifo */
void fifo_flush(fifo* f)
{
	int i;
	// add the size in front
	uint32_t* h=(uint32_t*)f->msgbuf.mtext;
	h[0]=f->idx;

	// fire
	for(i=0;i<nbcarte;i++)
		msgsnd(f->message_queue[i],&(f->msgbuf),f->idx,0);
	f->idx=4;
}


/* outputs data to the fifo from the master process */
void fifo_outpout(fifo* f,const void* data, int size)
{
	if (f->idx+size>PACKET_SIZE)
		fifo_flush(f);

	memcpy(&(f->msgbuf.mtext),data,size);
	f->idx+=size;

	if (f->idx==PACKET_SIZE)
		fifo_flush(f);
}

/* reads data from the fifo */
void fifo_input(fifo* f,void* data,int size )
{
	if (f->idx+size<=f->size)
	{
		uint32_t* h=(uint32_t*)f->msgbuf.mtext;
		msgrcv(f->message_queue[client_num],&(f->msgbuf),PACKET_SIZE,0,0);
		f->size=h[0];
		f->idx=4;
	}
	memcpy(data,&(f->msgbuf.mtext[f->idx]),size);
	f->idx+=size;
}



