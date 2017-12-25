#pragma once
#include "utility.h"
#include "fluid.h"
#define PI 3.1415926535

void DrawCircleArea(float cx, float cy, float cz, float r, int num_segments){
	GLfloat vertex[4];

	const GLfloat delta_angle = 2.0*PI / num_segments;
	glBegin(GL_TRIANGLE_FAN);

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);

	for (int i = 0; i < num_segments; i++){
		vertex[0] = std::cos(delta_angle*i) * r + cx;
		vertex[1] = std::sin(delta_angle*i) * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	vertex[0] = 1.0 * r + cx;
	vertex[1] = 0.0 * r + cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();
}


void draw_solid_cylinder(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks){
	gluCylinder(quad, base, top, height, slices, stacks);
	//top   
	DrawCircleArea(0.0, 0.0, height, top, slices);
	//base   
	DrawCircleArea(0.0, 0.0, 0.0, base, slices);
}

void draw_campfire(GLuint tex) {
	GLuint texture = tex;

	//glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(-16.0f, 0.0f, -16.0f);

	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);	
	gluQuadricTexture(quadric, GL_TRUE);	

	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	//glTranslatef(0.8, 0, 0);
	glRotatef(-90, 0, 1, 0);
	draw_solid_cylinder(quadric, 0.35f, 0.35f, 4, 32, 32);
	
	for (int i = 0; i < 11; i++) {
		glRotatef(30, 0, 1, 0);
		draw_solid_cylinder(quadric, 0.35f, 0.35f, 4, 32, 32);
	}
	
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, 1, 1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
}

void draw_house(float x, float y, float z, GLuint *tex) {
	/*
	char *filename[] = { "data\\wood3.BMP","data\\darkwood.BMP","data\\roof.BMP" };
	bool rc;
	GLuint texture[3];
	for (int i = 0; i < 3; i++) {
		rc = BuildTexture(filename[i], texture[i], 0, 0, 0);
	}
	*/
	GLuint *texture = tex;

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(x, y, z);

	float half_length = 10;
	float half_width = 7.5;
	float height = 10;
	float roof_height = 5;
	float door_width = 2;
	float door_height = 7;

	//left
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glNormal3f(-1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, 0, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_length, 0, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_length, height, half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height, -half_width);
	glEnd();

	//right
	glNormal3f(1.0f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_length, 0, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, 0, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height, half_width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_length, height, -half_width);
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

	//roof1
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	float norm_y, norm_z;
	norm_z = (half_width * pow(roof_height, 2)) / (pow(half_width, 2)* pow(roof_height, 2));
	norm_y = (roof_height * pow(half_width, 2)) / (pow(half_width, 2)* pow(roof_height, 2));
	glNormal3f(0, norm_y, norm_z);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, height, half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, height, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height + roof_height, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height + roof_height, 0);
	glEnd();

	//roof2
	glBegin(GL_QUADS);
	glNormal3f(0, norm_y, -norm_z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, height, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, height, -half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height + roof_height, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_length, height + roof_height, 0);
	glEnd();

	//roof3
	glBegin(GL_TRIANGLES);
	glNormal3f(1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_length, height, half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_length, height, -half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_length, height + roof_height, 0);
	glEnd();

	//roof4
	glBegin(GL_TRIANGLES);
	glNormal3f(-1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_length, height, -half_width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_length, height, half_width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_length, height + roof_height, 0);
	glEnd();

	//door
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(door_width, 0, -half_width-0.01);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-door_width, 0, -half_width-0.01);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-door_width, door_height, -half_width-0.01);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(door_width, door_height, -half_width-0.01);
	glEnd();

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, 1, 1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

void draw_fluid(Fluid *fluid, float x, float y, float z) {
	static int count = 0;
	count++;
	if (count > 10) {
		count = 0;
		fluid->Evaluate();
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(x, y, z);
	glNormal3f(0, 1.0f, 0);
	glRotatef(90, 1, 0, 0);
	fluid->draw();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void draw_single_fountain(float _outer_length, float _inner_length, float _height, GLuint _tex) {
	float half_outer_length = _outer_length;
	float half_inner_length = _inner_length;
	float height = _height;
	GLuint texture = _tex;

	glBindTexture(GL_TEXTURE_2D, texture);
	//outer
	//right
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_outer_length, 0, half_outer_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_outer_length, 0, -half_outer_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_outer_length, height, -half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_outer_length, height, half_outer_length);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_outer_length, 0, -half_outer_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_outer_length, 0, half_outer_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_outer_length, height, half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_outer_length, height, -half_outer_length);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_outer_length, 0, -half_outer_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_outer_length, 0, -half_outer_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_outer_length, height, -half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_outer_length, height, -half_outer_length);
	glEnd();

	//back
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_outer_length, 0, half_outer_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_outer_length, 0, half_outer_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_outer_length, height, half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_outer_length, height, half_outer_length);
	glEnd();

	//inner
	//right
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_inner_length, 0, half_inner_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_inner_length, 0, -half_inner_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_inner_length, height, -half_inner_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_inner_length, height, half_inner_length);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_inner_length, 0, -half_inner_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_inner_length, 0, half_inner_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_inner_length, height, half_inner_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_inner_length, height, -half_inner_length);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_inner_length, 0, -half_inner_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_inner_length, 0, -half_inner_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_inner_length, height, -half_inner_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_inner_length, height, -half_inner_length);
	glEnd();

	//back
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_inner_length, 0, half_inner_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_inner_length, 0, half_inner_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_inner_length, height, half_inner_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_inner_length, height, half_inner_length);
	glEnd();

	//top
	//right
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0f, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_inner_length, height, half_outer_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_outer_length, height, half_outer_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_outer_length, height, -half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_inner_length, height, -half_outer_length);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0f, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_inner_length, height, -half_outer_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_outer_length, height, -half_outer_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_outer_length, height, half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_inner_length, height, half_outer_length);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0f, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-half_inner_length, height, -half_inner_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(half_inner_length, height, -half_inner_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(half_inner_length, height, -half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-half_inner_length, height, -half_outer_length);
	glEnd();

	//back
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0f, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(half_inner_length, height, half_inner_length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-half_inner_length, height, half_inner_length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-half_inner_length, height, half_outer_length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(half_inner_length, height, half_outer_length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_fountain(float x, float y, float z, GLuint tex, Fluid *fountain1, Fluid *fountain2) {
	GLuint texture = tex;

	float half_outer_length = 7;
	float half_inner_length = 6;
	float height = 3;

	float half_outer_length2 = 3;
	float half_inner_length2 = 2;
	float height2 = 6;

	Fluid *f1 = fountain1;
	Fluid *f2 = fountain2;

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(x, y, z);
	draw_single_fountain(half_outer_length, half_inner_length, height, texture);
	glPopMatrix();

	draw_fluid(f1, x - half_inner_length, y + height / 2, z - half_inner_length);
	
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(x, y, z);
	draw_single_fountain(half_outer_length2, half_inner_length2, height2, texture);
	glPopMatrix();

	draw_fluid(f2, x - half_inner_length2, y + height2 * 0.8, z - half_inner_length2);

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, 1, 1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void draw_fake_lake(float x, float y, float z, GLuint tex, Fluid *fluid) {
	GLuint texture = tex;

	float half_outer_length = 12;
	float half_inner_length = 11;
	float height = 2;

	Fluid *f = fluid;

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(x, y, z);
	draw_single_fountain(half_outer_length, half_inner_length, height, texture);
	glPopMatrix();

	draw_fluid(f, x - half_inner_length, y + height * 0.8, z - half_inner_length);

}