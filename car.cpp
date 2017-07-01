//
//program: car.cpp
//author:  Gordon Griesel
//date:    summer 2017
//
//Framework for group attempting a 3D game.
//
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/keysym.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <GL/glx.h>
#include <GL/glu.h>
#include "log.h"
#include "fonts.h"
#include "game.h"
#include "ppm.h"
//---------------------------------------
//Alex start menu images
Ppmimage *menuImage = NULL;
GLuint menuTexture;
Ppmimage *gameoverImage = NULL;
GLuint gameoverTexture;

//---------------------------------------
Ppmimage *inCarPic = NULL;
GLuint inCarTextures;
//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

int startgame = 1;
void init(void);
void initXWindows(void);
void init_opengl(void);
//void init_textures(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void physics(void);
void render(void);


//Turned menu off so it doesn't break program -- Christian
Game g;

int main(void)
{
	printf("Loading Drunk Driver\n");
	initXWindows();
	init_opengl();
	init();
	while (!g.done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			check_keys(&e);
			//check to see if mouse clicked correctly
			check_button(&e);
		}
		physics();
		render();
		g.updateCooldowns();
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Drunk Driver");
}

void setup_screen_res(const int w, const int h)
{
	g.xres = w;
	g.yres = h;
	g.aspectRatio = (GLfloat)g.xres / (GLfloat)g.yres;
}

void initXWindows(void)
{
	//Look here for information on XVisualInfo parameters.
	//http://www.talisman.org/opengl-1.1/Reference/glXChooseVisual.html
	Window root;
	GLint att[] = { GLX_RGBA,
		GLX_STENCIL_SIZE, 2,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	//XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	setup_screen_res(640, 480);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask |
		ButtonPressMask | ButtonReleaseMask | 
		PointerMotionMask | StructureNotifyMask |
		SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	set_title();
}

void init(void)
{
	ControlManager::movingLeft = 
		ControlManager::movingRight = 
		ControlManager::slowingDown = 
		ControlManager::speedingUp = false;
}

void init_opengl(void)
{
	//OpenGL initialization
	glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, g.aspectRatio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	//Enable this so material colors are the same as vert colors.
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//Turn on a light
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	glEnable(GL_LIGHT0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//Alex Menu Image 
	menuImage	= ppm6GetImage("./images/menu.ppm");
	glGenTextures(1 , &menuTexture);
	glBindTexture(GL_TEXTURE_2D, menuTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			menuImage->width, menuImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, menuImage->data);	
	//End of Menu Image
	 //Alex's Game Over Image
        gameoverImage   = ppm6GetImage("./images/gameover.ppm");
        glGenTextures(1 , &gameoverTexture);
        glBindTexture(GL_TEXTURE_2D, menuTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                        gameoverImage->width, gameoverImage->height,
                        0, GL_RGB, GL_UNSIGNED_BYTE, gameoverImage->data);
	//Dave show car dash
	inCarPic = ppm6GetImage("./images/inCarPic.ppm");
	glGenTextures(1 , &inCarTextures);
	glBindTexture(GL_TEXTURE_2D, inCarTextures);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			inCarPic->width, inCarPic->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, inCarPic->data);	
	//Ppmimage *inCarImage=NULL;
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}

Flt vecNormalize(Vec vec) {
	Flt len = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	Flt tlen = 1.0 / len;
	vec[0] *= tlen;
	vec[1] *= tlen;
	vec[2] *= tlen;
	return len;
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	check_button(e);//Check to see if it works 
}

void check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		ControlManager::applyControls(g, key, true);
	} else if (e->type == KeyRelease) {
		int key = XLookupKeysym(&e->xkey, 0);
		ControlManager::applyControls(g, key, false);
	}
}


void identity33(Matrix m)
{
	m[0][0] = m[1][1] = m[2][2] = 1.0f;
	m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = 0.0f;
}

