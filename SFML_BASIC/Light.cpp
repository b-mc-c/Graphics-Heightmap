#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 

#include "stdafx.h"
#include "SFML/OpenGL.hpp"
#include "Terrain.h"
#include <cmath>
#include <iostream>
#include "Light.h"

using namespace std;

Light::Light(){

}

Light::~Light(){

}

void Light::lightInit(){
	GLfloat mat_specular[]={0.0,.3f,.8f,1.0f};
	GLfloat mat_diffuse[]={0.0,1.0f,0.0f,1.0f};
	GLfloat mat_ambient[]={0.1,0.1f,0.1f,1.0f};
	GLfloat mat_shininess = 50.0;

	glClearColor(0.1, 0.1, 0.1, 0.0);
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 5.0 };
	GLfloat light_position[] = {20, 50, 0};
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	// enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	// set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLightfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glLightfv(GL_BACK, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	// glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	/*
	// Assign created components to GL_LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, position);
	*/
}