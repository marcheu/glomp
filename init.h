#ifndef _INIT_H_
#define _INIT_H_

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>

#include "fifo.h"

#define DEBUG 0


extern int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarte
extern int nbcarte;//nombre de GPU dispo





extern int fenetreactive;


#endif

