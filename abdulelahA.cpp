// Abdulelah's CPP File
// June/14/2017
// Draw street will be declared here 
// June/24
// Obstacles and and function that related
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
//#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "log.h"
#include "fonts.h"
#include "game.h"
#include "ppm.h"

#define OBSTACLE_RENDER_DIST 100

void box(float w1, float h1, float d1) 
{ 
	float w=w1*0.5; 
	float d=d1*0.5; 
	float h=h1*0.5; 
	//notice the normals being set 
	glBegin(GL_QUADS); 
	//top 
	glNormal3f( 0.0f, 1.0f, 0.0f); 
	glVertex3f( w, h,-d); 
	glVertex3f(-w, h,-d); 
	glVertex3f(-w, h, d); 
	glVertex3f( w, h, d); 
	// bottom 
	glNormal3f( 0.0f, -1.0f, 0.0f); 
	glVertex3f( w,-h, d); 
	glVertex3f(-w,-h, d); 
	glVertex3f(-w,-h,-d); 
	glVertex3f( w,-h,-d); 
	// front 
	glNormal3f( 0.0f, 0.0f, 1.0f); 
	glVertex3f( w, h, d); 
	glVertex3f(-w, h, d); 
	glVertex3f(-w,-h, d); 
	glVertex3f( w,-h, d); 
	// back 
	glNormal3f( 0.0f, 0.0f, -1.0f); 
	glVertex3f( w,-h,-d); 
	glVertex3f(-w,-h,-d); 
	glVertex3f(-w, h,-d); 
	glVertex3f( w, h,-d); 
	// left side 
	glNormal3f(-1.0f, 0.0f, 0.0f); 
	glVertex3f(-w, h, d); 
	glVertex3f(-w, h,-d); 
	glVertex3f(-w,-h,-d); 
	glVertex3f(-w,-h, d); 
	// Right side 
	glNormal3f( 1.0f, 0.0f, 0.0f); 
	glVertex3f( w, h,-d); 
	glVertex3f( w, h, d); 
	glVertex3f( w,-h, d); 
	glVertex3f( w,-h,-d); 
	glEnd(); 
}

unsigned char *buildAlphaData(Ppmimage *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initobstacle(Game& g) 
{

//	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	system("convert ./images/cat.png ./images/cat.ppm");
	g.obstacleImage = ppm6GetImage("./images/cat.ppm");
	int w = g.obstacleImage->width;
	int h = g.obstacleImage->height;

	glGenTextures(1 , &g.obstacleTexture);
	glBindTexture(GL_TEXTURE_2D, g.obstacleTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    
	unsigned char *catData = buildAlphaData(g.obstacleImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGB, GL_UNSIGNED_BYTE, catData);
}


void obstacles(Game& g)
{
	Timers timers;	
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.obstacleTime, &timers.timeCurrent);
	if (timeSpan > g.delay) {
		++g.obstacleFrame;
		if (g.obstacle >= 4) 
			g.obstacleFrame -= 4;
		timers.recordTime(&timers.obstacleTime);
	}
	printf("An obstacle can be seen on the road\n");
	/*
	//Object is within renderable space
	double width = calculateWidth();
	double height = calculateHeight();
	//Ppmimage sprite;
	glBindTexture(GL_TEXTURE_2D, sprite);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	//Assumes one row for frames right now
	static int frame = 0;
	if (frame >= frameCount)
	frame = 0;;
	int ix = frame % frameCount;
	int iy = 0;
	float tx = (float)ix / frameCount;
	float ty = (float)iy / 1.0;
	glBegin(GL_QUADS);
	glTexCoord2f(tx,      ty+.5); glVertex2i(cx-w, cy-h);
	glTexCoord2f(tx,      ty);    glVertex2i(cx-w, cy+h);
	glTexCoord2f(tx+.125, ty);    glVertex2i(cx+w, cy+h);
	glTexCoord2f(tx+.125, ty+.5); glVertex2i(cx+w, cy-h);
	glEnd();

	frame++;


*/
	float x = g.xres/2.0;
	float y = g.yres/2.0;
	float h = 120.0;
	float w = h;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, g.obstacleTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	int ix = g.obstacleFrame % 2 ;
	int iy = 4;
	if (g.obstacleFrame >=2)
		iy=1;
	float tx = (float)ix / 2.0;
	float ty = (float)iy / 2.0;
	glBegin(GL_QUADS);
			glTexCoord2f(tx,		ty+.25); glVertex2i(x-w, y-h);
			glTexCoord2f(tx,		ty); 	glVertex2i(x-w, y+h);
			glTexCoord2f(tx+.512,	ty); 	glVertex2i(x+w, y+h);
			glTexCoord2f(tx+.512,	ty+.25); glVertex2i(x+w, y-h);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}

void RoadObstacle::init(std::string spriteLoc, int frameWidth, int frameHeight)
{
	//Loads up the sprite for the class type
	spriteLocation = spriteLoc;
	frameColumns = frameWidth;
	frameRows = frameHeight;
	sprite = ppm6GetImage(spriteLocation.c_str());
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *texData = buildAlphaData(sprite);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->width, sprite->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texData);
	free(texData);
	unlink(spriteLocation.c_str());
}

void RoadObstacle::render(Game& g)
{
	//This function renders the object on the road. Can be called on all
	//Objects, because it tests if they’re in the road
	if (g.distanceTraveled > roadPositionDistance) {
		//Object has been passed
		return;
	}
	if (g.distanceTraveled < roadPositionDistance - OBSTACLE_RENDER_DIST) {
		//Object is too far ahead to see
		return;
	}
	printf("An obstacle can be seen on the road\n");
	//Object is within renderable space
	double width = calculateWidth();
	double height = calculateHeight();
	printf("unfinished%f%f", width, height);
	//Ppmimage sprite => Part of class; no definition needed
	/*
	//
	glBindTexture(GL_TEXTURE_2D, sprite);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int frameCount = frameRows * frameColumns
	static int frame = 0;
	if (frame >= frameCount)
		frame = 0;;
	int ix = frame % frameRows;
	int iy = frame % frameColumns;
	float tx = (float)ix / frameCount;
	float ty = (float)iy / 1.0;
	glBegin(GL_QUADS);
		glTexCoord2f(tx,      ty+.5); glVertex2i(cx-w, cy-h);
		glTexCoord2f(tx,      ty);    glVertex2i(cx-w, cy+h);
		glTexCoord2f(tx+.125, ty);    glVertex2i(cx+w, cy+h);
		glTexCoord2f(tx+.125, ty+.5); glVertex2i(cx+w, cy-h);
	glEnd();
	frame++;
	//TODO
	*/
}
double RoadObstacle::calculateWidth()
{
	//TODO
	return 0.0;
}
double RoadObstacle::calculateHeight()
{
	//TODO
	return 0.0;
}
void CatObstacle::triggerHitEffects()
{
	//TODO Add sound effects?
	ControlManager::playAnimationHit();
}
