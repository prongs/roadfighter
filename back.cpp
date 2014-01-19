//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe 
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep.
#include "texture.h"
/* ascii code for the escape key */
#define ESCAPE 27

int window; 
int keys[256];
int count = 0; 
register float part=0.0;
int objects[100][2];

extern int texture[5];
register float left=-0.06;
register right=0.06;
const float shift=0.3f,length=0.6f;
float turn=0.0,len=0.0;

void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
    LoadGLTextures("Data/lesson6/background.bmp",0);				// Load The Texture(s) 
    LoadGLTextures("Data/lesson6/car3.bmp",1);
    LoadGLTextures("Data/lesson6/car_burning.bmp",2);
    LoadGLTextures("Data/lesson6/pagal.bmp",3);			
    LoadGLTextures("Data/lesson6/ullu.bmp",4);
			
    glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Clear The Background Color To Blue 
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);			// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix
    
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
    
    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
	Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
    int i=0;
    static int burning=0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();				// Reset The View

    glTranslatef(0.0f,0.0f,-5.0f);              // move 5 units into the screen.
    if(len<length)
        len=part;

    glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.

    glBegin(GL_POLYGON);		                // begin drawing a cube
    
    glTexCoord2f(0.0f, part); glVertex3f(-1.5f, -1.5f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, part); glVertex3f( 1.5f, -1.5f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.5f, 1.5f-3.0*part,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5f, 1.5f-3.0*part,  1.0f);	// Top Left Of The Texture and Quad
    glEnd();
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f, 1.5-3.0*part,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 1.5-3.0*part,  1.0f);	// Bottom Right Of The Texture and Quad
   // glTexCoord2f(1.0f, len); glVertex3f( 1.5f+len*shift/length, 1.5f-3.0f*part+3.0*len,  1.0f);	// Top Right Of The Texture and Quad


    glTexCoord2f(1.0f,part); glVertex3f(1.5,1.5f,1.0f);
    glTexCoord2f(0.0f,part); glVertex3f(-1.5,1.5f,1.0f);

    glEnd();
    if(!burning)   
        glBindTexture(GL_TEXTURE_2D,texture[1]);
    else
        glBindTexture(GL_TEXTURE_2D,texture[2]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(left, -1.2f,  1.1f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(right, -1.2f,  1.1f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(right, -1.2+0.24,  1.1f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(left, -1.2+0.24,  1.1f);	// Top Left Of The Texture and Quad    
                      
    glEnd();              
    
    
    if(!burning)
    {
	    if(keys['w'])
		{part+=0.002;}
	    if(keys['a'])
		if(left >=-0.655)
	    	{	left-=0.003;	right-=0.003;	}
		else
		    burning=1;
	    if(keys['d'] )
		if( right<=0.695)
		{	right+=0.003;	left+=0.003;	}
		else
		     burning=1;
	    if(part>=1.0f)
		part=0.0;
	    else if(part<=0.0)
		part=1.0;
    }    
    // since this is double buffered, swap the buffers to display what just got drawn.
    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* avoid thrashing this procedure */
   // usleep(100);
keys[key]=1;
	//printf("%d  %d  %d  %d",'c','v','c'|'v',(int)('c')==(int)('c'|'v'));
    /* If escape is pressed, kill everything. */
    if (key == ESCAPE ) 
    { 
	/* shut down our window */
	glutDestroyWindow(window); 
      
      /* exit the program...normal termination. */
	exit(0);                   
    }
}
void keyup(unsigned char key,int x,int y)
{
keys[key]=0;
}

int main(int argc, char **argv) 
{  
    /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
       X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */  
    glutInit(&argc, argv);  

    /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffer */  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

    /* get a 640 x 480 window */
    glutInitWindowSize(640, 480);  

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);  

    /* Open a window */  
    window = glutCreateWindow("The first game we made!!!");  

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);  

    /* Go fullscreen.  This is as soon as possible. */
    glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);
    glutKeyboardUpFunc(&keyup);

    /* Initialize our window. */
    InitGL(640, 480);
  
    /* Start Event Processing Engine */  
    glutMainLoop();  

    return 1;
}

