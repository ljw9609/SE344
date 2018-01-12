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
	void initCamera(const Vector3d& pos, const Vector3d& target, const Vector3d& up);	//��ʼ�����
	void setModelViewMatrix();															//ת��Ϊ��ͼ����
	void setShape(float viewAngle, float aspect, float Near, float Far);				//�����ڴ�С�仯ʱ��������ӽ�
	void slide(float du, float dv, float dn);											//�����������ƽ�Ƶļ��㺯��
	float getDist();																	//�Լ���ԭ��ľ���
	void rotateX(float angle);															//����Ļˮƽ��ת
	void rotateY(float angle);															//����Ļ��ֱ��ת
	void rotateRoll(float angle);														//���������ת
	void horizontalMove(float forward, float left);										//���ˮƽ�ƶ�
	void verticalMove(float up);														//�����ֱ�ƶ�
	void resetCamera();																	//�����������
	Vector3d myPosition();
private:
	Vector3d m_pos;			//�����λ��
	Vector3d m_target;		//Ŀ���λ��
	Vector3d m_up;			//�����ͷ������
	Vector3d u, v, n;		//��ʾ�������ϵ��u��������ҷ���v��������Ϸ���n�������ע�ӷ���
	vector<Vector3d> backup;//�������ݳ�ʼ������

	void roll(float angle);																//��n����ת
	void yaw(float angle);																//��v����ת
	void pitch(float angle);															//��u����ת
};

#endif /* camera_h */
#pragma once