void yy_transform(const Vec rotate, Matrix a)
{
	//This function applies a rotation to a matrix.
	//Call this function first, then call trans_vector() to apply the
	//rotations to an object or vertex.
	//
	if (rotate[0] != 0.0f) {
		Flt ct = cos(rotate[0]), st = sin(rotate[0]);
		Flt t10 = ct*a[1][0] - st*a[2][0];
		Flt t11 = ct*a[1][1] - st*a[2][1];
		Flt t12 = ct*a[1][2] - st*a[2][2];
		Flt t20 = st*a[1][0] + ct*a[2][0];
		Flt t21 = st*a[1][1] + ct*a[2][1];
		Flt t22 = st*a[1][2] + ct*a[2][2];
		a[1][0] = t10;
		a[1][1] = t11;
		a[1][2] = t12;
		a[2][0] = t20;
		a[2][1] = t21;
		a[2][2] = t22;
		return;
	}
	if (rotate[1] != 0.0f) {
		Flt ct = cos(rotate[1]), st = sin(rotate[1]);
		Flt t00 = ct*a[0][0] - st*a[2][0];
		Flt t01 = ct*a[0][1] - st*a[2][1];
		Flt t02 = ct*a[0][2] - st*a[2][2];
		Flt t20 = st*a[0][0] + ct*a[2][0];
		Flt t21 = st*a[0][1] + ct*a[2][1];
		Flt t22 = st*a[0][2] + ct*a[2][2];
		a[0][0] = t00;
		a[0][1] = t01;
		a[0][2] = t02;
		a[2][0] = t20;
		a[2][1] = t21;
		a[2][2] = t22;
		return;
	}
	if (rotate[2] != 0.0f) {
		Flt ct = cos(rotate[2]), st = sin(rotate[2]);
		Flt t00 = ct*a[0][0] - st*a[1][0];
		Flt t01 = ct*a[0][1] - st*a[1][1];
		Flt t02 = ct*a[0][2] - st*a[1][2];
		Flt t10 = st*a[0][0] + ct*a[1][0];
		Flt t11 = st*a[0][1] + ct*a[1][1];
		Flt t12 = st*a[0][2] + ct*a[1][2];
		a[0][0] = t00;
		a[0][1] = t01;
		a[0][2] = t02;
		a[1][0] = t10;
		a[1][1] = t11;
		a[1][2] = t12;
		return;
	}
}

void trans_vector(Matrix mat, const Vec in, Vec out)
{
	Flt f0 = mat[0][0] * in[0] + mat[1][0] * in[1] + mat[2][0] * in[2];
	Flt f1 = mat[0][1] * in[0] + mat[1][1] * in[1] + mat[2][1] * in[2];
	Flt f2 = mat[0][2] * in[0] + mat[1][2] * in[1] + mat[2][2] * in[2];
	out[0] = f0;
	out[1] = f1;
	out[2] = f2;
}


void physics(void)
{
	ControlManager::moveForward(g);
	ControlManager::checkBounds(g);
}

void render(void)
{
	//if startgame holds true pop up menu
	if (startgame == 1) {
		glDisable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, g.xres, g.yres);
		glMatrixMode(GL_MODELVIEW);   glLoadIdentity();
		glMatrixMode (GL_PROJECTION); glLoadIdentity();
		gluOrtho2D(0, g.xres, 0, g.yres);
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);

		gamemenu();
		glPopAttrib();
		glEnable(GL_DEPTH_TEST);
	} else {
		Rect r;
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//
		//3D mode
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		gluPerspective(45.0f, g.aspectRatio, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//for documentation...
		gluLookAt(
				g.cameraPosition[0], g.cameraPosition[1], g.cameraPosition[2],
				g.cameraPosition[0]-g.up[0], g.cameraPosition[1]-g.up[1], g.cameraPosition[2]-g.up[2],
				0, 1, 0);
		//
		drawStreet(g);
		//
		//show car dash
		showInCar();
		//
		//
		//switch to 2D mode
		//
		glViewport(0, 0, g.xres, g.yres);
		glMatrixMode(GL_MODELVIEW);   glLoadIdentity();
		glMatrixMode (GL_PROJECTION); glLoadIdentity();
		gluOrtho2D(0, g.xres, 0, g.yres);
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
		r.bot = g.yres - 20;
		r.left = 10;
		r.center = 0;
		ggprint8b(&r, 16, 0x00887766, "Drunk Driver");
		drawDebugInfo(g);
		glPopAttrib();
		//Blackout if need be
		blackoutScreen(g);
		if (g.gameState == PAUSED) {
			drawPauseMenu(g);
		}
	}
}

void drawStreet(Game& g)
{
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);
	float w = 5.0;
	float d = 300.0;
	float h = 0.0;
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( w, h,-d);
	glVertex3f(-w, h,-d);
	glVertex3f(-w, h, d);
	glVertex3f( w, h, d);
	glEnd();
	//double yellow line
	glColor3f(0.8f, 0.8f, 0.2f);
	w = 0.1;
	d = 300.0;
	h = 0.01;
	glPushMatrix();
	glTranslatef(-0.15f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( w, h,-d);
	glVertex3f(-w, h,-d);
	glVertex3f(-w, h, d);
	glVertex3f( w, h, d);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.15f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( w, h,-d);
	glVertex3f(-w, h,-d);
	glVertex3f(-w, h, d);
	glVertex3f( w, h, d);
	glEnd();
	glPopMatrix();
	//guard rails
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i=0; i<120; i++) {
		glPushMatrix();
		glTranslatef(6.0f, -0.5f, (float)-i*2.5);
		box(0.2, 5.0, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-6.0f, -0.5f, (float)-i*2.5);
		box(0.2, 5.0, 0.2);
		glPopMatrix();
	}
	//float lightPosition = *g.lightPosition + g.cameraPosition[2];
	//float lightPosition = *g.lightPosition;
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
}


