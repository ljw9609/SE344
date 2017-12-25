#ifndef camera_h
#define camera_h
#include "eigen.h"
#include <iostream>
#include <stdio.h>
#include <vector>
using std::vector;
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


class GLCamera {
public:
	GLCamera();
	~GLCamera();
	void initCamera(const Vector3d& pos, const Vector3d& target, const Vector3d& up);
	void setModelViewMatrix();
	void setShape(float viewAngle, float aspect, float Near, float Far);
	void slide(float du, float dv, float dn);
	void roll(float angle);
	void yaw(float angle);
	void pitch(float angle);
	float getDist();
	void rotateX(float angle);
	void rotateY(float angle);
	void horizontalMove(float forward, float left);
	void verticalMove(float up);
	void resetCamera();
	Vector3d myPosition();
private:
	Vector3d m_pos;
	Vector3d m_target;
	Vector3d m_up;
	Vector3d u, v, n;
	vector<Vector3d> backup;
};

#endif /* camera_h */
#pragma once
