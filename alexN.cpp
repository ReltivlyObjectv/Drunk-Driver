/*Author: Alexander Nguyen
Modify by: Alexander Nguyen
Date:6/17/2017

Purpose: Create a start menu that will necessary function
also have it connect to php file and upload score.
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <cstdlib>  
#include <unistd.h> 
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include "log.h"
#include "ppm.h"
  
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix [4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]

void print_group()
{
  string group[] = {"Alex", "Dave", "Christian", "Abdullah"};
}

struct Vec { 
    float x, y, z;
};

struct Shape {
  float width, height;
	float radius;
	Vec center;
};

#define MAXBUTTONS 8
//button click from bship framework
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int mouse[2];
    int down;
    float color[3];
    float dcolor[3];
    int click;
    unsigned int text_color;
}Button;
int nbuttons=0;

Button button[MAXBUTTONS];
void gamemenu(void);
void mouse_click(int b_click, int action, int x, int y);
void check_mouse(XEvent *e);


//initalize buttons in its own function
//6/19/17 does it need check_resize??
void button_init(void);
void button_render(void);
bool game = false;

void gamemenu(void)
{
	glPushMatrix();
	glBindTexture(GL_Texture_2D,0);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUAD); //look at bttleship init_opengl to place ppm images
	glTexCoord2f(0.0f,0.0f); glVertex2i(0,0); // will modify when it compiles
	glTexCoord2f(0.0f,0.0f); glVertex2i(0,yres);
	glTexCoord2f(0.0f,0.0f); glVertex2i(xres,yres);
	glTexCoord2f(0.0f,0.0f); glVertex2i(xres,0);
	glEnd();
