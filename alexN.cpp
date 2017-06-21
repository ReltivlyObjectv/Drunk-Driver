/*Author: Alexander Nguyen
  Modify by: Alexander Nguyen
  Date:6/17/2017

  Purpose: Create a start menu that will have Start, High Score, Credits and Exit
 */

/*
 *
 *
 *
 *
 *
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
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "fonts.h"
#include "log.h"
#include "ppm.h"
#include "game.h"


int xres=500;
int yres=500;
int game = 1;
int startgame = 1;
int done = 0;
#define MAXBUTTONS 8

//button click from bship framework
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int mouse[2];
    float color[3];
    float dcolor[3];
    int click;
    unsigned int text_color;
}Button;
int nbuttons=0;

Button button[MAXBUTTONS];
void gamemenu(void);
void mouse_click(int action);
void check_mouse(XEvent *e);


//initalize buttons in its own function
void button_init(void);
void button_render(void);
bool score = false;
bool credits = false;

//Start Menu
void gamemenu(void)
{

    glPushMatrix();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D,0);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS); //look at bttleship init_opengl to place ppm images
    glColor3f(100,100,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,0); // will modify when it compiles
    glTexCoord2f(5.0f,5.0f); glVertex2i(0,yres);
    glTexCoord2f(10.0f,5.0f); glVertex2i(xres,yres);
    glTexCoord2f(0.0f,5.0f); glVertex2i(xres,0);
    glEnd();
    button_init();
    button_render();
  
}


void button_init(void)
{
    //add button function to initiate	
}

void button_render(void)
{
    //add render function to render button
}

void mouse_click(int action)
{
 
    if (action == 1) {
      //center of menu
      for (int i=0;nbuttons; i++) {
        button[i].down = 1;
        button[i].click = 1;
        if (i == 0) {
          //start the game
          startgame = 0;
        } 
        if (i == 1) {
          score = true;
        }
        if (i == 2) {
          credits = true;
        }
        if (i == 3) {
          //set check key escape to done=1
          done = 1;
        }
      }
    }  
}

      
