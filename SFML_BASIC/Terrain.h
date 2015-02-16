#pragma once
class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector[3];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector *vertices;
	vector *colors;
	vector *texturemap;
	vector *normals;

	//std::vector<float> textureX;
	//std::vector<float> textureY;

	float getHeight(float x, float y);
	float getHeight2(float y);
	void setPoint(vector, float, float,float);
	
	bool wireMap;
	bool dNormals;

	sf::Image m_heightmap;
	/*sf::Texture grass , snow , water;*/
	
	
public:
	Terrain(void);
	~Terrain(void);
	bool getWireMeash();
	void setWireMesh(bool val);
	void Init();
	void setNormal(vector nor, float p1x, float p1y, float p1z,float p2x, float p2y, float p2z,float p3x, float p3y, float p3z );
	void Draw();
	void DrawNormals();
	void changeDNormal();
};

