#ifndef _SEGMENT_H_
#define _SEGMENT_H_

extern char* shmadr;
extern sem_t * semadr;

// creer le segment et ecrit dans la fifo
int segment_create(char* p,int size);

// recupere un segment dans la fifo et l'attache
char * segment_attach();

// detruit tous les segments
void segment_delete();


#endif

