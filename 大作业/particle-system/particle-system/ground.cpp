#include "ground.h"
#include "utility.h"

Ground::Ground() {}

Ground::~Ground() {}

void Ground::init() {
	char *fileName[] = { "data\\SAND1.BMP","data\\SAND4.BMP","data\\SAND0.BMP","data\\SAND2.BMP"};
	for (int i = 0; i < 4; i++) {
		texture[i] = LoadTextures3(fileName[i]);
	}
}

void Ground::draw() {
	//glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//ground1
	glPushMatrix();
	glTranslatef(-100.0f, 0.0f, -100.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4ub(255, 255, 255, 255);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, -1.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, -1.0f, -100.0f);
	glEnd();
	glPopMatrix();

	//ground2
	glPushMatrix();
	glTranslatef(100.0f, 0.0f, -100.0f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4ub(255, 255, 255, 255);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, -1.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, -1.0f, -100.0f);
	glEnd();
	glPopMatrix();

	//ground3
	glPushMatrix();
	glTranslatef(-100.0f, 0.0f, 100.0f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4ub(255, 255, 255, 255);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, -1.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, -1.0f, -100.0f);
	glEnd();
	glPopMatrix();

	//ground4
	glPushMatrix();
	glTranslatef(100.0f, 0.0f, 100.0f);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glColor4ub(255, 255, 255, 255);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -1.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, -1.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, -1.0f, -100.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glEnable(GL_LIGHTING);
}
