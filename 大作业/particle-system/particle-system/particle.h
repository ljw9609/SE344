#pragma once
#include <iostream>
#include <stdio.h>
#include "eigen.h"
#include <math.h>
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

//����������
class Particle {
private:
	Vector3d velo;		//�ٶ�
	Vector3d acc;		//���ٶ�
	Vector3d color;		//��ɫ
	Vector3d position;	//λ��
	Vector3d size;		//��С
	Vector3d angle;		//�Ƕ�

	float lifetime;		//����ֵ
	float dec;			//��ʧ�ٶ�
	GLuint texture;		//����
	
	bool has_tex;		//�Ƿ�������
	bool is_forever;    //�Ƿ�����

	unsigned int motion_mode;
	float ftheta;

	void draw_with_tex(GLuint texture);
	void draw_with_color();
	
public:
	Particle();
	~Particle();
	Particle(Vector3d init_velo, Vector3d init_acc, Vector3d init_size, Vector3d init_angle, bool init_has_tex,
		Vector3d init_color, GLuint init_tex, float init_life, float init_dec, bool init_is_forever, unsigned int init_mode);
	void init_particle(Vector3d init_velo, Vector3d init_acc, Vector3d init_pos, Vector3d init_size, Vector3d init_angle, bool init_has_tex,
					   Vector3d init_color,GLuint init_tex, float init_life, float init_dec, bool init_is_forever);
	void draw();
	void update();

	void setPosition(Vector3d pos);
	Vector3d getPosition();
	float getLifetime();
	Vector3d getVelocity();
	bool isForever();
};

//����ϵͳ��
class ParticleSystem {
private:
	float x1, y1, z1, x2, y2, z2;
	Particle **data;				//��������
	int particle_num;				//ϵͳ�����Ӹ���

	bool *dead;						//����״̬
	Particle* (*f)();				//����Particle��ָ��ĺ�����ָ��

	bool(*out_of_bound)(Particle*);	

public:
	ParticleSystem();
	~ParticleSystem();
	void init_system(Particle* (init)(), bool(*judge)(Particle*));
	void update();
	void reset();
	void set_emit_position(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2);
	ParticleSystem(int init_num, float init_x1, float init_y1, float init_z1, 
				   float init_x2, float init_y2, float init_z2);
};