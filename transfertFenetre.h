#ifndef _TRANSFERTFENETRE_H_
#define _TRANSFERTFENETRE_H_

#include"init.h"
#include"overrides.h"
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include <fcntl.h>

extern void (*lib_glRasterPos2f)( GLfloat x,GLfloat y );
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

extern void **shmadr_fenetre1;
extern void **shmadr_fenetre2;
//extern int * tabKey1;
//extern int * tabKey2;


/*procedure de creation des semaphores*/
extern void GLOMPcreateSem();



extern void * creershm_fenetre();
extern void lire_fenetre();
extern void ecrire_fenetre();

void createAllFen();
extern void GLOMPinitTabKey();



#endif
