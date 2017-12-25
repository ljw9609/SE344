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
#include <olectl.h>              // OLE���ƿ�ͷ�ļ�  
#include <math.h>              // ��ѧ����ͷ�ļ�  
#endif

#define BITMAP_ID 0x4D42


//int LoadGLTextures(char *Filename, GLuint *ttexture, int i);
GLuint LoadTextures3(const char* filename);
//GLuint LoadTextures4(const char* filename);
//GLuint LoadTextures5(const char* filename);

//void loadTex(int i, char *filename, GLuint* texture);//һ������  
//void loadTex(int i, char *filename, GLuint* texture, unsigned char* backgroundColor);//͸������  

bool BuildTexture(char *szPathName, GLuint &texid, BYTE r, BYTE g, BYTE b);
#endif /* utility_h */
#pragma once
