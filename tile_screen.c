
#include "tile_screen.h"
#include "lib_funcs.h"

static int tile_screen_x=0;
static int tile_screen_y=0;

static void tilecoord(int* tx,int* ty,int tilenum)
{
	*tx=tilenum%tile_screen_x;
	*ty=tilenum/tile_screen_x;
}

void tile_screen_glFrustum(int tilenum,GLdouble p0,GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5)
{
	int tx,ty;
	tilecoord(&tx,&ty,tilenum);

	GLdouble newp0=p0+(p1-p0)*tx/tile_screen_x;
	GLdouble newp1=p0+(p1-p0)*(tx+1)/tile_screen_x;
	GLdouble newp2=p2+(p3-p2)*ty/tile_screen_y;
	GLdouble newp3=p2+(p3-p2)*(ty+1)/tile_screen_y;

	lib_glFrustum(newp0,newp1,newp2,newp3,p4,p5);
}

void tile_screen_glOrtho(int tilenum,GLdouble p0,GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5)
{
	int tx,ty;
	tilecoord(&tx,&ty,tilenum);

	GLdouble newp0=p0+(p1-p0)*tx/tile_screen_x;
	GLdouble newp1=p0+(p1-p0)*(tx+1)/tile_screen_x;
	GLdouble newp2=p2+(p3-p2)*ty/tile_screen_y;
	GLdouble newp3=p2+(p3-p2)*(ty+1)/tile_screen_y;

	lib_glOrtho(newp0,newp1,newp2,newp3,p4,p5);
}



/* init the screen tiling module */
void tile_screen_init(int tilex,int tiley)
{
	tile_screen_x=tilex;
	tile_screen_y=tiley;
}



