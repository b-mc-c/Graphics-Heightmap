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
	texturemap = NULL;
	normals = NULL;
	
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;

	 wireMap = false;
}


Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
	delete [] texturemap;
	delete [] normals;
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
//float  Terrain::getHeight(float x, float y){
//
//	//for the sample we will calculate height based on distance form origin
//	float dist=sqrt(x*x+y*y);
//
//	//center will be the highest point
//	dist=30-dist;
//	//put a nice curve in it
//	dist*=dist;
//	dist*=dist;
//	//whoah, way to high, make it smaller
//	dist/=50000;
//
//	return dist;
//}
float  Terrain::getHeight2(float y)
{
	return y / 7;
}

void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices=new vector[numVerts];
	delete [] colors;
	colors=new vector[numVerts];
	delete [] texturemap;
	texturemap =new vector[numVerts];
	delete [] normals;
	normals =new vector[numVerts];

	if (!m_heightmap.loadFromFile("map2.png"))
	{
		cout << "height did not load " << endl;
	}

	//interpolate along the edges to generate interior points
	for(int i=0;i<gridWidth-1;i++){ //iterate left to right
		for(int j=0;j<gridDepth-1;j++){//iterate front to back
			int sqNum=(j+i*gridDepth);
			int vertexNum=sqNum*3*2; //6 vertices per square (2 tris)
			float front= j+1;
			float back = j;
			float left= i;
			float right= i + 1;
			int mapWidth = m_heightmap.getSize().x;
			int mapHeight = m_heightmap.getSize().y;
			sf::Color frontleftcol  = m_heightmap.getPixel((i ) * mapWidth / gridWidth ,(j +1 ) * mapHeight / gridDepth);
			sf::Color frontrightcol = m_heightmap.getPixel((i +1 ) * mapWidth / gridWidth ,(j +1) * mapHeight / gridDepth);
			sf::Color backleftcol= m_heightmap.getPixel((i ) * mapWidth / gridWidth ,(j ) * mapHeight / gridDepth);
			sf::Color  backrightcol= m_heightmap.getPixel((i + 1 ) * mapWidth / gridWidth ,(j) * mapHeight / gridDepth);
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
			int tempVertNum = vertexNum;
			
		
			setPoint(colors[vertexNum],FRONTLEFT,0,0);
			setPoint(texturemap[vertexNum],0, 1,0);
			setPoint(vertices[vertexNum++],left,getHeight2(FRONTLEFT),front);

			
			setPoint(colors[vertexNum],FRONTRIGHT,0,0);
			setPoint(texturemap[vertexNum],1, 1,0);
			setPoint(vertices[vertexNum++],right,getHeight2(FRONTRIGHT),front);

			setPoint(colors[vertexNum],BACKRIGHT,0,0);
			setPoint(texturemap[vertexNum],1, 0,0);
			setPoint(vertices[vertexNum++],right,getHeight2(BACKRIGHT),back);

			
			for(int i = tempVertNum; i < vertexNum; i++)
            {
				setNormal(normals[i],left,getHeight2(FRONTLEFT),front,right,getHeight2(FRONTRIGHT),front,right,getHeight2(BACKRIGHT),back);
            }
			tempVertNum = vertexNum;
			//tempVertNum = vertexNum;
			//declare a degenerate triangle
			//TODO: fix this to draw the correct triangle
			setPoint(colors[vertexNum],0,0,FRONTLEFT);
			setPoint(texturemap[vertexNum],0, 1,0);
			setPoint(vertices[vertexNum++],left,getHeight2(FRONTLEFT),front);

			setPoint(colors[vertexNum],0,0,BACKRIGHT);
			setPoint(texturemap[vertexNum],1, 0,0);
			setPoint(vertices[vertexNum++],right,getHeight2(BACKRIGHT),back);

			setPoint(colors[vertexNum],0,0,BACKLEFT);
			setPoint(texturemap[vertexNum],0, 0,0);
			setPoint(vertices[vertexNum++],left,getHeight2(BACKLEFT),back);
			
			//top left triangle
			for(int i = tempVertNum; i < vertexNum; i++)
			{
			setNormal(normals[i],left,getHeight2(FRONTLEFT),front,right,getHeight2(BACKRIGHT),back,left,getHeight2(BACKLEFT),back);
			}
			

		}
	}


		//create light source
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shininess[] = { 5.0 };
    GLfloat light_position[] = {0, 50, 0, 1.0 };
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
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

}

bool Terrain::getWireMeash(){return wireMap;}
void Terrain::setWireMesh(bool val){wireMap = val;}
void Terrain::setNormal(vector nor, float p1x, float p1y, float p1z,float p2x, float p2y, float p2z,float p3x, float p3y, float p3z )
{      
        sf::Vector3f u;
        sf::Vector3f v;
        sf::Vector3f n;
        sf::Vector3f p1( p1x,p1y,p1z); 
        sf::Vector3f p2( p2x,p2y,p2z);
        sf::Vector3f p3( p3x,p3y,p3z);
       
        u = p2 - p1;
        v = p3 - p1;
 
        n.x = (u.y * v.z) - (u.z * v.y);
        n.y = (u.z * v.x) - (u.x * v.z);
        n.z = (u.x * v.y) - (u.y * v.x);
       
        float length =  sqrt((n.x * n.x) + (n.y * n.y) + (n.z * n.z));
       
        nor[0] = n.x / length;
        nor[1] = n.y / length;
        nor[2] = n.z / length;
}

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
	float max = 0;
	for(int i =0;i<numVerts;i++)
	{
		//pass normals to open gl
        glNormal3fv(normals[i]);
		glTexCoord2d(vertices[i][0] / gridWidth  ,vertices[i][2] / gridDepth);
		//glColor3fv(colors[i]);
		glVertex3fv(vertices[i]);
		//if (vertices[i][1] > max)
		//{
		//	max = vertices[i][1];
		//}
		//cout << max<< endl;
 
	}
	glEnd();

	

	
	
}