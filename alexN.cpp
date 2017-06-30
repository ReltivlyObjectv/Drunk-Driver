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

#include <string.h>
#include "fonts.h"
#include "log.h"
#include "ppm.h"
#include "game.h"
//-------------------------------------------------
//Button width
#define BUTTON_W 50
//Button height
#define BUTTON_H 50
//Button bottom
#define BUTTON_B 400
//4 buttons Start, High Score, Credits, Exit
#define MAXBUTTONS 4
//------------------------------------------------
extern int startgame;

//button click from bship framework
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
	t_button() { over = false; }
}Button;

Button button[MAXBUTTONS];

//----------------------------------------------
void mouse_click(int action);
void check_button(XEvent *e);
extern Game g;
extern GLuint menuTexture;
void button_init(void);
void button_render(void);
bool pause = false;
static int credits = 1;
void game_pause(void);
static bool menu = true;
//---------------------------------------------

//Start Menu
void gamemenu(void)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,menuTexture);
	glBegin(GL_QUADS); 
	glTexCoord2f(0.0f,0.0f); glVertex2i(0,g.yres); 
	glTexCoord2f(0.0f,1.0f); glVertex2i(0,0);
	glTexCoord2f(1.0f,1.0f); glVertex2i(g.xres,0);
	glTexCoord2f(1.0f,0.0f); glVertex2i(g.xres,g.yres);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	button_init();
	button_render();


	 
}

//Button will be drawn onto the menu 
void button_init(void)
{
	//add button function to initiate
	//Start button
	button[0].r.width = BUTTON_W;
	button[0].r.height = BUTTON_H;
	button[0].r.left = g.xres/2 - button[0].r.width/2;
	button[0].r.bot = BUTTON_B;
	button[0].r.right = 
		button[0].r.left + button[0].r.width;
	button[0].r.top = 
		button[0].r.bot + button[0].r.height;
	button[0].r.centerx = 
		(button[0].r.left + button[0].r.right) / 2;
	button[0].r.centery = 
		(button[0].r.bot + button[0].r.top) / 2;
	strcpy(button[0].text, "Start");
	button[0].down = 0;
	button[0].click = 0;
	button[0].color[0] = 0.0f;
	button[0].color[1] = 0.0f;
	button[0].color[2] = 0.0f;
	button[0].dcolor[0] = button[0].color[0] * 0.5f;
	button[0].dcolor[1] = button[0].color[1] * 0.5f;
	button[0].dcolor[2] = button[0].color[2] * 0.5f;
	button[0].text_color = 0x00ffffff;
	//High Score button
	button[1].r.width = (BUTTON_W + 200);
	button[1].r.height = BUTTON_H;
	button[1].r.left = g.xres/2 - button[1].r.width/2;
	button[1].r.bot = (BUTTON_B - 100);
	button[1].r.right = 
		button[1].r.left + button[1].r.width;
	button[1].r.top = 
		button[1].r.bot + button[1].r.height;
	button[1].r.centerx = 
		(button[1].r.left + button[1].r.right) / 2;
	button[1].r.centery = 
		(button[1].r.bot + button[1].r.top) / 2;
	strcpy(button[1].text, "High Score");
	button[1].down = 0;
	button[1].click = 0;
	button[1].color[0] = 0.0f;
	button[1].color[1] = 0.0f;
	button[1].color[2] = 0.0f;
	button[1].dcolor[0] = button[1].color[0] * 0.5f;
	button[1].dcolor[1] = button[1].color[1] * 0.5f;
	button[1].dcolor[2] = button[1].color[2] * 0.5f;
	button[1].text_color = 0x00ffffff;
	//Credits button
	button[2].r.width = (BUTTON_W + 50);
	button[2].r.height = BUTTON_H;
	button[2].r.left = g.xres/2 - button[2].r.width/2;
	button[2].r.bot = (BUTTON_B - 200);
	button[2].r.right = 
		button[2].r.left + button[2].r.width;
	button[2].r.top = 
		button[2].r.bot + button[2].r.height;
	button[2].r.centerx = 
		(button[2].r.left + button[2].r.right) / 2;
	button[2].r.centery = 
		(button[2].r.bot + button[2].r.top) / 2;
	strcpy(button[2].text, "Credits");
	button[2].down = 0;
	button[2].click = 0;
	button[2].color[0] = 0.0f;
	button[2].color[1] = 0.0f;
	button[2].color[2] = 0.0f;
	button[2].dcolor[0] = button[2].color[0] * 0.5f;
	button[2].dcolor[1] = button[2].color[1] * 0.5f;
	button[2].dcolor[2] = button[2].color[2] * 0.5f;
	button[2].text_color = 0x00ffffff;
	//Quit button
	button[3].r.width = BUTTON_W;
	button[3].r.height = BUTTON_H;
	button[3].r.left = g.xres/2 - button[3].r.width/2;
	button[3].r.bot = (BUTTON_B - 300);
	button[3].r.right = 
		button[3].r.left + button[3].r.width;
	button[3].r.top = 
		button[3].r.bot + button[3].r.height;
	button[3].r.centerx = 
		(button[3].r.left + button[3].r.right) / 2;
	button[3].r.centery = 
		(button[3].r.bot + button[3].r.top) / 2;
	strcpy(button[3].text, "Exit");
	button[3].down = 0;
	button[3].click = 0;
	button[3].color[0] = 0.0f;
	button[3].color[1] = 0.0f;
	button[3].color[2] = 0.0f;
	button[3].dcolor[0] = button[3].color[0] * 0.5f;
	button[3].dcolor[1] = button[3].color[1] * 0.5f;
	button[3].dcolor[2] = button[3].color[2] * 0.5f;
	button[3].text_color = 0x00ffffff;
}

