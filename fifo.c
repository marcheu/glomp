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

#include "fifo.h"


extern int errno;

/*variable gobale au prog cf fifo.h*/
fifo cmd_fifo;

/* initialize the fifo */
void fifo_init(fifo* f)
{
	int i;
	f->message_queue=(int*)malloc(sizeof(int)*nbcarte);

	f->msgbuf.mtype=1;
	for(i=1;i<nbcarte;i++)
	{
		struct msqid_ds buf;
		f->message_queue[i]=msgget(IPC_PRIVATE,0666);
		if(f->message_queue[i]==-1)
		{
			printf("ERROR trop de file de message\n");
			exit(0);
		}

		msgctl(f->message_queue[i], IPC_STAT, &buf);
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
	h[0]=f->idx+1;//+1 pour lire la derniere commande XXX +4 ?
	// fire
	for(i=1;i<nbcarte;i++)
	{
		if(msgsnd(f->message_queue[i],&(f->msgbuf),f->idx,0)==-1)
			printf("ERROR msgsnd %d\n",errno);

	}

	f->idx=4;
}


/* outputs data to the fifo from the master process */
void fifo_output(fifo* f,const void* data, int size)
{
	if (f->idx+size>PACKET_SIZE)
		fifo_flush(f);  

	memcpy(&(f->msgbuf.mtext[f->idx]),data,size);
	f->idx+=size;

	if (f->idx==PACKET_SIZE)
		fifo_flush(f);  
}

/* reads data from the fifo */
void fifo_input(fifo* f,void* data,int size )
{ 
	if (f->idx+size>=f->size)
	{
		uint32_t* h=(uint32_t*)f->msgbuf.mtext;
		msgrcv(f->message_queue[client_num],&(f->msgbuf),PACKET_SIZE,0,0);

		f->size=h[0];
		f->idx=4;
	}
	memcpy(data,&(f->msgbuf.mtext[f->idx]),size);
	f->idx+=size;
}

