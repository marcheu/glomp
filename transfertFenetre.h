#ifndef _TRANSFERTFENETRE_H_
#define _TRANSFERTFENETRE_H_

#include"init.h"


extern void **shmadr_fenetre1;
extern void **shmadr_fenetre2;

void * creershm_fenetre();
void lire_fenetre();
void ecrire_fenetre();
#endif
