#pragma once
#include "utility.h"
#include <iostream>
#include <string>
#include <sstream>

/*
class RawLoader {
public:
	RawLoader();
	~RawLoader();
	GLuint load_raw(char *filename, int wrap, int width, int height);
};
*/
struct Vert {
	float x;
	float y;
	float z;
};

struct TexCoord {
	float u;
	float v;
};

struct Normal {
	float x;
	float y;
	float z;
};


class RawLoader {
private:
	int width;
	int height;
public:
	RawLoader(void);
	~RawLoader(void);

	GLuint LoadTextureRAW(char * filename, int wrap, int _width, int _height);
};

class Terrain {
private:
	unsigned int hmHeight;
	unsigned int hmWidth;
	unsigned int terrainList;
	unsigned int tID[2];
	unsigned int vhVBOVertices;
	unsigned int vhVBOTexCoords;
	unsigned int vhVBONormals;

	int vertexCount;
	bool loaded;

	GLuint complexity;
	GLubyte hHeightField[1024][1024];
	GLubyte texture[1024][1024];

	Vert *vertices;
	TexCoord *texCoords;
	Normal *normals;
public:
	Terrain(void);

	void Display();
	void Init();

	void DrawDots(void);
	void IncreaseComplexity(void);
	void DecreaseComplexity(void);
	float GetHeightAt(unsigned int x, unsigned int z);
	GLuint GetComplexity(void);
};