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

//单个粒子类
class Particle {
private:
	Vector3d velo;		//速度
	Vector3d acc;		//加速度
	Vector3d color;		//颜色
	Vector3d position;	//位置
	Vector3d size;		//大小
	Vector3d angle;		//角度

	float lifetime;		//生命值
	float dec;			//消失速度
	GLuint texture;		//纹理
	
	bool has_tex;		//是否有纹理
	bool is_forever;    //是否永存

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

//粒子系统类
class ParticleSystem {
private:
	float x1, y1, z1, x2, y2, z2;
	Particle **data;				//单个粒子
	int particle_num;				//系统内粒子个数

	bool *dead;						//粒子状态
	Particle* (*f)();				//返回Particle类指针的函数的指针

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