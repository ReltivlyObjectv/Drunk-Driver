/*Author: Alexander Nguyen
Modify by: Alexander Nguyen
Date:6/17/2017

Purpose: Create a start menu that will necessary function
also have it connect to php file and upload score.
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <cstdlib>  
#include <unistd.h> 
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include "log.h"
#include "ppm.h"
  
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix [4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]

void print_group()
{
  string group[] = {"Alex", "Dave", "Christian", "Abdullah"};
}

struct Vec { 
    float x, y, z;
};

struct Shape {
  float width, height;
	float radius;
	Vec center;
};

class onclick {
  public: 
    Rect r;
    char name[15];
    int mouse[2];
}onclick;



