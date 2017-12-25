#include "wall.h"

Wall::Wall(){}

Wall::~Wall(){}

void Wall::init() {
	char *filename[] = { "data\\wall.BMP" };
	bool rc = BuildTexture(filename[0], texture, 0, 0, 0);
}

void Wall::draw() {

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(-30, 0, -30);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 30.0f, -30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 30.0f, 30.0f);
	glEnd();
	glNormal3f(0, 0, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 30.0f, -30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 30.0f, -30.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30, 0, 30);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 30.0f, -30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 30.0f, 30.0f);
	glEnd();
	glNormal3f(0, 0, -1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 30.0f, 30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 30.0f, 30.0f);
	glEnd();
	glNormal3f(-1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, -1.0f, 10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 30.0f, 30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 30.0f, 10.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, -30);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(-1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 30.0f, 30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 30.0f, -30.0f);
	glEnd();
	glNormal3f(0, 0, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -1.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 30.0f, -30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 30.0f, -30.0f);
	glEnd();
	glNormal3f(0, 0, -1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -1.0f, 30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 30.0f, 30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 30.0f, 30.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 0, 20);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(-1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0f, -1.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -1.0f, 20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 30.0f, 20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 30.0f, -20.0f);
	glEnd();
	glNormal3f(0, 0, -1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, -1.0f, 20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -1.0f, 20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 30.0f, 20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 30.0f, 20.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}