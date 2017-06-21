//Christian Russell
#include <math.h>
#include <stdio.h>
#include <ctime>
#include "game.h"
#define BASIC_MOVEMENT 0.1
#define MAX_MOVEMENT 1
#define MIN_MOVEMENT 0.5
#define ROAD_WIDTH 4
/* 
	int turnLeft, turnRight, slowDown, speedUp;
	void setDefaultControls();
	void loadCustomControls();
*/
//bool movingLeft, movingRight, slowingDown, speedingUp;
bool ControlManager::movingLeft,
     ControlManager::movingRight,
     ControlManager::slowingDown,
     ControlManager::speedingUp;


void ControlManager::applyDrunkSwerve(Game& g)
{
	double swerveMovement = 
		sin(
		g.cameraPosition[2] * 
		ControlManager::calculateSwerveModifier(g.inebriationLevel)
		) * .05;
	//printf("%f\n", g.cameraPosition[0]);
	//g.cameraPosition[0] += sin(g.cameraPosition[2] / 4) * calculateSwerveModifier(g.inebriationLevel);	
	if(ControlManager::movingLeft) {
		if(swerveMovement > 0) {		
			swerveMovement = -1 * BASIC_MOVEMENT / 2;
		}
	}else if(ControlManager::movingLeft) {
		if(swerveMovement > 0) {		
			swerveMovement =  BASIC_MOVEMENT / 2;
		}
	
	}else {
	
	}
	g.cameraPosition[0] += swerveMovement;
	
}

void ControlManager::moveForward(Game& g)
{
	g.cameraPosition[2] -= g.speed;
	g.lightPosition[2] = g.cameraPosition[2];
	if(movingLeft){
		g.cameraPosition[0] -= 0.1;
	}
	if(movingRight){
		g.cameraPosition[0] += 0.1;
	}
	if(speedingUp){
		if(g.speed < MAX_MOVEMENT){
			g.speed *= 1.1;
		}
	}
	if(slowingDown){
		if(g.speed > MIN_MOVEMENT){
			g.speed *= 0.95;
		}
	}
	if(g.cameraPosition[2] <= -100){
		g.cameraPosition[2] = 2;
	}
}
double ControlManager::calculateSwerveModifier(int inebriationLevel)
{
	return 0.05 * inebriationLevel;
}

void ControlManager::applyControls(Game& g, int key, bool isPress)
{
	switch(key) {
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
		case XK_Escape:
			printf("Key is pressed: %s (%d)\n", "Escape", key);
			g.done = 1;
			break;
	}
}
void ControlManager::checkBounds(Game& g){
	if(g.cameraPosition[0] > ROAD_WIDTH){
		g.cameraPosition[0] = ROAD_WIDTH;
	}
	if(g.cameraPosition[0] < -1 * ROAD_WIDTH){
		g.cameraPosition[0] = -1 * ROAD_WIDTH;
	}
}

