#include "fence.h"

Fence::Fence(){}

Fence::~Fence(){}

/*
Fence::Fence(Vector3d _pos, Vector3d _size, GLuint _tex) {
	position = _pos;
	size = _size;
	texture = _tex;
}

void Fence::set_position(Vector3d pos){
	position = pos;
}

void Fence::set_texture(GLuint tex){
	texture = tex;
}
*/

void Fence::init() {
	char *filename[] = { "data\\wood.BMP" };
	bool rc = BuildTexture(filename[0], texture, 0, 0, 0);

	half_length = 0.5f;
	height = 3.0f;
	half_width = 0.1f;
}

void Fence::draw_single_fence1(float x, float y, float z) {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(x, y, z);
	//right
	glNormal3f(1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_width, 0, -half_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_width, 0, half_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_width, height, half_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_width, height, -half_length);
	glEnd();
	//left
	glNormal3f(-1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_width, 0, -half_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_width, 0, half_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_width, height, half_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_width, height, -half_length);
	glEnd();
	//front
	glNormal3f(0, 0, -1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_width, 0, -half_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_width, 0, -half_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_width, height, -half_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_width, height, -half_length);
	glEnd();
	//back
	glNormal3f(0, 0, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_width, 0, half_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_width, 0, half_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_width, height, half_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_width, height, half_length);
	glEnd();
	//top
	glNormal3f(0, 1.0f, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_width, height, -half_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_width, height, -half_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_width, height, half_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_width, height, half_length);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Fence::draw_single_fence2(float x, float y, float z) {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(x, y, z);
	//right
	glNormal3f(1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_length, 0, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, 0, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height, half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_length, height, -half_width);
	glEnd();
	//left
	glNormal3f(-1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, 0, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_length, 0, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_length, height, half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height, -half_width);
	glEnd();
	//front
	glNormal3f(0, 0, -1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, 0, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, 0, -half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height, -half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height, -half_width);
	glEnd();
	//back
	glNormal3f(0, 0, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, 0, half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, 0, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height, half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height, half_width);
	glEnd();
	//top
	glNormal3f(0, 1.0f, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, height, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, height, -half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height, half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height, half_width);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Fence::draw() {
	
	float x1 = -30.0f;
	float y1 = -1.0f;
	float z1 = -4.5f;
	for (z1; z1 > -60.0f; z1 -= 1.5f) {
		draw_single_fence1(x1, y1, z1);
	}

	float x2 = 20.0f;
	float y2 = -1.0f;
	float z2 = 8.0f;
	for (z2; z2 < 40.0f; z2 += 1.5f) {
		draw_single_fence1(x2, y2, z2);
	}

	float x3 = -0.5f;
	float y3 = -1.0f;
	float z3 = -4.5f;
	for (x3; x3 > -22.0f; x3 -= 1.5f) {
		draw_single_fence2(x3, y3, z3);
		draw_single_fence2(x3, y3, -z3);
	}

	float x4 = -59.5f;
	float y4 = -1.0f;
	float z4 = -4.5f;
	for (x4; x4 < -38.0f; x4 += 1.5f) {
		draw_single_fence2(x4, y4, z4);
		draw_single_fence2(x4, y4, -z4);
	}

	float x5 = 1.0f;
	float y5 = -1.0f;
	float z5 = 4.5f;
	for (x5; x5 < 22.0f; x5 += 1.5f) {
		draw_single_fence2(x5, y5, z5);
		draw_single_fence2(x5, y5, -z5);
	}

	float x6 = 38.5f;
	float y6 = -1.0f;
	float z6 = -4.5f;
	for (x6; x6 < 60.0f; x6 += 1.5f) {
		draw_single_fence2(x6, y6, z6);
		if (x6 < 39.5f)
			draw_single_fence2(x6, y6, -z6);
	}

	

}