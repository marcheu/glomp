# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <semaphore.h>

#include"fifo.h"

extern int client_num;
extern int nbcarte;
extern int fenetreactive;
extern void **shmadr_fenetre1;
extern void **shmadr_fenetre2;
extern sem_t **semadrfen_in;
extern sem_t **semadrfen_out;
extern int width;
extern int height;
extern int * heightclient;


void * creershm_fenetre();
void lire_fenetre();
void ecirre_fenetre();
