#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "fifo.h"

#define TAILLEMEM 128*1024


//la structure qui contient le shm servant de fifo
//dans le .h pour qu'elle soit global
extern uint8_t* cmd_fifo;//le shm en lui meme
extern uint32_t cmd_fifo_idx;//indice du pere
extern uint32_t idx; //tableau des indice client

extern int client_num;//numeros du client, va nous permettre de selectionner les processus
//les fils de 0 a nbcarte-1, le pere = nbcarre
extern int nbcarte;//nombre de GPU dispo

extern int width,height;//taille de la zone
extern int * heightclient;

extern void **shmadr_fenetre1;
extern void **shmadr_fenetre2;
extern sem_t **semadrfen_in;
extern sem_t **semadrfen_out;
extern int fenetreactive;

extern GLuint *tabtext;//le tableau des etxtures
extern GLuint *shm_text_client;//un shm pour le tableau des textures
extern pthread_mutex_t *mutex;//le mutex qui le protege
extern int tailletabtext;

extern sem_t **semap_in;
extern sem_t **semap_out;//les semaphore pour proteger la fifo

