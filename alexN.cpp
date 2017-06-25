/*Author: Alexander Nguyen
  Modify by: Alexander Nguyen
  Date:6/17/2017

  Purpose: Create a start menu that will have Start, High Score, Credits and Exit
 */

/* 6/17/17 : Borrow some definition and function from bship and moditify it
 * 6/21/17 : Add ppm image of menu, start button init function move check mouse
 * 6/22/17 : Move check mouse back and add the function to it to extend check mouse
 * 6/24/17 : Created and update check_button and initalize function in check_mouse
 *	     Finish button_init and button_render, will be adding addition 
 *           Game over menu/screen function, will be moving #include and definition 
 *	     then clean up codes to K&R.
 */

//Will fix the include on Saturday
#include <string.h>
#include "fonts.h"
#include "log.h"
#include "ppm.h"
#include "game.h"


int game = 1;
int startgame = 1;
int done = 0;
//4 buttons Start, High Score, Credits, Exit
#define MAXBUTTONS 4

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
    glBindTexture(GL_TEXTURE_2D,menuTexture);
    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,0); 
    glTexCoord2f(0.0f,5.0f); glVertex2i(0,g.yres);
    glTexCoord2f(50.0f,5.0f); glVertex2i(g.xres,g.yres);
    glTexCoord2f(5.0f,0.0f); glVertex2i(g.xres,0);
    glEnd();
    glPopMatrix();
    if(game) {	
    	button_init();
    }
    button_render();
  
}

//Button width
#define BUTTON_W 100;
//Button height
#define BUTTON_H 50;
//Button bottom
#define BUTTON_B 500;

//Button will be drawn onto the menu 
void button_init(void)
{
    //add button function to initiate
	//Start button
	button[nbuttons].r.width = BUTTON_W;
	button[nbuttons].r.height = BUTTON_H;
	button[nbuttons].r.left = g.xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = BUTTON_B;
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
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x0000ffff;
	nbuttons++;
	//High Score button
	button[nbuttons].r.width = (BUTTON_W + 200);
	button[nbuttons].r.height = BUTTON_H;
	button[nbuttons].r.left = g.xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = (BUTTON_B - 100);
	button[nbuttons].r.right = 
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = 
		button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = 
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = 
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "High Score");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x0000ffff;
	nbuttons++;
	//Credits button
	button[nbuttons].r.width = (BUTTON_W + 50);
	button[nbuttons].r.height = BUTTON_H;
	button[nbuttons].r.left = g.xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = (BUTTON_B - 200);
	button[nbuttons].r.right = 
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = 
		button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = 
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = 
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Credits");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x0000ffff;
	nbuttons++;
	//Quit button
	button[nbuttons].r.width = BUTTON_W;
	button[nbuttons].r.height = BUTTON_H;
	button[nbuttons].r.left = g.xres/2 - button[nbuttons].r.width/2;
	button[nbuttons].r.bot = (BUTTON_B - 300);
	button[nbuttons].r.right = 
		button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = 
		button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = 
		(button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = 
		(button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Exit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.0f;
	button[nbuttons].color[1] = 0.0f;
	button[nbuttons].color[2] = 0.0f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x0000ffff;
	nbuttons++;
}

//Render the buttons
void button_render(void)
{
    //add render function to render button
    Rect r;
    for (int i = 0; i < nbuttons; i++) {
	    if (button[i].over) {
		    //create the highlight color of button
		    glColor3f(5.0f, 0.0f, 23.0f);
		    //draw a highlight around the button
		    glLineWidth(2);
		    glBegin(GL_LINE_LOOP);
		    glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
		    glVertex2i(button[i].r.left-2,  button[i].r.top+2);
		    glVertex2i(button[i].r.right+2, button[i].r.top+2);
		    glVertex2i(button[i].r.right+2, button[i].r.bot-2);
		    glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
		    glEnd();
		    glLineWidth(1);
		}
	    if (button[i].down) {
		    glColor3fv(button[i].dcolor);
	    } else {
		    glColor3fv(button[i].color);
	    }
	    glBegin(GL_QUADS);
			glVertex2i(button[i].r.left,  button[i].r.bot);
			glVertex2i(button[i].r.left,  button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();
		r.left = button[i].r.centerx;
		r.bot  = button[i].r.centery-8;
		r.center = 1;
		if (button[i].down) {
			ggprint16(&r, 0, button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, button[i].text_color, button[i].text);
		}
	}
	
}


//Created a check button to check to see if mouse will click
void check_button(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	int x = e->xbutton.x;
	int y = e->xbutton.y;;
	int i;
	int lbutton=0;
	int rbutton=0;
	//y is backward
	y = g.yres - y;
	//when mouse button is release
	if (e->type == ButtonRelease) {
		mouse_click(2);
		return;
	}
	if (e->type == ButtonPress) {
		//when mouse left button is pressed
		if (e->xbutton.button == 1) {
			lbutton = 1;
		}
		//when mouse right button is pressed
		if (e-xbutton.button == 3) {
			rbutton = 1;
		}
	}
	//check to see if mouse move either in x or y direction
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex=e->xbutton.x;
		savey=e->xbutton.y;

	}
	else if (x == savex && y == savey)
		return;
	
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

//Will excuted when one of the state holds true
void mouse_click(int action)
{
 if(startgame)
 {
    if (action == 1) {
      //center of menu
      for (int i=0; i<nbuttons; i++) {
	if (button[i].over) {
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
   }
}
      
