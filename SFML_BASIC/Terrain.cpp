//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
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

using namespace std;

Terrain::Terrain(void)
{
	gridWidth=100;
	gridDepth=100;

	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	vertices=NULL;
	colors=NULL;	
	
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;

	 wireMap = false;
}


Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
}

//interpolate between two values
float lerp(float start, float end, float t){
	return start+(end-start)*t;
}


void Terrain::setPoint(vector v,float x, float y, float z){

		v[0]=x;
		v[1]=y;
		v[2]=z;
}

//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(float x, float y){

	//for the sample we will calculate height based on distance form origin
	float dist=sqrt(x*x+y*y);

	//center will be the highest point
	dist=30-dist;
	//put a nice curve in it
	dist*=dist;
	dist*=dist;
	//whoah, way to high, make it smaller
	dist/=50000;

	return dist;
}
float  Terrain::getHeight2(float y){
	return y / 20;
}

void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices=new vector[numVerts];
	delete [] colors;
	colors=new vector[numVerts];
	if (!m_heightmap.loadFromFile("map2.png"))
	{
		cout << "height did not load " << endl;
	}
	if (!water.loadFromFile("water.png"))
	{
		// error...
		cout << "water did not load " << endl;
	}
	if (!grass.loadFromFile("grass.png"))
	{
    // error...
		cout << "grass did not load  " << endl;
	}
	if (!snow.loadFromFile("snow.png"))
	{
    // error...
		cout << "snow did not load " << endl;

	}
	

	//interpolate along the edges to generate interior points
	for(int i=0;i<gridWidth-1;i++){ //iterate left to right
		for(int j=0;j<gridDepth-1;j++){//iterate front to back
			int sqNum=(j+i*gridDepth);
			int vertexNum=sqNum*3*2; //6 vertices per square (2 tris)
			float front=lerp(-terrDepth/2,terrDepth/2,(float)j/gridDepth);
			float back =lerp(-terrDepth/2,terrDepth/2,(float)(j+1)/gridDepth);
			float left=lerp(-terrWidth/2,terrWidth/2,(float)i/gridDepth);
			float right=lerp(-terrDepth/2,terrDepth/2,(float)(i+1)/gridDepth);
			int mapWidth = m_heightmap.getSize().x;
			int mapHeight = m_heightmap.getSize().y;
			sf::Color frontleftcol  = m_heightmap.getPixel((i ) * mapWidth / gridWidth ,(j ) * mapHeight / gridDepth);
			sf::Color frontrightcol = m_heightmap.getPixel((i +1 ) * mapWidth / gridWidth ,(j) * mapHeight / gridDepth);
			sf::Color backleftcol= m_heightmap.getPixel((i ) * mapWidth / gridWidth ,(j + 1) * mapHeight / gridDepth);
			sf::Color  backrightcol= m_heightmap.getPixel((i + 1 ) * mapWidth / gridWidth ,(j + 1) * mapHeight / gridDepth);
			float BACKLEFT = (backleftcol.r + backleftcol.g + backleftcol.b) / 3;
			float BACKRIGHT = (backrightcol.r + backrightcol.g + backrightcol.b) / 3;
			float FRONTLEFT = (frontleftcol.r + frontleftcol.g + frontleftcol.b) / 3;
			float FRONTRIGHT = (frontrightcol.r + frontrightcol.g + frontrightcol.b) / 3;
			/*
			back   +-----+	looking from above, the grid is made up of regular squares
			       |tri1/|	'left & 'right' are the x cooded of the edges of the square
				   |   / |	'back' & 'front' are the y coords of the square
				   |  /  |	each square is made of two trianlges (1 & 2)
				   | /   |	
				   |/tri2|
			front  +-----+
			     left   right
				 */
			//tri1
		
			setPoint(colors[vertexNum],0,FRONTLEFT,0);
			setPoint(vertices[vertexNum++],left,getHeight2(FRONTLEFT),front);

			setPoint(colors[vertexNum],0,FRONTRIGHT,0);
			setPoint(vertices[vertexNum++],right,getHeight2(FRONTRIGHT),front);

			setPoint(colors[vertexNum],0,BACKRIGHT,0);
			setPoint(vertices[vertexNum++],right,getHeight2(BACKRIGHT),back);


			//declare a degenerate triangle
			//TODO: fix this to draw the correct triangle
			setPoint(colors[vertexNum],0,FRONTLEFT,0);
			setPoint(vertices[vertexNum++],left,getHeight2(FRONTLEFT),front);

			setPoint(colors[vertexNum],0,BACKRIGHT,0);
			setPoint(vertices[vertexNum++],right,getHeight2(BACKRIGHT),back);

			setPoint(colors[vertexNum],0,BACKLEFT,0);
			setPoint(vertices[vertexNum++],left,getHeight2(BACKLEFT),back);
			
			//float average = (leftcol.r + leftcol.g + leftcol.b) / 3;
			//cout << " R " <<leftcol.r << " G " << leftcol.g << " B " << leftcol.b <<  " Average : " << average << endl;
			


		}
	}




}

bool Terrain::getWireMeash(){return wireMap;}
void Terrain::setWireMesh(bool val){wireMap = val;}
void Terrain::Draw(){
	
	
	
	if(wireMap)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glBegin(GL_TRIANGLES);
	for(int i =0;i<numVerts;i++)
	{
		glColor3fv(colors[i]);
		glVertex3fv(vertices[i]);
	}
	glEnd();

	

	
	
}