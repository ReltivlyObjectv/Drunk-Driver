//Dave Rezac
//cpp files for Drunk Driving game
//Summer 2017

/////////Responsibilities/////////
//
//
/////////Updates//////////////////
//
//

#include <iostream>
#include "game.h"
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "ppm.h"

extern Game g;

using namespace std;

Ppmimage *inCarImage=NULL;
GLuint inCarTexture;
int displayDash=1;
//int w=0, h=0;
//void initOpengl(void);

//overlay in car view onto animated screen
void showInCar(void)
{
	glBindTexture(GL_TEXTURE_2D, inCarTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
  
void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	inCarImage = ppm6GetImage("inCarPic.ppm");
	glBindTexture(GL_TEXTURE_2D, inCarTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,inCarImage->width, 
		inCarImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
		inCarImage->data);
		
}

//////////////////////////////////Start here
/*
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	inCarImage = ppm6GetImage("inCarPic.ppm");
	//forestImage      = ppm6GetImage("./images/forest.ppm");
	//forestTransImage = ppm6GetImage("./images/forestTrans.ppm");
	//umbrellaImage    = ppm6GetImage("./images/umbrella.ppm");
	//
	//create opengl texture elements
	glGenTextures(1, &inCarTexture);
	//glGenTextures(1, &silhouetteTexture);
	//glGenTextures(1, &forestTexture);
	//glGenTextures(1, &umbrellaTexture);
	//-------------------------------------------------------------------------
	//bigfoot
	//
	//int w = bigfootImage->width;
	//int h = bigfootImage->height;
	//
	glBindTexture(GL_TEXTURE_2D, inCarTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, inCarImage->data);
	//-------------------------------------------------------------------------
	//
*/
////////////////////////////End here
	//silhouette
	//this is similar to a sprite graphic
	//
	/*
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(inCarImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	*/
	//-------------------------------------------------------------------------
	//
	//umbrella
	//
	/*
	glBindTexture(GL_TEXTURE_2D, umbrellaTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	silhouetteData = buildAlphaData(umbrellaImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	*/
	//-------------------------------------------------------------------------
	//
	//forest
	/*
	glBindTexture(GL_TEXTURE_2D, forestTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		forestImage->width, forestImage->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);*/
	//-------------------------------------------------------------------------
	//
	//forest transparent part
	//
	/*
	glBindTexture(GL_TEXTURE_2D, forestTransTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	w = forestTransImage->width;
	h = forestTransImage->height;
	unsigned char *ftData = buildAlphaData(forestTransImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	*/
	//-------------------------------------------------------------------------
//}
