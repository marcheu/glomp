#ifndef _TRANSFERTFENETRE_H_
#define _TRANSFERTFENETRE_H_

#include"init.h"
#include"overrides.h"
#include<stdlib.h>
#include<semaphore.h>

extern void (*lib_glRasterPos2i)( GLint x,GLint y );

extern int* client_load;

extern void **shmadr_fenetre1;
extern void **shmadr_fenetre2;

//extern sem_t **semadrfen_in;
//extern sem_t **semadrfen_out;

void * creershm_fenetre();
void lire_fenetre();
void ecrire_fenetre();



#endif
