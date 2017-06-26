//Author: Christian Russell
//June 20, 2017
//-Created Control Manager
//--Interprets keyboard
//--Drunk swerve calculation based on drunkness level
//--Reads keyboard and applies controls:
//--Turn left, right, and speed up, and slow down
//--Bounds checking to make sure you don't swerve off map
//--Send you to beginning of road once a certain point is reached
//---to create seamless loop
//--Makes light follow car like a headlight
//--Animation function for when an object is hit
//-Added to Game class
//--Converter for camera speed to MPH
//--Converter for distance traveled to Miles
//-Road Obstacle Class
//--This is a class that can serve as a parent class
//---type for all obstacles. Effects will be overrided
//---virtual functions
//-Other Changes
//--Added debug info screen to display important variables

#include <math.h>
#include <stdio.h>
#include <ctime>
#include <string>
#include <sstream>
#include <unistd.h>
#include "game.h"
#include "fonts.h"
#include "ppm.h"

#ifdef __APPLE__
#else
#include <malloc.h>
#endif

#define BASIC_MOVEMENT 0.1
#define MAX_MOVEMENT 1
#define MIN_MOVEMENT 0.1
#define ROAD_WIDTH 4
#define SPEED_TO_MPH_MULT 100
#define FPS 30
#define OBSTACLE_DEPTH 5
#define OBSTACLE_WIDTH 5
#define OBSTACLE_RENDER_DIST 100

bool ControlManager::movingLeft,
     ControlManager::movingRight,
     ControlManager::slowingDown,
     ControlManager::speedingUp, 
     ControlManager::hittingObject;

int RoadObstacle::frameRows,
    RoadObstacle::frameColumns;

std::string RoadObstacle::spriteLocation;
Ppmimage* RoadObstacle::sprite;
GLuint RoadObstacle::texture;

