/*
 *   GLOMP - transparent multipipe OpenGL
 *   Copyright (C) 2007 the GLOMP team (see AUTHORS)
 *   
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "init.h"
#include "lib_funcs.h"
#include "tile_screen.h"
#include "multi_screen.h"
#include "array.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>


/********variable global a tous le prog (cf init.h)*************/
int client_num;
int nbcarte;

sem_t * semadrfen_in;
sem_t * semadrfen_out;

int * client_load;


int width,height;/*taille de la fenetre de l'appli*/
/****************************************************************/

extern void initPointers();


/*lance la boucle principale des clients qui vont desencapsuler les focntions
de la fifo avec client_unpack*/
static void GLOMPclient_run()
{
	while(1){
		client_unpack();
	}
}


/*initialise tout le programme
se lance avant le main du prog OpenGL
creer la fifo, les pointeurs de fonctions, ...*/
void GLOMP_init()
{
	static int init=0;
	if (init==1)
		return;

	init=1;

	printf("Initializing GLOMP\n");

	int i;
	Display* dpy;
	char* force;

	/* get all useful function pointers by dlsym() */
	lib_funcs_init();

	/* count screens */
	dpy = lib_XOpenDisplay("");
	nbcarte=ScreenCount(dpy);
	lib_XCloseDisplay(dpy);
	nbcarte=2;
	printf("  - found %d GPUs\n",nbcarte);
	/*potentially override the number of GPUs*/
	force=getenv("FORCE_GPU");
	if (force)
	{
		printf("  - forcing %d GPUs\n",atoi(force));
		nbcarte=atoi(force);
	}
	printf("\n");

	// init the screen tiling
	switch(nbcarte)
	{
		case 4:tile_screen_init(2,2);break;
		case 6:tile_screen_init(3,2);break;
		case 8:tile_screen_init(4,2);break;
		case 9:tile_screen_init(3,3);break;
		case 10:tile_screen_init(5,2);break;
		case 12:tile_screen_init(4,3);break;
		default:tile_screen_init(nbcarte,1);break;
	}

	// init our static segment
	segment_create_static();

	/*initilisation (bidon de client_load)*/
	/*doit servir a repartir le travaille sur les GPU*/
	/*soit parce qu'elle ont des puissances differentes*/
	/*soit perce qu'une des crates travaille sur une portion de zone + simple (avec moins de vertex par exemple)*/
	/*initially everyone has the same load*/
	client_load=(int *)malloc(nbcarte*sizeof(int));
	for(i=0;i<nbcarte;i++)
		client_load[i]=1;


	client_num=0;

	fifo_init(&cmd_fifo);

#ifdef ENABLE_SINGLE_SCREEN
	single_screen_init();
#else
	multi_screen_init();
#endif

	/*spawn client processes*/
	for(i=1;i<nbcarte;i++)
	{
		pid_t pid;
		pid=fork();
		if (pid==0)
		{
			/*child process*/
			client_num=i;/*client num = num du tour de creation (de 0 a 3) */
			break;
		}
	}

	// fetches all useful function pointers for auto-generated functions
	initPointers();
	// creates the output function table
	creertabfunc();
	// fetches all useful function poitners for hand-written functions
	lib_funcs_init();
	// setup the vertex array code
	array_init();

#if 1
	if (client_num!=0){  
		GLOMPclient_run();/*tourne en boucle*/
	}
#else

	/*pour le debugage les segfault des client ne sont pas afficher*/
	/*ce bout de code inverse les serveur et cleint et permet donc l'affichage des segfault*/

	if (client_num==nbcarte-1)
		client_num=0;
	else if (client_num==0)
		client_num=nbcarte-1;

	if (client_num!=0) 
		GLOMPclient_run();/*tourne en boucle*/

#endif
}


/* Modify the __start function by overiding this function */

int __libc_start_main(int(*main_fct)(int, char **), int argc, 
		char ** ubp_av,void (*init) (void), void (*fini) (void), 
		void (*rtld_fini) (void),void (* stack_end)) {

	/*pointeur servant a deriver la fonction main*/
	int (*func)(int(*)(int, char **), int, char **,
			void (*) (void), void (*) (void), 
			void (*) (void), void(*));

	/*place le pointeur du main*/
	func  = dlsym(((void *) -1l), "__libc_start_main");

	GLOMP_init();/*lance l'initilisation de GLOMP*/
	int r= func(main_fct, argc, ubp_av, init, fini, rtld_fini, stack_end);/*lance le main du prog OpenGL*/
	return r;
}



