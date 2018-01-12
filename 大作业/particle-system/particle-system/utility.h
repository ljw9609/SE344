#ifndef utility_h
#define utility_h

#define _CRT_SECURE_NO_WARNINGS  
#include "eigen.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <glut/glut.h>
#include "GL/glew.h"
#else
#define FREEGLUT_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLTools.h>
#include <GL/GLAUX.H>
#include <olectl.h>              // OLE控制库头文件  
#include <math.h>              // 数学函数头文件  
#endif

#define BITMAP_ID 0x4D42



GLuint LoadTextures3(const char* filename);

bool BuildTexture(char *szPathName, GLuint &texid, BYTE r, BYTE g, BYTE b);
#endif /* utility_h */
#pragma once
