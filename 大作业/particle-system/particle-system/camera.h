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


class Camera {
public:
	Camera();
	~Camera();
	void initCamera(const Vector3d& pos, const Vector3d& target, const Vector3d& up);	//初始化相机
	void setModelViewMatrix();															//转化为视图矩阵
	void setShape(float viewAngle, float aspect, float Near, float Far);				//当窗口大小变化时设置相机视角
	void slide(float du, float dv, float dn);											//相机绕三个轴平移的计算函数
	float getDist();																	//自己到原点的距离
	void rotateX(float angle);															//沿屏幕水平旋转
	void rotateY(float angle);															//沿屏幕竖直旋转
	void rotateRoll(float angle);														//相机自身旋转
	void horizontalMove(float forward, float left);										//相机水平移动
	void verticalMove(float up);														//相机竖直移动
	void resetCamera();																	//重设相机参数
	Vector3d myPosition();
private:
	Vector3d m_pos;			//相机的位置
	Vector3d m_target;		//目标的位置
	Vector3d m_up;			//相机的头顶方向
	Vector3d u, v, n;		//表示相机坐标系，u代表相机右方向，v代表相机上方向，n代表相机注视方向
	vector<Vector3d> backup;//用来备份初始化条件

	void roll(float angle);																//绕n轴旋转
	void yaw(float angle);																//绕v轴旋转
	void pitch(float angle);															//绕u轴旋转
};

#endif /* camera_h */
#pragma once
