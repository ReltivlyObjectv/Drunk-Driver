/*Author: Alexander Nguyen
  Modify by: Alexander Nguyen
  Date:6/17/2017

  Purpose: Create a start menu that will have Start, High Score, Credits and Exit
 */

/* 6/17/17 : Borrow some definition and function from bship and moditify it
 * 6/21/17 : add ppm image of menu, start button init function move check mouse
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
void check_button(XEvent *e);


extern Game g;
extern GLuint menuTexture;
//initalize buttons in its own function
void button_init(void);
void button_render(void);
bool high_score = false;
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
    glBindTexture(GL_TEXTURE_2D,menuTexture);
    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,0); 
    glTexCoord2f(0.0f,5.0f); glVertex2i(0,g.yres);
    glTexCoord2f(50.0f,5.0f); glVertex2i(g.xres,g.yres);
    glTexCoord2f(5.0f,0.0f); glVertex2i(g.xres,0);
  
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    if(game) {	
    	button_init();
    }
    button_render();
  
}


void button_init(void)
{
    //add button function to initiate
	button[nbuttons].r.width = 0;
	button[nbuttons].r.height = 0;
	button[nbuttons].r.left = g.xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = 0;
	button[nbuttons].r.right = 
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = 
		button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = 
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = 
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Start");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.0f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.0f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.0f;
	button[nbuttons].text_color = 0x0000ffff;
	nbuttons++;
}

void button_render(void)
{
    //add render function to render button
}


//move check mouse from car.cpp to this .cpp
void check_button(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	int x,y;
	int i;
	int lbutton=0;
	int rbutton=0;

	if (x == savex && y == savey)
		return;
	savex=x;
	savey=y;


	for (i=0; i<nbuttons; i++) {
		button[i].over=0;
		button[i].down=0;


		if (x >= button[i].r.left &&
			x <= button[i].r.right &&
			y >= button[i].r.bot &&
			y <= button[i].r.top) {
			button[i].over=1;
			break;
		}
	}
	if (lbutton)
		mouse_click(1);
	if (rbutton)
		mouse_click(2);

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
	  //show high score
          high_score = true;
        }
        if (i == 2) {
	  //show credits
          credits = true;
        }
        if (i == 3) {
          //Exit the game
          done = 1;
        }
      }
    }  
}

      
