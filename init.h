#ifndef _INIT_H_
#define _INIT_H_

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include "pbuffer.h"
#include "client.h"
#include "server.h"
#include "overrides.h"
#include "transfertFenetre.h"
#include "fifo.h"

#define DEBUG 0

 

/**********************************
variable globale a tous le programe
************************************/

/*les fils de 1 a nbcarte-1, le pere = 0
numeros du client, va nous permettre de selectionner les processus*/
extern int client_num;
extern int nbcarte;/*nombre de GPU dispo*/
extern int fenetreactive;/*choix de la fenetre courante*/

extern sem_t * semadrfen_in;/*semaphore de protection de la fifo*/
extern sem_t * semadrfen_out;

extern int * client_load;/*sert a calculer la charge de travaille de chaque GPU,chaque case du tab represente un pourcentage de calcule attribué à la carte (pas encore utiliser reellemet, toutes les cartes ont la meme chages)*/


extern int width,height;/*taille de la zone*/
#endif

