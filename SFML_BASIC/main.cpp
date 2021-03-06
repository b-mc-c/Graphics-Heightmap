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
#include <iostream> 
  
 #include "Light.h"
#include "Terrain.h"

#include "Camera.h"

int main() 
{ 
    // Create the main window 

	int width=600,height=600;

	sf::RenderWindow App(sf::VideoMode(width, height, 32), "SFML OpenGL"); 
    // Create a clock for measuring time elapsed     
    sf::Clock clock; 
	float deltaTime = 0.0f;
	sf::Time timeSinceLastUpdate;

	aiVector3D position(0,10,-30);
	Camera camera;
	Light light;
    camera.Init(position); //create a camera
      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.5f, 0.5f, 0.5f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
	//light.lightInit();
    //// Setup a perspective projection & Camera position 
	glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 

	sf::Texture grass , snow , water;
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


	//load & bind the shader
	sf::Shader shader;
	//all the lighting & texture blending code should  be put in 'fragment.glsl'
	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
        exit(1);
    }	
	shader.setParameter("waterTexture",water);
	shader.setParameter("grassTexture",grass);
	shader.setParameter("snowTexture",snow);

	sf::Shader::bind(&shader);

	//Create our Terrain
	Terrain terrain;
	terrain.Init();

    // Start game loop 
    while (App.isOpen()) 
    { 

		timeSinceLastUpdate = clock.getElapsedTime();
		deltaTime = timeSinceLastUpdate.asSeconds();
		clock.restart();
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::I)) 
                 terrain.setWireMesh(!terrain.getWireMeash());
			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::N)) 
				terrain.changeDNormal();
			//update the camera
			camera.Update(Event, deltaTime);
 
            
    
        } 
		
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   
        // Apply some transformations 
        //initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity();

		camera.Projection();

		//get the viewing transform from the camera
		camera.ViewingTransform();


		//make the world spin
		//TODO:probably should remove this in final
		//static float ang=0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		

	
		//draw the world
		light.lightInit();
		terrain.Draw();
		
		   
        // Finally, display rendered frame on screen 
        App.display(); 

    } 
   
    return EXIT_SUCCESS; 
}
