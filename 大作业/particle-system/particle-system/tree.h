#pragma once
#ifndef _TREE_H_
#define _TREE_H_

#include "utility.h"
#include <iostream>
#include <stdio.h>

class Tree {
private:
	GLuint texture[2];
	void draw_single_tree(Vector3d _pos, Vector3d _size);
	Vector3d set_size(double _height, double _width, double _levels);

public:
	Tree();
	~Tree();
	void init();
	void draw();
};

struct Color {
	float red, green, blue;
};

class AFlower {
private:
	Color petal_color;
	Color stalk_color;
	int petals;
	float petal_radius;
	float growth;
	int stalk_size;
	float stalk_angle;
	float x_axis_stalk;
	float y_axis_stalk;
	float z_axis_stalk;
	
public:
	AFlower();
	~AFlower();
	void draw(float x, float y, float z);

};

class Flower {
private:
	AFlower *data[100];
	float x[100];
	float y[100];
	float z[100];
public:
	Flower();
	~Flower();
	void init();
	void draw();
};


#endif // !_TREE_H_
