
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern uint8_t* cmd_fifo;
extern uint32_t cmd_fifo_idx;

#define OUTPUT_FIFO(A,S) \
	do{\
	/* here check there is enough room or wait or rewind or... */\
	memcpy(&cmd_fifo[cmd_fifo_idx],A,S);\
	cmd_fifo_idx+=S;\
	}while(0)


