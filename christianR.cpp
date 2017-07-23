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
//--Added function to blur the screen for a period of time

#include <math.h>
#include <stdio.h>
#include <ctime>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>

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
#define FPS 50
#define OBSTACLE_DEPTH 1
#define OBSTACLE_WIDTH 1
#define OBSTACLE_RENDER_DIST 100
#define CAMERA_HEIGHT 1
#define COOLDOWN_DRINK 50
#define COOLDOWN_BAC .000008
#define BAC_PER_BEER .025
#define TURN_MAX 0.1
#define TURN_SPEED 0.01
#define SPEED_MOD_FASTER .001
#define SPEED_MOD_SLOWER .005



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

double ControlManager::applyDrunkSwerve(Game& g)
{
	double swerveMovement = ControlManager::calculateSwerveModifier(g);
	if (ControlManager::movingLeft) {
		if (swerveMovement < 0) {		
			swerveMovement = -1 * BASIC_MOVEMENT / 2;
		}
	} else if (ControlManager::movingRight) {
		if (swerveMovement > 0) {		
			swerveMovement =  BASIC_MOVEMENT / 2;
		}
	} else {

	}
	return swerveMovement;

}
void ControlManager::moveForward(Game& g)
{
	if (g.gameState != UNPAUSED) {
		return;
	}
	//TODO Two Headlights as light source
	g.cameraPosition[2] -= g.speed;
	g.distanceTraveled += g.speed;
	g.lightPosition[2] = g.cameraPosition[2];
	displayHitAnimation(g);
	//Turn
	double totalTurning = 0;
	if (movingLeft && !hittingObject) {
		totalTurning -= 0.1;
	}
	if (movingRight && !hittingObject) {
		totalTurning += 0.1;
	}
	totalTurning += ControlManager::applyDrunkSwerve(g);
	g.cameraPosition[0] += totalTurning;
	//Pivot Camera
	if (!hittingObject) {
		if (totalTurning < 0) {
			g.up[0] += TURN_SPEED;
		} else if (totalTurning > 0) {
			g.up[0] -= TURN_SPEED;
		} else {
			if (g.up[0] > 0) {
				g.up[0] -= TURN_SPEED / 2;
				if (g.up[0] < 0) {
					g.up[0] = 0;
				}
			} else if (g.up[0] < 0) {
				g.up[0] += TURN_SPEED / 2;
				if (g.up[0] > 0) {
					g.up[0] = 0;
				}
			}
		}
		if (g.up[0] > TURN_MAX) {
			g.up[0] = TURN_MAX;
		}
		if (g.up[0] < -TURN_MAX) {
			g.up[0] = -TURN_MAX;
		}
	}
	//Speed
	if (speedingUp && !hittingObject) {
		if (g.speed < MAX_MOVEMENT) {
			g.speed += SPEED_MOD_FASTER;
		}
	}
	if (slowingDown && !hittingObject) {
		if (g.speed > MIN_MOVEMENT) {
			g.speed -= SPEED_MOD_SLOWER;
		}
	}
	if (g.cameraPosition[2] <= -100) {
		g.cameraPosition[2] = 2;
	}
}
double ControlManager::calculateSwerveModifier(Game& g)
{
	static bool currentlySwerving = false;
	static int inebLevel = g.getInebriationLevel();
	if (hittingObject) {
		//Cannot use tires in the air
		currentlySwerving = false;
		return 0.0;
	}
	if (currentlySwerving) {
		//Don't interrupt current swerve upon higher drunkenness
		//printf("Already swerving\n");
	} else {
		//No swerve in progress; update value
		if (inebLevel != g.getInebriationLevel()) {
			printf("Disregarding previous level: %d\n", inebLevel);
			inebLevel = g.getInebriationLevel();
			printf("Recording new level: %d\n", inebLevel);
		}
	}
	switch (inebLevel) {
		case 1:
			//Random jerking movements
			//printf("Calculating swerve for: %d\n", inebLevel);
			static double progress = 0;
			static bool backwards = false;
			if (currentlySwerving) {
				if (progress >= (3/2) * PI) {
					//swerve is over
					progress = 0;
					currentlySwerving = false;
					return 0;
				} else {
					//continue swerve
					progress += .1;
					if (backwards) {
						return -1 * (sin(progress) / 30);
					} else {
						return sin(progress) / 30;
					}
				}
			} else {
				//Random chance to start swerve
				int randNum = rand() % 1000;
				//printf("Rand num: %d\n", randNum);
				if (randNum < 5) {
					currentlySwerving = true;
					if (rand() % 2 == 1) {
						//printf("Start swerving to the right\n");
						backwards = false;
					} else {
						//printf("Start swerving to the left\n");
						backwards = true;
					}
				} else {
					//printf("Won't start swerving\n");
				}
				return 0;
			}
			break;
		case 2:
			//Consistent bobbing and weaving
			return sin(g.cameraPosition[2] / 6) / 30;
			break;
		case 3:
			//Consistent bobbing and weaving and fade to black
			return sin(g.cameraPosition[2] / 4.5) / 30;
			break;
		case 4:
			//Random blackouts
			if (int randNum = rand() % 1000 < 5) {
				blackoutScreen(g, randNum);
			}
			return sin(g.cameraPosition[2] / 4.5) / 30;
			break;
		case 5:
			//Longer blackouts
			if (int randNum = rand() % 1000 < 8) {
				blackoutScreen(g, randNum);
			}
			return sin(g.cameraPosition[2] / 4.5) / 30;
			break;
		case 6:
			//Crash into side; you are dead
			static bool right = (rand() % 2 == 1) ? true : false;
			if (right) {
				return 0.5;
			} else {
				return -0.5;
			}
			return 0;
			break;
		default:
			//No modifier
			return 0;
			break;
	}
}
void ControlManager::applyControls(Game& g, int key, bool isPress)
{
	switch (key) {
		case XK_Right:
		case XK_d:
			ControlManager::movingRight = isPress;
			break;
		case XK_Left:
		case XK_a:
			ControlManager::movingLeft = isPress;
			break;
		case XK_Up:
		case XK_w:
			ControlManager::speedingUp = isPress;
			break;
		case XK_Down:
		case XK_s:
			ControlManager::slowingDown = isPress;
			break;
		case XK_h:
			if (isPress && g.gameState == UNPAUSED) {
				ControlManager::playAnimationHit();
			}
			break;
		case XK_j:
			if (isPress && g.gameState == UNPAUSED) {
				if (g.cooldownDrink == 0) {
					printf("Key is pressed: %s (%d)\n", "Drinking a nice, cold bev", key);
					g.bloodAlcoholContent += BAC_PER_BEER;
					g.minimumBAC += BAC_PER_BEER * .33;
					g.cooldownDrink = COOLDOWN_DRINK;
				}			
			}
			break;
		case XK_p:
			if (isPress) {
				if (g.gameState == PAUSED) {
					g.gameState = UNPAUSED;
				} else if (g.gameState == UNPAUSED) {
					g.gameState = PAUSED;
				}
			}
			break;
		case XK_r:
			reset(&g);
			break;
		case XK_Escape:
			printf("Key is pressed: %s (%d)\n", "Escape", key);
			g.done = 1;
			break;
	}
}
void ControlManager::checkBounds(Game& g)
{
	if (g.gameState != UNPAUSED) {
		return;
	}
	if (g.cameraPosition[0] > ROAD_WIDTH) {
		printf("Gameover!\n");
		g.cameraPosition[0] = ROAD_WIDTH;
		ControlManager::movingLeft = ControlManager::movingRight = false;
		g.gameState = GAMEOVER;
	}
	if (g.cameraPosition[0] < -1 * ROAD_WIDTH) {
		printf("Gameover!\n");
		g.cameraPosition[0] = -1 * ROAD_WIDTH;
		ControlManager::movingLeft = ControlManager::movingRight = false;
		g.gameState = GAMEOVER;
	}
}
void ControlManager::playAnimationHit()
{
	//Called by external functions to begin playing the animation
	if (hittingObject) {
		//printf("Hit animation already in progress\n");
		return;
	} else {
		//printf("Playing hit animation\n");
		hittingObject = true;
	}
}
void ControlManager::displayHitAnimation(Game& g, bool restart)
{
	//Called every frame to display any hit animations
	static double progress = 0.0;
	if (restart) {
		progress = 0;
		hittingObject = false;
	}
	if (hittingObject) {
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
void ControlManager::reset(Game* g)
{
	movingLeft = 
		movingRight = 
		slowingDown = 
		speedingUp = false;
	g->up[0] = 0;
	g->up[1] = 0;
	g->up[2] = 1;
	MakeVector(0.0, 1.0, 0.0, g->cameraPosition);
	g->speed = 0.1;
	g->distanceTraveled = 0;
	g->bloodAlcoholContent = g->minimumBAC = 0;
	g->cooldownDrink = 0;
	displayHitAnimation(*g, true);
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
	if (bloodAlcoholContent < 0.04) {
		return 0;
	} else if (bloodAlcoholContent < 0.08) {
		return 1;
	} else if (bloodAlcoholContent < .1) {
		return 2;
	} else if (bloodAlcoholContent < .14) {
		return 3;
	} else if (bloodAlcoholContent < .2) {
		return 4;
	} else if (bloodAlcoholContent < .3) {
		return 5;
	} else {
		return 6;
	}
}
std::string Game::getInebriationDescription()
{
	std::string desc;
	switch (getInebriationLevel()) {
		case 0:
			desc = "Sober";
			break;
		case 1:
			desc = "Buzzed";
			break;
		case 2:
			desc = "Legally Drunk";
			break;
		case 3:
			desc = "90's Drunk";
			break;
		case 4:
			desc = "Fucked Up";
			break;
		case 5:
			desc = "Blackout Drunk";
			break;
		case 6:
			desc = "Dead";
			break;
		default:
			desc = "Error";
	}
	return desc;
}
void Game::updateCooldowns()
{
	if (gameState == PAUSED) {
		return;
	}
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
    /*
    if (g.distanceTraveled != roadPositionDistance) {
        #ifdef DEBUG
        printf("Obstacle Pos: %f\nPlayer Pos: %f", roadPositionDistance, g.distanceTraveled);
        #endif
        return false;
    } else {
        #ifdef DEBUG
        printf("Obstacle reached on road");
        #endif
    }
    */
	if (g.distanceTraveled > roadPositionDistance + OBSTACLE_DEPTH) {
        return false;
    }
    if (g.distanceTraveled < roadPositionDistance - OBSTACLE_DEPTH) {
		return false;
	}
	if (g.cameraPosition[0] < roadPositionLR - OBSTACLE_WIDTH) {
        return false;
	}
    if (g.cameraPosition[0] > roadPositionLR + OBSTACLE_WIDTH) {
		//return false;
    }
	return true;
}
void RoadObstacle::triggerHitEffects()
{
	ControlManager::playAnimationHit();
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
    //Calculate distance
    double distanceAhead = roadPositionDistance - g.distanceTraveled;
    //Render
    glPushMatrix();
	glBegin(GL_QUADS); 
	glNormal3f( 0.0f, 0.0f, 1.0f); 
	glVertex3f( roadPositionLR + OBSTACLE_WIDTH,1, g.cameraPosition[2] - distanceAhead); 
	glVertex3f(-roadPositionLR - OBSTACLE_WIDTH,1, g.cameraPosition[2] - distanceAhead); 
	glVertex3f(-roadPositionLR - OBSTACLE_WIDTH,0, g.cameraPosition[2] - distanceAhead); 
	glVertex3f( roadPositionLR + OBSTACLE_WIDTH,0, g.cameraPosition[2] - distanceAhead); 
	glEnd(); 
    glPopMatrix();
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
void CatObstacle::triggerHitEffects()
{
	//TODO Add sound effects?
	ControlManager::playAnimationHit();
}
void blackoutScreen(Game& g, float secs)
{
	if (g.gameState == PAUSED) {
		return;
	}
	//Progress starts at 0 and ends at Pi;
	static double progress = 0;
	static double seconds = 1;
	static bool finished = true;
	static double progressPerCall;

	if (progress >= PI) {
		//Free the function up for further use
		finished = true;
		progress = 0;
		return;
	} else if (!finished) {
		//Continue blackout
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0, 0.0, 0.0, sin(progress));
		glTranslated(g.xres, g.yres, 0);
		glBegin(GL_QUADS);
		glVertex2i(-g.xres, -g.yres);
		glVertex2i(-g.xres, g.yres);
		glVertex2i(g.xres, g.yres);
		glVertex2i(g.xres, -g.yres);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		progress += progressPerCall;
		return;
	} else if (secs > 0) {
		//Start new blur
		progressPerCall = PI / (seconds * FPS);
		finished = false;
		seconds = secs;
		return;
	}
}
void drawPauseMenu(Game& g)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0,0,0,0.25);
	glTranslated(g.xres, g.yres, 0);
	glBegin(GL_QUADS);
	double w = g.xres;
	double h = g.yres;
	glBegin(GL_QUADS);
	glVertex2i(-w,-h);
	glVertex2i(-w, h);
	glVertex2i(w, h);
	glVertex2i(w,-h);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
}
#ifdef DEBUG
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
    //Camera Position
    sprintf(buffer, "Road Position (Dist): %.3f", g.cameraPosition[2]);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Vertical position
	sprintf(buffer, "Road Position (Vertical): %.3f", g.cameraPosition[1]);
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Inebriation Level
	sprintf(buffer, "Drunkness Level: %d", g.getInebriationLevel());
	ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
	//Inebriation Level
	sprintf(buffer, "You are: %s", g.getInebriationDescription().c_str());
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
	ggprint8b(&debugStats, 16, 0x0000FF00, "P - Pause");
	ggprint8b(&debugStats, 16, 0x0000FF00, "R - Restart");
}
#endif
