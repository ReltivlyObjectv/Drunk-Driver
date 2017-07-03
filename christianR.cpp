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

#include "glm/glm.hpp"
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
#define OBSTACLE_DEPTH 5
#define OBSTACLE_WIDTH 5
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
GLuint Object3d::loadBMP(std::string path)
{
	//Done with the assistance of http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
	//Data read from the header of the BMP file
	// Each BMP file begins by a 54-bytes header
	unsigned char header[54];
	// Position in the file where the actual data begins
	unsigned int dataPos;
	unsigned int width, height;
	//Image size is = width*height*3
	unsigned int imageSize;
	// Actual RGB data
	unsigned char* data;
	// Open the file
	FILE* file = fopen(path.c_str(),"rb");
	if (!file) {
		printf("Texture could not be opened: %s\n", path.c_str());
		working = false;
		return 0;
	}
	//If not 54 bytes read : problem
	if (fread(header, 1, 54, file) != 54) { 
		printf("Not a correct BMP file\n");
		working = false;
		return 0;
	}
	//Since BMP always starts with 'BM', check if correct type
	if (header[0]!='B' || header[1]!='M') {
		printf("Not a correct BMP file\n");
		working = false;
		return 0;
	}
	//Now we can read the size of the image, the location of the data in the file, etc
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize==0) {
		// 3 : one byte for each Red, Green and Blue component
		imageSize=width*height*3; 
	}
	if (dataPos==0) {
		// The BMP header is done that way
		dataPos=54; 
	}
	//Now that we know the size of the image, we can allocate some memory to read the image into, and read
	// Create a buffer
	data = new unsigned char [imageSize];
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
	//Everything is in memory now, the file can be closed
	fclose(file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return textureID;
}
void Object3d::loadOBJ(std::string path)
{
	//Done with the assistance of http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
	printf("Loading model: %s\n", path.c_str());
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	FILE* file = fopen(path.c_str(), "r");
	if (file == NULL) {
		printf("Model could not be opened: %s\n", path.c_str());
		working = false;
		return;
	}
	while (true) {
		char lineHeader[256];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break; // EOF = End Of File. Quit the loop.
		} else {
			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			} else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			} else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			} else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
						&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
						&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by simple parser: %s\n", path.c_str());
					working = false;
					return;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}
	}
	for (unsigned int i=0; i<vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex-1];
		vertices.push_back(vertex);
	}
	for (unsigned int i=0; i<uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex-1];
		uvs.push_back(uv);
	}
	for (unsigned int i=0; i<normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex-1];
		normals.push_back(normal);
	}
}
Object3d::Object3d(std::string path, std::string texPath, double roadPosLR, double roadPosDistance)
{
	roadPositionLR = roadPosLR;
	roadPositionDistance = roadPosDistance;
	texture = loadBMP(texPath);
	loadOBJ(path);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
}
bool Object3d::isWorking()
{
	return working;
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
	//sprintf(buffer, "Road Position (Vertical): %.3f", g.cameraPosition[1]);
	//ggprint8b(&debugStats, 16, 0x00FFFF00, buffer);
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
}
