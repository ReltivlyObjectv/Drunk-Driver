/*Author: Alexander Nguyen
 Modify by: Alexander Nguyen
 Date:6/17/2017
 Purpose: Create a start menu that will have Start, High Score, Credits and Exit
 */

/* 6/17/17 : Borrow some definition and function from bship and moditify it
 * 6/21/17 : Add ppm image of menu, start button init function move check mouse
 * 6/22/17 : Move check mouse back and add the function to it to extend check mouse
 * 6/24/17 : Created and update check_button and initalize function in check_mouse
 *6/30/17 : Fix menu bug, added pause menu and gameover menu
 *July Update: Menu fixed, Credits scene completed
 */
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "fonts.h"
#include "log.h"
#include "ppm.h"
#include "game.h"
#include "math.h"
//-------------------------------------------------
//Button width
#define BUTTON_W 50
//Button height
#define BUTTON_H 50
//Button bottom
#define BUTTON_B 400
//4 buttons Start, High Score, Credits, Exit
#define MAXBUTTONS 4
#define GAMEOVER_BUTTONS 2
//------------------------------------------------
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
Button button2[GAMEOVER_BUTTONS];

//----------------------------------------------
void mouse_click(int action, Game& g);
void check_button2(XEvent *e,Game& g);
extern Game g;
extern GLuint menuTexture;
extern GLuint gameoverTexture;
//---------------------------------------------
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
void check_button(XEvent *e, Game& g)
{
    static int savex = 0;
    static int savey = 0;
    int x,y,i;
    int lbutton=0;
    int rbutton=0;
    //when mouse button is release
    if (e->type == ButtonRelease) {
        mouse_click(2, g);
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
        mouse_click(1, g);
    if (rbutton)
        mouse_click(2, g);
    
}

//Will excuted when one of the state holds true
void mouse_click(int action, Game& g)
{
    //if(!menu) {
    if (g.gameState == MENU) {
        if (action == 1) {
            //center of menu
            for (int i=0; i<MAXBUTTONS; i++) {
                if (button[i].over) {
                    button[i].down = 1;
                    button[i].click = 1;
                    if (i == 0) {
                        //start the game
                        //startgame = false;
                        //menu = true;
                        printf("Starting game...\n");
                        g.gameState = UNPAUSED;
                        break;
                    }
                    if (i == 1) {
                        //show high score
                        system("firefox http://www.army.mil/article/181742/dont_ruin_the_game_by_drinking_driving");
                        break;
                    }
                    if (i == 2) {
                        //show credits
                        //credits = 0;
                        //TODO Uncomment below line once credits is implemented
                        g.gameState = CREDITS;
                        break;
                    }
                    if (i == 3) {
                        //Exit the game
                        g.done = 1;
                    }
                }
            }
        }
    } else if (g.gameState == GAMEOVER) {
        gameover_click(action, g);
        
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
    
    if (g.gameState == PAUSED) {
        ggprint16(&r, 16, 0, "Press P again to unpause");
        //pause = false;
    }
}
//---------------------------------------------------------
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
//Game Over Screen
void game_over()
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,gameoverTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,g.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(g.xres,g.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(g.xres,0);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    gameover_init();
    gameover_render();
    
}

void gameover_click(int action, Game& g)
{
    if (g.gameState == GAMEOVER) {
        if (action == 1) {
            int i =0;
            for (i=0; i<GAMEOVER_BUTTONS; i++) {
                if (button2[i].over) {
                    button2[i].down = 1;
                    button2[i].click = 1;
                    if (i==0) {
                        //bring back to game menu
                        g.gameState = MENU;
                        ControlManager::reset(&g);
                        break;
                    }
                    if (i==1) {
                        g.done = 1;
                    }
                }
            }
        }
    }
}

void gameover_init()
{
    button2[0].r.width = BUTTON_W;
    button2[0].r.height = BUTTON_H;
    button2[0].r.left = g.xres/2 - button2[0].r.width/2;
    button2[0].r.bot = BUTTON_B - 200;
    button2[0].r.right =
    button2[0].r.left + button2[0].r.width;
    button2[0].r.top =
    button2[0].r.bot + button2[0].r.height;
    button2[0].r.centerx =
    (button2[0].r.left + button2[0].r.right) / 2;
    button2[0].r.centery =
    (button2[0].r.bot + button2[0].r.top) / 2;
    strcpy(button2[0].text, "Menu");
    button2[0].down = 0;
    button2[0].click = 0;
    button2[0].color[0] = 0.0f;
    button2[0].color[1] = 0.0f;
    button2[0].color[2] = 0.0f;
    button2[0].dcolor[0] = button2[0].color[0] * 0.5f;
    button2[0].dcolor[1] = button2[0].color[1] * 0.5f;
    button2[0].dcolor[2] = button2[0].color[2] * 0.5f;
    button2[0].text_color = 0x00ffffff;
    
    button2[1].r.width = (BUTTON_W);
    button2[1].r.height = BUTTON_H;
    button2[1].r.left = g.xres/2 - button2[1].r.width/2;
    button2[1].r.bot = (BUTTON_B - 275);
    button2[1].r.right =
    button2[1].r.left + button2[1].r.width;
    button2[1].r.top =
    button2[1].r.bot + button2[1].r.height;
    button2[1].r.centerx =
    (button2[1].r.left + button2[1].r.right) / 2;
    button2[1].r.centery =
    (button2[1].r.bot + button2[1].r.top) / 2;
    strcpy(button2[1].text, "Quit");
    button2[1].down = 0;
    button2[1].click = 0;
    button2[1].color[0] = 0.0f;
    button2[1].color[1] = 0.0f;
    button2[1].color[2] = 0.0f;
    button2[1].dcolor[0] = button2[1].color[0] * 0.5f;
    button2[1].dcolor[1] = button2[1].color[1] * 0.5f;
    button2[1].dcolor[2] = button2[1].color[2] * 0.5f;
    button2[1].text_color = 0x00ffffff;
}

void gameover_render(void)
{
    //add render function to render button
    for (int i = 0; i < GAMEOVER_BUTTONS; i++) {
        if (button2[i].over) {
            //create the highlight color of button
            glColor3f(1.5f, 0.0f, 1.0f);
            //draw a highlight around the button
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
            glVertex2i(button2[i].r.left-2,  button2[i].r.bot-2);
            glVertex2i(button2[i].r.left-2,  button2[i].r.top+2);
            glVertex2i(button2[i].r.right+2, button2[i].r.top+2);
            glVertex2i(button2[i].r.right+2, button2[i].r.bot-2);
            glVertex2i(button2[i].r.left-2,  button2[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        glBegin(GL_QUADS);
        if (button2[i].down) {
            glColor3fv(button2[i].dcolor);
        } else {
            glColor3fv(button2[i].color);
        }
        glVertex2i(button2[i].r.left,  button2[i].r.bot);
        glVertex2i(button2[i].r.left,  button2[i].r.top);
        glVertex2i(button2[i].r.right, button2[i].r.top);
        glVertex2i(button2[i].r.right, button2[i].r.bot);
        glEnd();
        Rect r;
        r.left = button2[i].r.centerx;
        r.bot  = button2[i].r.centery-8;
        r.center = 1;
        if (button2[i].down) {
            ggprint16(&r, 0, button2[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, button2[i].text_color, button2[i].text);
        }
    }
}
void check_button2(XEvent *e, Game& g)
{
    static int savex = 0;
    static int savey = 0;
    int x,y,i;
    int lbutton=0;
    int rbutton=0;
    //when mouse button is release
    if (e->type == ButtonRelease) {
        mouse_click(2, g);
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
    for (i=0; i<GAMEOVER_BUTTONS; i++) {
        button2[i].over=0;
        button2[i].down=0;
        
        if (x >= button2[i].r.left &&
            x <= button2[i].r.right &&
            y >= button2[i].r.bot &&
            y <= button2[i].r.top) {
            button2[i].over=1;
            break;
        }
    }
    if (lbutton)
        mouse_click(1, g);
    if (rbutton)
        mouse_click(2, g);
    
}

void game_credits(void) 
{
	g.creditFrame = 0;
	Timers timers;
	timers.recordTime(&timers.creditTime);
	static Rect r;
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = 
		timers.timeDiff(&timers.creditTime, &timers.timeCurrent);
	if (timeSpan > g.delay) { 
		++g.creditFrame;
		if (g.creditFrame >= 10)
			g.creditFrame -= 1;
		timers.recordTime(&timers.creditTime);
	}
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_LIGHTING);
	r.bot = g.yres++/2 -100;	
	//r.bot = g.yres++;
	r.left = g.xres/2;
	r.center = 1;
	ggprint40(&r, 16, 0x00ffffff, "Drunk Driver");
	for (int i = 0; i < 3; i++) {
		ggprint40(&r, 16, 0x00ffffff, " ");
	}
	ggprint40(&r, 16, 0x00ffffff, "CMPS 3350 - SUMMER ");
	for (int i = 0; i < 3; i++) {
		ggprint40(&r, 16, 0x00ffffff, " ");
	}
	ggprint16(&r, 16, 0x00ffffff, "Game Developers");
	ggprint16(&r, 16, 0x00ffffff, "-----------------");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Alexander Nguyen");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Christian R");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Dave R");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Framework by Professor Gordon");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Copyrighted 2017 Drunk Driver");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Thank you for playing");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint40(&r, 16, 0x00ffffff, " ");
	ggprint16(&r, 16, 0x00ffffff, "Press Esc to Exit ");
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void drawstreetLight(void)
{
	float stagger = 5.0;
	for (int i = 0; i< 60; i++) {
		//Poles - Left
		glPushMatrix();
		glTranslatef(6.0f, 1.5f, (float)(-i*10));
		box(0.2, 5.0, 0.2);
		glPopMatrix();
		//Poles - Right
		glPushMatrix();
		glTranslatef(-6.0f, 1.5f, (float)(-i*10)-stagger);
		box(0.2, 5.0, 0.2);
		glPopMatrix();
		//Lights - Right
		glPushMatrix();
		glTranslatef((float)(6.0-(2.5/2.0)+0.1), 4.0f,
			 (float)-i*10);
		box(2.5, 0.2, 0.2);
		glPopMatrix();
		//Lights - Left
		glPushMatrix();
		glTranslatef((float)(-6.0+(2.5/2.0)-0.1), 4.0f, 
			(float)(-i*10)-stagger);
		box(2.5, 0.2, 0.2);
		glPopMatrix();
		}
}
void drawLightSource(void) 
{
	float stagger = 5.0;
        for (int i = 0; i <60; i++) {
		glColor3f(255.0, 255.0, 102);
		glPushMatrix();
		glTranslatef((float)(-6.0+(4.8/2.0)-0.1), 3.9f, 
			(float)(-i*10)-stagger);
		box(0.21, 0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef((float)(6.0-(4.8/2.0)+0.1), 3.9f,
			(float)-i*10);
		box(0.21, 0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef((float)(6.0-(4.8/2.0)+0.1), 3.9f,
			(float)-i*10);
		box(0.21, 0.2, 0.2);
		glPopMatrix();


	}
}
void drawPowerLines(void)
{
	for (int i = 0; i < 2; i++) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glPushMatrix();
		glTranslatef((i==0?1:-1)*10.0f, 4.7f, (float)-75);
		//Middle
		box(1.0/16.0, 1.0/16.0, 150);
		//Right 
		glTranslatef(0.5f, 0.0f, (float)0);
		box(1.0/16.0, 1.0/16.0, 150);
		//Left
		glTranslatef(-1.0f, 0.0f, (float)0);
		box(1.0/16.0, 1.0/16.0, 150);
		glPopMatrix();
	}
	glColor3f(0.72f, 0.42f, 0.28f);
	for (int i = 0; i < 30; i++) {
		//Poles
		glPushMatrix();
		glTranslatef(10.0f, -0.5f, (float)-i*20);
		box(0.5, 10, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-10.0f, -0.5f, (float)-i*20);
		box(0.5, 10, 0.2);
		glPopMatrix();
		//Chassis
		glPushMatrix();
		glTranslatef(10.0f, 4.7f, (float)-i*20);
		box(2.5, 0.5, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-10.0f, 4.7f, (float)-i*20);
		box(2.5, 0.5, 0.2);
		glPopMatrix();
	}
}


void drawSun(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0, 0, -10);
    int i, x, y;
    double radius = 0.30;
    glColor3ub(253, 184, 19);     
    double Pi = 2.0 * 3.142;
    x = 0, y = 0;
    glBegin(GL_TRIANGLE_FAN); 
    glVertex2f(x, y); 
    for (i = 0; i <= 20; i++)   {
        glVertex2f (
            (x + (radius * cos(i * Pi / 20))), (y + (radius * sin(i * Pi / 20)))
            );
    }
    glEnd(); 
}
