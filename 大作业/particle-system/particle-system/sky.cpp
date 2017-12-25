#include "sky.h"
#include "utility.h"

Sky::Sky(){}

Sky::~Sky(){}

void Sky::init() {
	char *fileName[] = { "data\\Back.BMP","data\\Front.BMP","data\\Bottom.BMP","data\\Top.BMP","data\\Left.BMP","data\\Right.BMP" };
	//char *fileName[] = { "data\\skybox\\oasisnight_back.BMP","data\\skybox\\oasisnight_front.BMP","data\\skybox\\oasisnight_top.BMP",
		//"data\\skybox\\oasisnight_top.BMP","data\\skybox\\oasisnight_left.BMP","data\\skybox\\oasisnight_right.BMP" };
	for (int i = 0; i < 6; i++) {
		texture[i] = LoadTextures3(fileName[i]);
	}
}

void Sky::render(float x, float y, float z) {
	x = 0;
	y = -1;
	z = 0;
	float width = 200.0;
	float height = 100.0;
	float length = 200.0;

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(x, y, z);
	
	//back
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z - length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z - length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width, y + height, z - length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width, y, z - length);
	glEnd();

	//front
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x - width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x - width, y, z + length);
	glEnd();

	//bottom
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z - length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x - width, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x - width, y, z - length);
	glEnd();

	//up
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z - length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x - width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x - width, y + height, z - length);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x - width, y + height, z - length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x - width, y, z - length);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z - length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z - length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}