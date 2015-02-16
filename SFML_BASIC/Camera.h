#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stdafx.h"
#include <iostream>

using namespace std;



class Camera{
	static aiVector3D zero,yaxis,zaxis,xaxis;
public:
	aiVector3D position;
	aiVector3D forward;
	aiVector3D up;
	aiVector3D sideways;

	const int width,height;

	bool projectionType;

	//float angleYaw;
	//float anglePitch;

	float forwardSpeed;
	float rotationSpeed;


	Camera():forwardSpeed(200.0f),rotationSpeed(20.0f), projectionType(false), width(600), height(600){}//,angleYaw(0.0f),anglePitch(0.0f){}

	void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis, aiVector3D& s=xaxis){
		position=p;
		forward = f;
		up= u;
		sideways=s;

	}

	void Update(sf::Event e, float deltaTime){//respond to keyboard events
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::D)){
			MoveLeftRight(+1, deltaTime);
		}

		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::A)){
			MoveLeftRight(-1, deltaTime);
		}

		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::W)){
			MoveForwardBack(1, deltaTime);
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::S)){
			MoveForwardBack(-1, deltaTime);
		}

		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Q)){
			MoveUpDown(1, deltaTime);
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::E)){
			MoveUpDown(-1, deltaTime);
		}

		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Right)){
			TurnRightLeft(1, deltaTime);
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Left)){
			TurnRightLeft(-1, deltaTime);
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Up)){
			TurnUpDown(1, deltaTime);
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Down)){
			TurnUpDown(-1, deltaTime);
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::O)){
			projectionType = true;
		}
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::P)){
			projectionType = false;
		}


	}
	void MoveLeftRight(int dir, float deltaTime){ //Dir=+1=>Right, dir=-1=> Left
		position +=(sideways*deltaTime*(forwardSpeed*dir));
	}

	void MoveUpDown(int dir, float deltaTime){ //Dir=+1=>Right, dir=-1=> Left
		position-=(up*deltaTime*(forwardSpeed*dir));
	}

	void MoveForwardBack(int dir, float deltaTime){ //Dir=+1=>Forward, dir=-1=> Back
		position+=(forward*deltaTime*(forwardSpeed*dir));
	}

	void TurnRightLeft(int dir, float deltaTime){ //Dir=+1=>Right, dir=-1=> Left
		//TODO fix this
		//forward.x -= rotationSpeed*deltaTime*dir;
		//float theta = rotationSpeed;
		//angleYaw += dir * rotationSpeed*deltaTime;
		//forward = aiVector3D(cos(angleYaw), 0, sin(angleYaw));

		float angle = rotationSpeed*-dir*deltaTime;
		aiMatrix3x3 m; //create 3x3 matrix
		m.Rotation(angle,up,m);
		forward*=m;
		sideways = forward^up;

	}

	void TurnUpDown(int dir, float deltaTime){ //Dir=+1=>Up, dir=-1=> Down
		//TODO this is not the right way
		//anglePitch -= dir * rotationSpeed*deltaTime;
		//forward = aiVector3D(0,cos(anglePitch),sin(anglePitch));
		float angle = rotationSpeed*dir*deltaTime;
		aiMatrix3x3 m; //create 3x3 matrix
		sideways = forward^up;
		sideways.Normalize();
		m.Rotation(angle,sideways,m);
		forward*=m;
		up*=m;
		sideways = forward^up;
		//up *=m;

	}

	void ViewingTransform(){
		gluLookAt(	position.x,position.y,position.z,// camera position
			position.x + forward.x, position.y + forward.y,position.z + forward.z, //look at this point
			0,1,0); //camera up
	}

	void Projection(){
		if(projectionType){
			glOrtho(-100, 100, -100,100,1,100000);

		}
		else {
			gluPerspective(90.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar
		}
	}


};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::xaxis(1.0f,0.0f,0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);
