/*
 * Window/viewport/frustum handling
 */
#include "init.h"
#ifdef ENABLE_SINGLE_SCREEN
#include "single_screen.h"
#else
#include "multi_screen.h"
#endif

#include "overrides.h"

static int glxattribs[100]={
	GLX_RED_SIZE, 4, 
	GLX_GREEN_SIZE, 4, 
	GLX_BLUE_SIZE, 4, 
	GLX_DEPTH_SIZE, 8,
	None};

/*
 * Our glXSwapBuffers function that intercepts the "real" function.
 *
 */
void glXSwapBuffers(Display *dpy, GLXDrawable drawable)
{
	int fnum=OVERRIDE_BASE;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_flush(&cmd_fifo);
#ifdef ENABLE_SINGLE_SCREEN
	single_screen_swap(dpy,drawable);
#else
	multi_screen_swap(dpy,drawable);
#endif
}

void GLOMPglXSwapBuffers()
{ 
#ifdef ENABLE_SINGLE_SCREEN
	single_screen_swap(NULL,0);
#else
	multi_screen_swap(NULL,0);
#endif
}



/*on recupere les proprite de la fenetre (taille) pour les diviser par le nbr de cartes, afin de repartire les taches,puis on la lance*/
int XSetStandardProperties(
			   Display*            dpy,
			   Window	        w,
			   _Xconst char*	name,
			   _Xconst char*	icon_string,
			   Pixmap		icon_pixmap,
			   char**		argv,
			   int			argc,
			   XSizeHints*		hints
			   )
{
	/*on la relance betement*/
	printf("XSetStandardProperties called\n");
	return lib_XSetStandardProperties(dpy,w,name,icon_string,icon_pixmap,argv,argc,hints  );

}

static void CreateWindow()
{
	static int init=0;
	int fnum=OVERRIDE_BASE+39;
	int i,value;

	if (init==1)
		return;
	init=1;
		

#ifdef ENABLE_SINGLE_SCREEN
	single_screen_init_window(glxattribs);
#else
	multi_screen_init_window(glxattribs);
#endif

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&width,sizeof(width));
	fifo_output(&cmd_fifo,&height,sizeof(height));

	i=0;
	do
	{
		value=*(glxattribs+i);
		fifo_output(&cmd_fifo,&value,sizeof(int));
		i++;
	}
	while(value!=None);

}


GLXWindow XCreateWindow(Display *display, Window parent, int x, int y,
			       unsigned int width2, unsigned int height2, unsigned int border_width, int depth, unsigned int class, Visual *visual,
			       unsigned long valuemask, XSetWindowAttributes *attribute)
{  
	width=width2;
	height=height2;
	printf("XCreateWindow %d %d\n",width,height);

	CreateWindow();

#ifdef ENABLE_SINGLE_SCREEN
	return lib_XCreateWindow(display, parent, x, y,width, height, border_width,depth,class,visual,valuemask, attribute);
#else
	if (!getenv("FORCE_GPU"))
	{
		XF86VidModeModeInfo **modes;
		int modeNum;
		lib_XF86VidModeGetAllModeLines(display, DefaultScreen(display), &modeNum, &modes);
		width=modes[0]->hdisplay;
		height=modes[0]->vdisplay;
		attribute->override_redirect=True;
		return lib_XCreateWindow(display, parent, x, y,width, height, border_width,depth,class,visual,valuemask | CWOverrideRedirect, attribute);
	}
	return lib_XCreateWindow(display, parent, x, y,width, height, border_width,depth,class,visual,valuemask, attribute);

#endif
}

void GLOMPXCreateWindow()
{
	int i,value;
	fifo_input(&cmd_fifo,&width,4);
	fifo_input(&cmd_fifo,&height,4);
	i=0;
	do
	{
		fifo_input(&cmd_fifo,&value,4);
		glxattribs[i]=value;
		i++;
	}
	while(value!=None);

#ifdef ENABLE_SINGLE_SCREEN
	single_screen_init_window(glxattribs);
#else
	multi_screen_init_window(glxattribs);
#endif
}

GLXContext glXCreateNewContext(Display *dpy, GLXFBConfig config,
                                      int render_type, GLXContext share_list,
                                      Bool direct)
{
	printf("glXCreateNewContext called !!!!!!!\n");
	return lib_glXCreateNewContext(dpy,config,render_type,share_list,direct);
}


GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, const int *attrib_list)
{
	printf("glXCreateWindow called !!!!!!!\n");
	return lib_glXCreateWindow(dpy,config,win,attrib_list);
}


GLXContext glXCreateContext(Display *dpy, XVisualInfo *vis,
                                   GLXContext share_list, Bool direct)
{
	printf("glXCreateContext called !!!!!!!\n");
	CreateWindow();
	return lib_glXCreateContext(dpy,vis,share_list,direct);
}

void glXDestroyContext(Display *dpy, GLXContext ctx)
{
	printf("glXDestroyContext called !!!!!!!\n");
	lib_glXDestroyContext(dpy,ctx);
}

Bool glXMakeCurrent(Display *dpy, GLXDrawable drawable, GLXContext ctx)
{
	printf("glXMakeCurrent called !!!!!!!\n");
	return lib_glXMakeCurrent(dpy,drawable,ctx);
}

void glXCopyContext(Display *dpy, GLXContext src, GLXContext dst, unsigned long mask)
{
	printf("glXCopyContext called !!!!!!!\n");
	lib_glXCopyContext(dpy,src,dst,mask);
}

XVisualInfo* glXChooseVisual(Display *dpy, int screen,
                                    int *attrib_list)
{
	int i=0;
	GLOMP_init();

	while(attrib_list[i]!=None) 
		i++;

	memcpy(glxattribs,attrib_list,i*sizeof(int));

	return lib_glXChooseVisual(dpy,screen,attrib_list);
}

const char * glXQueryServerString(Display *dpy, int screen, int name)
{
	return lib_glXQueryServerString(dpy,screen,name);
}

const char * glXGetClientString(Display *dpy, int name)
{
	return lib_glXGetClientString(dpy,name);
}

void glFrustum ( GLdouble p0 , GLdouble p1 , GLdouble p2 , GLdouble p3 , GLdouble p4 , GLdouble p5 )
{
	int fnum=OVERRIDE_BASE+1;
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&p0,8);
	fifo_output(&cmd_fifo,&p1,8);
	fifo_output(&cmd_fifo,&p2,8);
	fifo_output(&cmd_fifo,&p3,8);
	fifo_output(&cmd_fifo,&p4,8);
	fifo_output(&cmd_fifo,&p5,8);

#ifdef ENABLE_SINGLE_SCREEN
	int totalload=0;
	int beforeload=0;
	int i;
	GLdouble newp2;
	GLdouble newp3;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}
	newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
	newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);
	lib_glFrustum(p0,p1,newp2,newp3,p4,p5);
#else
	tile_screen_glFrustum(client_num,p0,p1,p2,p3,p4,p5);
#endif
}


/*on a besoin de gerer la perspective donc on interceptent les info de glfrumstun*/
void GLOMPglFrustum()
{
	int i;

	GLdouble p0;
	GLdouble p1;
	GLdouble p2;
	GLdouble p3;
	GLdouble p4;
	GLdouble p5;
	GLdouble newp2;
	GLdouble newp3;
	fifo_input(&cmd_fifo,&p0,8);
	fifo_input(&cmd_fifo,&p1,8);
	fifo_input(&cmd_fifo,&p2,8);
	fifo_input(&cmd_fifo,&p3,8);
	fifo_input(&cmd_fifo,&p4,8);
	fifo_input(&cmd_fifo,&p5,8);

#ifdef ENABLE_SINGLE_SCREEN
	int totalload=0;
	int beforeload=0;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}


	newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
	newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);
	printf("[%d] les anciennes %f %f --- les nexs %f %f\n",client_num,p2,p3,newp2,newp3);

	lib_glFrustum(p0,p1,newp2,newp3,p4,p5);
#else
	tile_screen_glFrustum(client_num,p0,p1,p2,p3,p4,p5);
#endif
}

