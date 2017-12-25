#ifndef ground_h
#define ground_h
#include "utility.h"
#include <iostream>
#include <stdio.h>

#ifdef __APPLE__
#include <glut/glut.h>
#include "GL/glew.h"
#else
#define FREEGLUT_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLTools.h>
#endif

class Ground {
private:
	GLuint texture[4];

public:
	Ground();
	~Ground();
	void init();
	void draw();
};

#endif /* ground_h */
#pragma once
