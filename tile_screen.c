
#include <string.h>
#include "tile_screen.h"
#include "lib_funcs.h"

static int tile_screen_x=0;
static int tile_screen_y=0;

static void tilecoord(int* tx,int* ty,int tilenum)
{
	*tx=tilenum%tile_screen_x;
	*ty=tilenum/tile_screen_x;
}

void tile_screen_glLoadMatrixf(int tilenum,const GLfloat* m)
{
	int tx,ty;
	GLfloat local_matrix[16];
	tilecoord(&tx,&ty,tilenum);
	memcpy(local_matrix,m,sizeof(GLfloat)*16);

	local_matrix[0]*=(float)tile_screen_x;
	if (tile_screen_x>1)
		local_matrix[8]=2.*(float)tx/(float)(tile_screen_x-1)-1.;
	
	// FIXME a verifier
	local_matrix[5]*=(float)tile_screen_y;
	if (tile_screen_y>1)
		local_matrix[9]=2.*(float)ty/(float)(tile_screen_y-1)-1.;

	lib_glLoadMatrixf(local_matrix);
}

void tile_screen_glLoadMatrixd(int tilenum,const GLdouble* m)
{
	int tx,ty;
	GLdouble local_matrix[16];
	tilecoord(&tx,&ty,tilenum);
	memcpy(local_matrix,m,sizeof(GLdouble)*16);

	local_matrix[0]*=(float)tile_screen_x;
	if (tile_screen_x>1)
		local_matrix[8]=2.*(float)tx/(float)(tile_screen_x-1)-1.;

	// FIXME a verifier
	local_matrix[5]*=(float)tile_screen_y;
	if (tile_screen_y>1)
		local_matrix[9]=2.*(float)ty/(float)(tile_screen_y-1)-1.;

	lib_glLoadMatrixd(local_matrix);
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



