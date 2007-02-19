# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <semaphore.h>

#define W 1024
#define H 768


extern int client_num;
extern int numero_proc;
extern int fenetreactive;
extern char **shmadr_fenetre1;
extern char **shmadr_fenetre2;
extern sem_t **semadrfen_in;
extern sem_t **semadrfen_out;


char * creershm_fenetre();
void lire_fenetre();
void ecirre_fenetre();
