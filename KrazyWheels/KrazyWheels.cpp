// KrazyWheels.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "KrazyEngine\KrazyEnginePolicy_01Phase\KrazyWheelsRendrer.h"
#include "KrazyEngine\KrazyEnginePolicy_01Phase\GLeish.h"
#include "KrazyEngine\KrazyEnginePolicy_01Phase\KrazyViewPortContext.h"
#include "KrazyEngine\KrazyEnginePolicy_01Phase\KrazyBackgroundTrack.h"


static KrazyWheelsPtr g_Wheels;

#define VIEWPORT_WIDTH  800
#define VIEWPORT_HEIGHT 600

static bool _first_play = true;

void onResize(int w, int h)
{
	if(_first_play){
		_first_play = false;
		KrazyBackgroundTrack::getDefault()->play(BACKGROUND_TRACK);
	}
	g_Wheels._rendrer()->on_resize(w , h);
	glutReshapeWindow( VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
}
//
void onDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  g_Wheels._rendrer()->on_render();
  glutSwapBuffers();
}
//
void onTimer(int value)
{
	if (0 != value) {
      	onDisplay();
    }
    //glutTimerFunc(10000, onTimer, 1);
}
//
void onIdle()
{
	glutPostRedisplay();

}
//
void onClick(int button, int state, int x, int y)
{
	if ((state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON)){
		//..,
		float _width(0.0f);float _height(0.0f);
		float _x = KrazyCaertsianConverter::get_x(x , int(VIEWPORT_WIDTH));
		float _y = KrazyCaertsianConverter::get_y(y , int(VIEWPORT_HEIGHT));
		//..,
		g_Wheels._rendrer()->on_touch(_x , _y);
    }
}


int main(int argc, char* argv[])
{
 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
   
  
  glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
  glutCreateWindow("Krazy r Wheels");
  
  /* Extension wrangler initialising */
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return 0;
  }
  g_Wheels._rendrer()->on_surface_created();
  glutMouseFunc(onClick); 
  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onResize);
  glutIdleFunc(onIdle);
  glutTimerFunc(10000, onTimer, 0);
  glutMainLoop();
	
  return 0;
}

