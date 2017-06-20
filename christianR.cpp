//Christian Russell
#include <math.h>
#include "game.h"
#include <stdio.h>

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
	g.cameraPosition[0] = sin(g.cameraPosition[2] * ControlManager::calculateSwerveModifier(g.inebriationLevel));
}

void ControlManager::moveForward(Game& g)
{
	g.cameraPosition[2] -= g.speed;
}
double ControlManager::calculateSwerveModifier(int inebriationLevel)
{
	return 0.05 * inebriationLevel;
}

void ControlManager::applyControls(Game& g, int key)
{
	switch(key) {
		case XK_Right:
		case XK_d:
			printf("Key is pressed: %s (%d)\n", "Right", key);
			g.cameraPosition[0] += 0.1;
			break;
		case XK_Left:
		case XK_a:
			printf("Key is pressed: %s (%d)\n", "Left", key);
			g.cameraPosition[0] -= 5;
			movingLeft = true;
			break;
		case XK_Up:
		case XK_w:
			printf("Key is pressed: %s (%d)\n", "Speed Up", key);
			g.speed *= 1.1;
			printf("Speeding Up. Velocity: %.5f\n", g.speed);
			break;
		case XK_Down:
		case XK_s:
			printf("Key is pressed: %s (%d)\n", "Slow Down", key);
			g.speed *= 0.95;
			printf("Slowing Down. Velocity: %.5f\n", g.speed);
			break;
		case XK_Escape:
			printf("Key is pressed: %s (%d)\n", "Escape", key);
			g.done = 1;
			break;
	}
}
