//Christian Russell
#include <math.h>
#include "game.h"
#include <stdio.h>

/* 
 int turnLeft, turnRight, slowDown, speedUp;
 void setDefaultControls();
 void loadCustomControls();
 */
void ControlManager::setDefaultControls(){

}

void ControlManager::loadCustomControls(){
}

void ControlManager::applyDrunkSwerve(Game& g){
	g.cameraPosition[2] -= 0.1;
	g.cameraPosition[0] = 0.0 + sin(g.cameraPosition[2]*0.3);
}

void ControlManager::applyControls(Game& g, int key){
	switch(key) {
		case XK_1:
			break;
		case XK_Right:
			printf("Key is pressed: %s (%d)\n", "Left", key);
			//ControlManager::turnRight();
			g.cameraPosition[0] += 0.1;
			break;
		case XK_Left:
			printf("Key is pressed: %s (%d)\n", "Right", key);
			g.cameraPosition[0] -= 0.1;
			break;
		case XK_q:
			break;
		case XK_b:
			break;
		case XK_s:
			break;
		case XK_Escape:
			printf("Key is pressed: %s (%d)\n", "Escape", key);
			g.done = 1;
			break;
	}
}
