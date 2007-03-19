#ifndef _TRANSFERTFENETRE_H_
#define _TRANSFERTFENETRE_H_

#include"init.h"
#include"overrides.h"
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include <fcntl.h>

extern void (*lib_glRasterPos2i)( GLint x,GLint y );
extern void (*lib_glDrawPixels)( GLsizei width,
				GLsizei height,
				GLenum format,
				GLenum type,
				const GLvoid *pixels );
extern void (*lib_glReadPixels)( GLint x,
				GLint y,
				GLsizei width,
				GLsizei height,
				GLenum format,
				GLenum type,
				GLvoid *pixels );


extern int* client_load;

//extern void **shmadr_fenetre1;
//extern void **shmadr_fenetre2;

//extern sem_t **semadrfen_in;
//extern sem_t **semadrfen_out;

extern void * creershm_fenetre();
extern void lire_fenetre();
extern void ecrire_fenetre();



#endif
