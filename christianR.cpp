//Christian Russell
#include <math.h>
#include <stdio.h>
#include <ctime>
#include "game.h"
#define BASIC_MOVEMENT 0.1
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
	if(movingLeft){
		g.cameraPosition[0] -= 0.1;
	}
	if(movingRight){
		g.cameraPosition[0] += 0.1;
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
void ControlManager::checkBounds(Game& g){
	if(g.cameraPosition[0] > ROAD_WIDTH){
		g.cameraPosition[0] = ROAD_WIDTH;
	}
	if(g.cameraPosition[0] < -1 * ROAD_WIDTH){
		g.cameraPosition[0] = -1 * ROAD_WIDTH;
	}
}
void drawStreet()
{
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);
	float w = 5.0;
	float d = 300.0;
	float h = 0.0;
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( w, h,-d);
	glVertex3f(-w, h,-d);
	glVertex3f(-w, h, d);
	glVertex3f( w, h, d);
	glEnd();
	//double yellow line
	glColor3f(0.8f, 0.8f, 0.2f);
	w = 0.1;
	d = 300.0;
	h = 0.01;
	glPushMatrix();
	glTranslatef(-0.15f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( w, h,-d);
	glVertex3f(-w, h,-d);
	glVertex3f(-w, h, d);
	glVertex3f( w, h, d);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.15f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( w, h,-d);
	glVertex3f(-w, h,-d);
	glVertex3f(-w, h, d);
	glVertex3f( w, h, d);
	glEnd();
	glPopMatrix();
	//guard rails
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i=0; i<120; i++) {
		glPushMatrix();
		glTranslatef(6.0f, -0.5f, (float)-i*2.5);
		box(0.2, 5.0, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-6.0f, -0.5f, (float)-i*2.5);
		box(0.2, 5.0, 0.2);
		glPopMatrix();
	}
}
