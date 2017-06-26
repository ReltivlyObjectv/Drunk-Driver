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
#define FPS 35
#define OBSTACLE_DEPTH 5
#define OBSTACLE_WIDTH 5
#define CAMERA_HEIGHT 1
#define COOLDOWN_DRINK 100
#define COOLDOWN_BAC .000008
#define BAC_PER_BEER .025

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
	if (movingLeft && !hittingObject) {
		g.cameraPosition[0] -= 0.1;
	}
	if (movingRight && !hittingObject) {
		g.cameraPosition[0] += 0.1;
	}
	if (speedingUp && !hittingObject) {
		if (g.speed < MAX_MOVEMENT) {
			g.speed += .001;
		}
	}
	if (slowingDown && !hittingObject) {
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
	if(hittingObject){
		//Cannot use tires in the air
		return 0.0;
	}else if (g.getInebriationLevel() == 0) {
		return 0.0;
	}
	double inebriationLevelModifier = (1 + (g.getInebriationLevel() / 3));
	return .001 * 5 * sin(g.cameraPosition[2] * 0.25)  * inebriationLevelModifier;
}
void ControlManager::applyControls(Game& g, int key, bool isPress)
{
	switch (key) {
		case XK_Right:
		case XK_d:
			if (isPress) {
//				printf("Key is pressed: %s (%d)\n", "Right", key);
			} else {
//				printf("Key released: %s (%d)\n", "Right", key);
			}
			ControlManager::movingRight = isPress;
			break;
		case XK_Left:
		case XK_a:
			if (isPress) {
//				printf("Key is pressed: %s (%d)\n", "Left", key);
			} else {
//				printf("Key released: %s (%d)\n", "Left", key);
			}
			ControlManager::movingLeft = isPress;
			break;
		case XK_Up:
		case XK_w:
			if (isPress) {
//				printf("Key is pressed: %s (%d)\n", "Speed Up", key);
//				printf("Speeding Up. Velocity: %.5f\n", g.speed);
			} else {
//				printf("Key released: %s (%d)\n", "Speed Up", key);
			}
			ControlManager::speedingUp = isPress;
			break;
		case XK_Down:
		case XK_s:
			if (isPress) {
//				printf("Key is pressed: %s (%d)\n", "Slow Down", key);
//				printf("Slowing Down. Velocity: %.5f\n", g.speed);
			} else {
			
//				printf("Key is released: %s (%d)\n", "Slow Down", key);
			}
			ControlManager::slowingDown = isPress;
			break;
		case XK_h:
			if (isPress) {
//				printf("Key is pressed: %s (%d)\n", "Hit Animation", key);
				ControlManager::playAnimationHit();
			} else {
				printf("Key released: %s (%d)\n", "Hit Animation", key);
			}
			break;
		case XK_j:
			if (isPress) {
				if (g.cooldownDrink == 0) {
					printf("Key is pressed: %s (%d)\n", "Drinking a nice, cold bev", key);
					g.bloodAlcoholContent += BAC_PER_BEER;
					g.minimumBAC += BAC_PER_BEER * .33;
					g.cooldownDrink = COOLDOWN_DRINK;
				} else {
//					printf("Key is pressed: %s (%d)\n", "Cannot drink (cooldown)", key);
				}
			} else {
//				printf("Key is released: %s (%d)\n", "Drinking button", key);
			}
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
double Game::getDistanceMiles()
{
	return distanceTraveled / SPEED_TO_MPH_MULT / FPS;
}
int Game::getInebriationLevel()
{
	if(bloodAlcoholContent < 0.08){
		return 0;
	} else if (bloodAlcoholContent < .1) {
		return 1;
	} else if (bloodAlcoholContent < .14) {
		return 2;
	} else if (bloodAlcoholContent < .2) {
		return 3;
	} else if (bloodAlcoholContent < .3) {
		return 4;
	} else {
		return 5;
	}
}
void Game::updateCooldowns()
{
	//Called every frame -- reduces each cooldown by one
	if (cooldownDrink > 0) {
		cooldownDrink--;
	}
	if (bloodAlcoholContent > minimumBAC) {
		bloodAlcoholContent -= COOLDOWN_BAC;
	}
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
	//Inebriation Level
	sprintf(buffer, "Drunkness Level: %d", g.getInebriationLevel());
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//BAC
	sprintf(buffer, "BAC: %.3f", g.bloodAlcoholContent);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//BAC
	sprintf(buffer, "Minimum BAC: %.3f", g.minimumBAC);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Drinking Cooldown
	sprintf(buffer, "Cooldown (Drinking): %d", g.cooldownDrink);
	ggprint8b(&debugStats, 16, 0x00FFA500, buffer);
	//Controls
	ggprint8b(&debugStats, 16, 0x0000FF00, "W/Up - Speed Up");
	ggprint8b(&debugStats, 16, 0x0000FF00, "S/Down - Slow Down");
	ggprint8b(&debugStats, 16, 0x0000FF00, "A/Left - Speed Up");
	ggprint8b(&debugStats, 16, 0x0000FF00, "D/Right - Turn Right");
	ggprint8b(&debugStats, 16, 0x0000FF00, "H - Hit Animation Test");
	ggprint8b(&debugStats, 16, 0x0000FF00, "J - Drink a beer");
}