//Render the buttons
void button_render(void)
{
	//add render function to render button
	for (int i = 0; i < MAXBUTTONS; i++) {
		if (button[i].over) {
			//create the highlight color of button
			glColor3f(1.5f, 0.0f, 1.0f);
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
		glBegin(GL_QUADS);
		if (button[i].down) {
			glColor3fv(button[i].dcolor);
		} else {
			glColor3fv(button[i].color);
		}
		glVertex2i(button[i].r.left,  button[i].r.bot);
		glVertex2i(button[i].r.left,  button[i].r.top);
		glVertex2i(button[i].r.right, button[i].r.top);
		glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();
		Rect r;
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
	int x,y,i; 
	int lbutton=0;
	int rbutton=0;
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
		if (e->xbutton.button == 3) {
			rbutton = 1;
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = g.yres-y;
	//check to see if mouse move either in x or y direction
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex=e->xbutton.x;
		savey=e->xbutton.y;

	}
	if (x == savex && y == savey)
		return;

	savex=x;
	savey=y;
	for (i=0; i<MAXBUTTONS; i++) {
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
    if(menu) {
	menu = false;
	if (action == 1) {
		//center of menu
		for (int i=0; i<MAXBUTTONS; i++) {
			if (button[i].over) {
				button[i].down = 1;
				button[i].click = 1;
				if (i == 0) {
					//start the game
					startgame = 0;
					break;
				} 
				if (i == 1) {
					//show high score
					system("firefox http://www.army.mil/article/181742/dont_ruin_the_game_by_drinking_driving");
					break;
				}
				if (i == 2) {
					//show credits
					credits = 0;
					break;
				}
				if (i == 3) {
					//Exit the game
					g.done = 1;
				}
			}
		}
	}
	}
}

void game_pause(void)
{
    double h = 100.0;
    double w = 200.0;
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0, 0.0, 0.0, 0.8);
    glTranslated(g.xres/2, g.yres/2, 0);
    glBegin(GL_QUADS);
    glVertex2i(-w, -h);
    glVertex2i(-w,  h);
    glVertex2i( w,  h);
    glVertex2i( w, -h);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();

    Rect r;
    r.bot = g.yres/2 + 150;
    r.left = g.xres/2;
    r.center= 1;
    ggprint16(&r, 16, 0, "Pause Screen.");

    if (pause) {
	ggprint16(&r, 16, 0, "Press P again to unpause");
	pause = false;
    }
	//---------------------------------------------------------
//Game Over Screen
void Game_over(void)
{
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,gameoverTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex2i(0,g.yres);
        glTexCoord2f(0.0f,1.0f); glVertex2i(0,0);
        glTexCoord2f(1.0f,1.0f); glVertex2i(g.xres,0);
        glTexCoord2f(1.0f,0.0f); glVertex2i(g.xres,g.yres);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
}


}