void ControlManager::applyDrunkSwerve(Game& g)
{
	double swerveMovement = ControlManager::calculateSwerveModifier(g);
		if (ControlManager::movingLeft) {
			if (swerveMovement < 0) {		
				swerveMovement = -1 * BASIC_MOVEMENT / 2;
			}
		}else if (ControlManager::movingRight) {
			if (swerveMovement > 0) {		
				swerveMovement =  BASIC_MOVEMENT / 2;
			}
		}else {

		}
	g.cameraPosition[0] += swerveMovement;

}
void ControlManager::moveForward(Game& g)
{
	//TODO Two Headlights as light source
	g.cameraPosition[2] -= g.speed;
	g.distanceTraveled += g.speed;
	g.lightPosition[2] = g.cameraPosition[2];
	displayHitAnimation(g);
	if (movingLeft) {
		g.cameraPosition[0] -= 0.1;
	}
	if (movingRight) {
		g.cameraPosition[0] += 0.1;
	}
	if (speedingUp) {
		if (g.speed < MAX_MOVEMENT) {
			g.speed += .001;
		}
	}
	if (slowingDown) {
		if (g.speed > MIN_MOVEMENT) {
			g.speed -= .005;
		}
	}
	if (g.cameraPosition[2] <= -100) {
		g.cameraPosition[2] = 2;
	}
}
double ControlManager::calculateSwerveModifier(Game& g)
{
	double inebriationLevelModifier = (1 + (g.inebriationLevel / 3));
	//printf("%f\n", g.cameraPosition[0]);
	//g.cameraPosition[0] += sin(g.cameraPosition[2] / 4) * 
	//calculateSwerveModifier(g.inebriationLevel);	
	/*
	 *  * .05;
	 */
	return 0.05 * sin(g.cameraPosition[2] / 4.0)  * inebriationLevelModifier;
}
void ControlManager::applyControls(Game& g, int key, bool isPress)
{
	switch (key) {
		case XK_Right:
		case XK_d:
			printf("Key is pressed: %s (%d)\n", "Right", key);
			ControlManager::movingRight = isPress;
			break;
		case XK_Left:
		case XK_a:
			printf("Key is pressed: %s (%d)\n", "Left", key);
			ControlManager::movingLeft = isPress;
			break;
		case XK_Up:
		case XK_w:
			printf("Key is pressed: %s (%d)\n", "Speed Up", key);
			ControlManager::speedingUp = isPress;
			printf("Speeding Up. Velocity: %.5f\n", g.speed);
			break;
		case XK_Down:
		case XK_s:
			printf("Key is pressed: %s (%d)\n", "Slow Down", key);
			ControlManager::slowingDown = isPress;
			printf("Slowing Down. Velocity: %.5f\n", g.speed);
			break;
		case XK_h:
			printf("Key is pressed: %s (%d)\n", "Hit Animation", key);
			ControlManager::playAnimationHit();
			break;
		case XK_Escape:
			printf("Key is pressed: %s (%d)\n", "Escape", key);
			g.done = 1;
			break;
	}
}
void ControlManager::checkBounds(Game& g)
{
	if (g.cameraPosition[0] > ROAD_WIDTH) {
		g.cameraPosition[0] = ROAD_WIDTH;
	}
	if (g.cameraPosition[0] < -1 * ROAD_WIDTH) {
		g.cameraPosition[0] = -1 * ROAD_WIDTH;
	}
}
void ControlManager::playAnimationHit()
{
	//Called by external functions to begin playing the animation
	if (hittingObject) {
		printf("Hit animation already in progress\n");
		return;
	} else {
		printf("Playing hit animation\n");
		hittingObject = true;
	}
}
void ControlManager::displayHitAnimation(Game& g)
{
	//Called every frame to display any hit animations
	if (hittingObject) {
		static double progress = 0.0;
		if (progress >= 3) {
			progress = 0.0;
			hittingObject = false;
			g.up[1] = 0;
			return;
		}
		progress += .1;
		g.up[1] = -0.3 * sin(progress);
	}
}
double Game::getMPH()
{
	return speed * SPEED_TO_MPH_MULT;
}
double Game::getDistanceMiles(){
	return distanceTraveled / SPEED_TO_MPH_MULT / FPS;
}
RoadObstacle::RoadObstacle(double roadPosLR, double roadPosDistance) 
{
	roadPositionLR = roadPosLR;
	roadPositionDistance = roadPosDistance;
}
void RoadObstacle::init(std::string spriteLoc, int frameWidth, int frameHeight)
{
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
bool RoadObstacle::isCameraInside(Game& g)
{
	if (g.distanceTraveled < roadPositionDistance - OBSTACLE_DEPTH
	|| g.distanceTraveled > roadPositionDistance + OBSTACLE_DEPTH
	) {
		return false;
	}
	if (g.cameraPosition[0] < roadPositionLR - OBSTACLE_WIDTH
	|| g.cameraPosition[0] > roadPositionLR + OBSTACLE_WIDTH) {
		return false;
	}
	return true;
}
void RoadObstacle::triggerHitEffects()
{
	ControlManager::playAnimationHit();
}
void RoadObstacle::render(Game& g)
{
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
	//TODO
	*/
}
double RoadObstacle::calculateWidth(){
	//TODO
	return 0.0;
}
double RoadObstacle::calculateHeight(){
	//TODO
	return 0.0;
}
void drawDebugInfo(Game& g)
{
	Rect debugStats;
	debugStats.bot = g.yres - 20;
	debugStats.left = g.xres - (175);
	debugStats.center = 0;
	char buffer[50];
	//Speed
	sprintf(buffer, "Speed: %02.3f", g.speed);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	sprintf(buffer, "Speed: %2.1f MPH", g.getMPH());
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Distance Traveled
	sprintf(buffer, "Distance Traveled: %.3f", g.distanceTraveled);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	sprintf(buffer, "Distance Traveled: %.1f Miles", g.getDistanceMiles());
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Left/Right position
	sprintf(buffer, "Road Position (R/L): %.3f", g.cameraPosition[0]);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Controls
	ggprint8b(&debugStats, 16, 0x0000FF00, "W/Up - Speed Up");
	ggprint8b(&debugStats, 16, 0x0000FF00, "S/Down - Slow Down");
	ggprint8b(&debugStats, 16, 0x0000FF00, "A/Left - Speed Up");
	ggprint8b(&debugStats, 16, 0x0000FF00, "D/Right - Turn Right");
	ggprint8b(&debugStats, 16, 0x0000FF00, "H - Hit Animation Test");
}
//From walk.cpp
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
