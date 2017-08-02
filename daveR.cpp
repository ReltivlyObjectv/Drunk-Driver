//Dave Rezac
//cpp files for Drunk Driving game
//Summer 2017

/////////Responsibilities/////////
//
//
/////////Updates//////////////////
//
//

#include <iostream>
#include "game.h"
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <list>
#include "ppm.h"
#include <stdio.h>

extern Game g;

using namespace std;

GLuint inCarTexture;
Ppmimage* sprite;
int displayDash=1;
//int w=0, h=0;
//void initOpengl(void);

//overlay in car view onto animated screen
void showInCar(Game& g)
{
    	//Next line added
        glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, inCarTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}
  
void initCarPics(void)
{
	sprite = ppm6GetImage("images/inCarPic.ppm");
	glGenTextures(1, &inCarTexture);
	glBindTexture(GL_TEXTURE_2D, inCarTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *texData = buildAlphaData(sprite);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->width, sprite->height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texData);
	free(texData);
}
void initObstacles(std::list<RoadObstacle*>& obstacles)
{
	//This is called at startup, and initializes each obstacle (must be called for every class)
	//RoadObstacle::init("images/cat.ppm", 2, 4);
	for (int i = 0; i < 1500; i+=3) {
		if (rand() % 5 == 0) {
			float leftRight = (float)((rand() % 800) - 400) / 100.0;
			switch (rand() % 9) {
				case 0:
					obstacles.push_back(new CatObstacle(leftRight,i,"images/cat.ppm", 2, 4));
					break;
				case 1: 
					obstacles.push_back(new ManObstacle(leftRight,i,"images/man.ppm"));
					break;
				case 2:
					obstacles.push_back(new VehicleObstacle(leftRight,i,"images/car.ppm"));
					break;
				case 3:
					obstacles.push_back(new BusObstacle(leftRight,i,"images/bus.ppm"));
					break;
				case 4:
				case 5:
				case 6:
					obstacles.push_back(new BeerObstacle(leftRight,i,"images/beer.ppm", 8, 1));
					break;
				case 7:
				case 8:
					obstacles.push_back(new TacoObstacle(leftRight,i,"images/taco.ppm"));
					break;
			}
		}
	}
//	obstacles.push_back(new FinishObstacle(-4,1600,"images/checkered.ppm"));
	for (int i=-4; i<6; i+=2) {
		obstacles.push_back(new FinishObstacle(i,1600,"images/checkered.ppm"));
	}
}
//Cat Obstacle
CatObstacle::CatObstacle(double roadPosLR, double roadPosDistance, 
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance, 
	spriteLoc,frameWidth, frameHeight) 
{
	//Leave Blank -- Just calls to super constructor
}
void CatObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	#ifdef DEBUG
	printf("Obstacle hit: %s\n", "Cat");
	#endif
	ControlManager::playAnimationHit();
}


//Man Obstacle
ManObstacle::ManObstacle(double roadPosLR, double roadPosDistance,
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance,
	spriteLoc, frameWidth, frameHeight)
{
	//Leave Blank -- Just calls to super constructor
}

void ManObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	#ifdef DEBUG
	ControlManager::playAnimationHit();
	printf("Obstacle hit: %s\n", "Man");
	#else
	g.gameState = GAMEOVER;
	#endif
}

//Vehicle Obstacle
VehicleObstacle::VehicleObstacle(double roadPosLR, double roadPosDistance, 
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance,
	spriteLoc, frameWidth, frameHeight)
{
	//Leave Blank -- Just calls to super constructor
}

void VehicleObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	#ifdef DEBUG
	printf("Obstacle hit: %s\n", "Vehicle");
	#endif
	g.speed /= 2.0;
}

//Bus Obstacle
BusObstacle::BusObstacle(double roadPosLR, double roadPosDistance,
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance,
	spriteLoc, frameWidth, frameHeight)
{
	//Leave Blank -- Just calls to super constructor
}

void BusObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	#ifdef DEBUG
	printf("Obstacle hit: %s\n", "Bus");
	#endif
	g.speed = 0.1;
}

//Beer Obstacle
BeerObstacle::BeerObstacle(double roadPosLR, double roadPosDistance,
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance,
	spriteLoc, frameWidth, frameHeight)
{
	//Leave Blank -- Just calls to super constructor
}

void BeerObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	#ifdef DEBUG
	printf("Obstacle hit: %s\n", "Beer");
	#endif
	ControlManager::drinkBeer(g);
}

//Taco Obstacle
TacoObstacle::TacoObstacle(double roadPosLR, double roadPosDistance, 
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance,
	spriteLoc, frameWidth, frameHeight)
{
	//Leave Blank -- Just calls to super constructor
}

void TacoObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	#ifdef DEBUG
	printf("Obstacle hit: %s\n", "Taco");
	#endif
	if (g.bloodAlcoholContent > g.minimumBAC + .005)
		g.bloodAlcoholContent -= 0.005;
	else
		g.bloodAlcoholContent = g.minimumBAC;
}
//Finish Line Obstacle
FinishObstacle::FinishObstacle(double roadPosLR, double roadPosDistance, 
	std::string spriteLoc, int frameWidth, int frameHeight) 
	: RoadObstacle(roadPosLR, roadPosDistance,
		spriteLoc, frameWidth, frameHeight)
{
	//Leave Blank -- Just calls to super constructor
}

void FinishObstacle::triggerHitEffects()
{
	//Everthing in here is executed when the player hits the obstacle
	if (isActive) {
		isActive = false;
	} else {
		return;
	}
	//Game winning condition
	g.gameState = CREDITS;
	ControlManager::reset(&g);
	g.gameState = MENU;
}


void filterRGB(float R, float G, float B) 
{
// Currently applies a colored overlay based on RGB, each from 0.0-1.0
//      glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(R, G, B);
        glBegin(GL_QUAD_STRIP);
        glVertex2f( 0.0f,  0.0f);
        glVertex2f( 0.0f, 5.1f);
        glVertex2f(5.1f,  0.0f);
        glVertex2f(5.1f, 5.1f);
        glEnd();
}

//added new section for database connection here
/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/


/*
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main(void)
{
cout << endl;
cout << "Running 'SELECT 'Hello World!' »
   AS _message'..." << endl;

try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  // Create a connection
  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
  // Connect to the MySQL test database
  con->setSchema("test");

  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT 'Hello World!' AS _message"); // replace with your statement
  while (res->next()) {
    cout << "\t... MySQL replies: ";
    // Access column data by alias or column name
    cout << res->getString("_message") << endl;
    cout << "\t... MySQL says it again: ";
    // Access column fata by numeric offset, 1 is the first column
    cout << res->getString(1) << endl;
  }
  delete res;
  delete stmt;
  delete con;

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " »
     << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}

cout << endl;

return EXIT_SUCCESS;
}

*/

//end of database connection
//go back and set up for school database

//////////////////////////////////Start here
/*
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	inCarImage = ppm6GetImage("inCarPic.ppm");
	//forestImage      = ppm6GetImage("./images/forest.ppm");
	//forestTransImage = ppm6GetImage("./images/forestTrans.ppm");
	//umbrellaImage    = ppm6GetImage("./images/umbrella.ppm");
	//
	//create opengl texture elements
	glGenTextures(1, &inCarTexture);
	//glGenTextures(1, &silhouetteTexture);
	//glGenTextures(1, &forestTexture);
	//glGenTextures(1, &umbrellaTexture);
	//-------------------------------------------------------------------------
	//bigfoot
	//
	//int w = bigfootImage->width;
	//int h = bigfootImage->height;
	//
	glBindTexture(GL_TEXTURE_2D, inCarTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, inCarImage->data);
	//-------------------------------------------------------------------------
	//
*/
////////////////////////////End here
	//silhouette
	//this is similar to a sprite graphic
	//
	/*
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(inCarImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	*/
	//-------------------------------------------------------------------------
	//
	//umbrella
	//
	/*
	glBindTexture(GL_TEXTURE_2D, umbrellaTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	silhouetteData = buildAlphaData(umbrellaImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	*/
	//-------------------------------------------------------------------------
	//
	//forest
	/*
	glBindTexture(GL_TEXTURE_2D, forestTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		forestImage->width, forestImage->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);*/
	//-------------------------------------------------------------------------
	//
	//forest transparent part
	//
	/*
	glBindTexture(GL_TEXTURE_2D, forestTransTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	w = forestTransImage->width;
	h = forestTransImage->height;
	unsigned char *ftData = buildAlphaData(forestTransImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	*/
	//-------------------------------------------------------------------------
//}
