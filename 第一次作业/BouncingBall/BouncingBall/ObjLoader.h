#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glew.h"
#include "GL/glut.h"
#include <vector>
#include <string>
using namespace std;

class ObjLoader {

public:
	ObjLoader(string filename);
	void Draw();

private:
	vector<vector<GLfloat>>vSets;
	vector<vector<GLint>>fSets;

};

#pragma once