#ifdef ENABLE_SINGLE_SCREEN
void glViewport ( GLint x,GLint y,GLsizei w,GLsizei h )
{
	int i;
	int fnum=OVERRIDE_BASE+40;

	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&x,4);
	fifo_output(&cmd_fifo,&y,4);
	fifo_output(&cmd_fifo,&w,4);
	fifo_output(&cmd_fifo,&h,4);

	int totalload=0;
	int beforeload=0;
	GLsizei newp3;
	GLint newp1;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}
	newp1=y+h*((double)beforeload/(double)totalload);

	newp3=h*(double)client_load[client_num]/(double)totalload;


	printf("viewport:%d    %d %d %d %d",client_num,y,h,newp1,newp3);

	lib_glViewport(x,newp1,w,newp3);

}

void GLOMPglViewport()
{
	int i;

	GLsizei p2,p3,newp3;
	GLint p0,p1,newp1;
	fifo_input(&cmd_fifo,&p0,4);
	fifo_input(&cmd_fifo,&p1,4);
	fifo_input(&cmd_fifo,&p2,4);
	fifo_input(&cmd_fifo,&p3,4);

	int totalload=0;
	int beforeload=0;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}
	newp1=p1+p3*((double)beforeload/(double)totalload);

	newp3=p3*(double)client_load[client_num]/(double)totalload;

	printf("viewport:%d    %d %d %d %d",client_num,p1,p3,newp1,newp3);

	lib_glViewport(p0,newp1,p2,newp3);
}
#endif


void glOrtho ( GLdouble p0, GLdouble p1,GLdouble p2,GLdouble p3,GLdouble p4,GLdouble p5 )
{
	int fnum=OVERRIDE_BASE+41;
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
	fifo_output(&cmd_fifo,&p0,8);
	fifo_output(&cmd_fifo,&p1,8);
	fifo_output(&cmd_fifo,&p2,8);
	fifo_output(&cmd_fifo,&p3,8);
	fifo_output(&cmd_fifo,&p4,8);
	fifo_output(&cmd_fifo,&p5,8);

#ifdef ENABLE_SINGLE_SCREEN
	int totalload=0;
	int beforeload=0;
	int i;
	GLdouble newp2;
	GLdouble newp3;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}


	newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
	newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);

	lib_glOrtho(p0,p1,newp2,newp3,p4,p5);
#else
	tile_screen_glOrtho(client_num,p0,p1,p2,p3,p4,p5);
#endif
}

void GLOMPglOrtho()
{

	int i;

	GLdouble p0;
	GLdouble p1;
	GLdouble p2;
	GLdouble p3;
	GLdouble p4;
	GLdouble p5;
	GLdouble newp2;
	GLdouble newp3;
	fifo_input(&cmd_fifo,&p0,8);
	fifo_input(&cmd_fifo,&p1,8);
	fifo_input(&cmd_fifo,&p2,8);
	fifo_input(&cmd_fifo,&p3,8);
	fifo_input(&cmd_fifo,&p4,8);
	fifo_input(&cmd_fifo,&p5,8);

#ifdef ENABLE_SINGLE_SCREEN
	int totalload=0;
	int beforeload=0;
	for(i=0;i<nbcarte;i++)
	{
		totalload+=client_load[i];
		if (i<client_num)
			beforeload+=client_load[i];
	}


	printf("%f %f\n",p2,p3);

	newp2=p2+(p3-p2)*((double)beforeload/(double)totalload);
	newp3=p2+(p3-p2)*((double)(beforeload+client_load[client_num])/(double)totalload);

	printf("new %f %f\n",newp2,newp3);

	lib_glOrtho(p0,p1,newp2,newp3,p4,p5);
#else
	tile_screen_glOrtho(client_num,p0,p1,p2,p3,p4,p5);
#endif
}

static void server_quit()
{
	int fnum=OVERRIDE_BASE+49;
	fifo_output(&cmd_fifo,&fnum,sizeof(fnum));
}

void GLOMPquit()
{
#ifdef ENABLE_SINGLE_SCREEN
	multi_screen_close();
#else
#endif
	exit(0);
}


int XDestroyWindow ( Display * disp, Window parent)
{
	//server_quit();
	int r=lib_XDestroyWindow(disp,parent);
	kill(0,SIGKILL);
	return r;
}

int XCloseDisplay ( Display * disp) 
{
	//server_quit();
	int r=lib_XCloseDisplay(disp);
	kill(0,SIGKILL);  
	return r;
}

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xmin, xmax, ymin, ymax;

	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}


