#pragma once
#include "utility.h"
#include <iostream>

class Wall {
private:
	GLuint texture;

public:
	Wall();
	~Wall();
	void init();
	void draw();
};