#pragma once
#include "utility.h"
#include <iostream>
#include <stdio.h>

class Fence {
private:
	GLuint texture;
	float half_length = 0.5f;
	float height = 3.0f;
	float half_width = 0.1f;
	void draw_single_fence1(float x, float y, float z);
	void draw_single_fence2(float x, float y, float z);

public:
	Fence();
	~Fence();
	//Fence(Vector3d _pos, Vector3d size, GLuint _tex);
	//void set_position(Vector3d pos);
	//void set_texture(GLuint tex);
	void init();
	void draw();
};
