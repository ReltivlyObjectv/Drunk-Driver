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
#define CAMERA_HEIGHT 1

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
	return 0.05 * sin(g.cameraPosition[2] * 0.25)  * inebriationLevelModifier;
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
			g.cameraPosition[1] = CAMERA_HEIGHT;
			return;
		} else {
			g.cameraPosition[1] = CAMERA_HEIGHT * 1.5;
		}
		progress += .1;
		
		//g.cameraPosition[1] = CAMERA_HEIGHT + (sin(progress) / 2);
		g.up[1] = -0.2 * sin(progress);
		g.cameraPosition[1] = CAMERA_HEIGHT + (sin(progress) * .65);
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
	//Vertical position
	sprintf(buffer, "Road Position (Vertical): %.3f", g.cameraPosition[1]);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Controls
	ggprint8b(&debugStats, 16, 0x0000FF00, "W/Up - Speed Up");
	ggprint8b(&debugStats, 16, 0x0000FF00, "S/Down - Slow Down");
	ggprint8b(&debugStats, 16, 0x0000FF00, "A/Left - Speed Up");
	ggprint8b(&debugStats, 16, 0x0000FF00, "D/Right - Turn Right");
	ggprint8b(&debugStats, 16, 0x0000FF00, "H - Hit Animation Test");
}
