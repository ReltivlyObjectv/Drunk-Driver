#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include "glm/glm.hpp"
#include "ppm.h"

void box (float , float , float );

//some defined macros
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a) (a)[0]=(-(a)[0]); (a)[1]=(-(a)[1]); (a)[2]=(-(a)[2]);
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a) ((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecLenSq(a) sqrtf((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c) \
	(c)[0]=(a)[0]+(b)[0];\
	(c)[1]=(a)[1]+(b)[1];\
	(c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c) \
	(c)[0]=(a)[0]-(b)[0]; \
	(c)[1]=(a)[1]-(b)[1]; \
	(c)[2]=(a)[2]-(b)[2]
#define VecS(A,a,b) (b)[0]=(A)*(a)[0]; (b)[1]=(A)*(a)[1]; (b)[2]=(A)*(a)[2]
#define VecAddS(A,a,b,c) \
	(c)[0]=(A)*(a)[0]+(b)[0]; \
	(c)[1]=(A)*(a)[1]+(b)[1]; \
	(c)[2]=(A)*(a)[2]+(b)[2]
#define VecCross(a,b,c) \
	(c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1]; \
	(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2]; \
	(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define VecZero(v) (v)[0]=0.0;(v)[1]=0.0;v[2]=0.0
#define ABS(a) (((a)<0)?(-(a)):(a))
#define SGN(a) (((a)<0)?(-1):(1))
#define SGND(a) (((a)<0.0)?(-1.0):(1.0))
#define rnd() (float)rand() / (float)RAND_MAX
#define PI 3.14159265358979323846264338327950
#define MY_INFINITY 1000.0

typedef float Flt;
typedef Flt Vec[3];
typedef Flt Matrix[4][4];

enum GameStates {
	MENU,
	PAUSED,
	UNPAUSED,
	CREDITS,
	GAMEOVER
};
class Game {
	public:
		int gameState;
		int xres, yres;
		int done;
		double speed, distanceTraveled, bloodAlcoholContent, minimumBAC;
		int cooldownDrink;
		Flt aspectRatio;
		Vec cameraPosition, up;
		GLfloat lightPosition[4];
		//
		int obstacle;
		int obstacleFrame;
		GLuint obstacleTexture;
		Ppmimage *obstacleImage;
		double delay;
		//
		Game() {
			//constructor
			gameState = MENU;
			xres=640;
			yres=480;
			aspectRatio = (GLfloat)xres / (GLfloat)yres;
			up[0] = 0;
			up[1] = 0;
			up[2] = 1;
			MakeVector(0.0, 1.0, 8.0, cameraPosition);
			//light is up high, right a little, toward a little
			MakeVector(100.0f, 240.0f, 40.0f, lightPosition);
			lightPosition[3] = 1.0f;
			done = 0;
			speed = 0.1;
			distanceTraveled = 0;
			bloodAlcoholContent = minimumBAC = 0;
			cooldownDrink = 0;
		}
		double getMPH();
		double getDistanceMiles();
		void updateCooldowns();
		int getInebriationLevel();
		std::string getInebriationDescription();
		void renderAllRoadside();
};
void showInCar(void);
void drawStreet(Game& g);
void drawPauseMenu(Game& g);
void blackoutScreen(Game& g, float secs=-1);
void drawDebugInfo(Game& g);
void check_button(XEvent *e);
unsigned char *buildAlphaData(Ppmimage *img);

//Alex's Function
void check_button(XEvent *e, Game& g);
void gamemenu(void);
void button_init(void);
void button_render(void);
void game_pause(void);
void game_over(void);
//
void box (float , float , float );
void initobstacle(Game& g);
void obstacles(Game& g);
class ControlManager {
	public:
		static bool movingLeft, movingRight, slowingDown, speedingUp, hittingObject;
		static void applyControls(Game& g, int key, bool isPress);
		static double applyDrunkSwerve(Game& g);
		static void moveForward(Game& g);
		static void checkBounds(Game& g);
		static double calculateSwerveModifier(Game& g);
		static void playAnimationHit();
	private:
		static void displayHitAnimation(Game& g);
};

//This is a parent class to be used by all road obstacles as children classes
class RoadObstacle {
	public:
		RoadObstacle(double roadPosLR, double roadPosDistance);
		static void init(std::string spriteLoc, int frameWidth=1, int frameHeight=1);
		bool isCameraInside(Game& g);
		void render(Game& g);
		virtual void triggerHitEffects();
	private:
		static int frameColumns, frameRows;
		static std::string spriteLocation;
		static Ppmimage* sprite;
		static GLuint texture;
		double roadPositionLR, roadPositionDistance;
		double calculateWidth();
		double calculateHeight();
};
class CatObstacle {
	public:
		virtual void triggerHitEffects();
};

//----------------------------------------------------------------------------

//Setup timers
class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec obstacleTime;
	Timers() {
		physicsRate = 1.0 / 30.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
} ;
//-----------------------------------------------------------------------------


#endif
