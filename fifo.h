
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define TAILLEMEM 128*1024
 
extern uint8_t* cmd_fifo;
extern uint8_t* buffer;
extern uint32_t cmd_fifo_idx;
extern uint32_t idx; //indice client

void creerFifo();


#define OUTPUT_FIFO(A,S) \
	do{\
	/* here check there is enough room or wait or rewind or... */\
	memcpy(&cmd_fifo[cmd_fifo_idx],A,S);\
	cmd_fifo_idx=(cmd_fifo_idx+S)%TAILLEMEM;\
	}while(0)

#define INPUT_FIFO(S) \
	do{\
	/* here check there is enough room or wait or rewind or... */\
	memcpy(&buffer,cmd_fifo[idx],S);\
	idx=(idx+S)%TAILLEMEM;\
	}while(0)


