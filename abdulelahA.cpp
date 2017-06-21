// Abdulelah's CPP File
// June/14/2017
// Draw street will be declared here 
//
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "log.h"
#include "fonts.h"
#include "game.h"


void box(float w1, float h1, float d1) 
{ 
        float w=w1*0.5; 
        float d=d1*0.5; 
        float h=h1*0.5; 
        //notice the normals being set 
        glBegin(GL_QUADS); 
                //top 
                glNormal3f( 0.0f, 1.0f, 0.0f); 
                glVertex3f( w, h,-d); 
                glVertex3f(-w, h,-d); 
                glVertex3f(-w, h, d); 
                glVertex3f( w, h, d); 
                // bottom 
                glNormal3f( 0.0f, -1.0f, 0.0f); 
                glVertex3f( w,-h, d); 
                glVertex3f(-w,-h, d); 
                glVertex3f(-w,-h,-d); 
                glVertex3f( w,-h,-d); 
                // front 
                glNormal3f( 0.0f, 0.0f, 1.0f); 
                glVertex3f( w, h, d); 
                glVertex3f(-w, h, d); 
                glVertex3f(-w,-h, d); 
                glVertex3f( w,-h, d); 
                // back 
                glNormal3f( 0.0f, 0.0f, -1.0f); 
                glVertex3f( w,-h,-d); 
                glVertex3f(-w,-h,-d); 
                glVertex3f(-w, h,-d); 
                glVertex3f( w, h,-d); 
                // left side 
                glNormal3f(-1.0f, 0.0f, 0.0f); 
                glVertex3f(-w, h, d); 
                glVertex3f(-w, h,-d); 
                glVertex3f(-w,-h,-d); 
                glVertex3f(-w,-h, d); 
                // Right side 
                glNormal3f( 1.0f, 0.0f, 0.0f); 
                glVertex3f( w, h,-d); 
                glVertex3f( w, h, d); 
                glVertex3f( w,-h, d); 
                glVertex3f( w,-h,-d); 
                glEnd(); 
        glEnd(); 
} 


