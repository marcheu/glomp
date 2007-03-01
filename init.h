#ifndef _INIT_H_
#define _INIT_H_

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "fifo.h"



extern int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarte
extern int nbcarte;//nombre de GPU dispo

extern int width,height;//taille de la zone

extern void **shmadr_fenetre1;
extern void **shmadr_fenetre2;
extern sem_t **semadrfen_in;
extern sem_t **semadrfen_out;
extern int fenetreactive;


#endif

