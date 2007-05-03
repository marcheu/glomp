#ifndef _TILE_SCREEN_H_
#define _TILE_SCREEN_H_

#include "glheader.h"

void tile_screen_init(int tilex,int tiley);
void tile_screen_glFrustum(int tilenum,GLdouble p0,GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5);
void tile_screen_glOrtho(int tilenum,GLdouble p0,GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5);

#endif

