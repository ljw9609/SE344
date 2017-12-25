#ifndef sky_h
#define sky_h

#include "utility.h"
#include <iostream>
#include <stdio.h>

#ifdef __APPLE__
#include <glut/glut.h>
#include "GL/glew.h"
#else
#define FREEGLUT_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLTools.h>
#endif


class Sky {
private:
	GLuint texture[6];

public:
	Sky();
	~Sky();
	void init();
	void render(float x, float y, float z);
};


#endif
#pragma once
