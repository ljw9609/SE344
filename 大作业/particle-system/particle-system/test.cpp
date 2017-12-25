#include "camera.h"
#include "ground.h"
#include "sky.h"
#include "particle2.h"
#include "utility.h"
#include "objloader.h"
#include "tree.h"
#include "fence.h"
#include "wall.h"
#include "fluid.h"
#include "drawScene.h"
#include <string>
#include <iostream>
using namespace std;


/* 摄像机类 */
GLCamera camera;

/* 地面类 */
Ground ground;

/* 天空类 */
Sky sky;

Tree tree;
Flower flower;

Fence fence;
Wall wall;

Fluid *fluid;
Fluid *fountain1;
Fluid *fountain2;
GLuint fluid_tex;

ObjLoader box;
ObjLoader mountain;
ObjLoader pikachu;
ObjLoader dinosaur;

GLuint texture[5];
ParticleSystem *flame;
ParticleSystem *snow;
ParticleSystem *lazer;
ParticleSystem *waterfall;
ParticleSystem *sun;
ParticleSystem *sun2;
ParticleSystem *fountain;

/** 用来设置粒子的属性值 */
//float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec;
int r, g, b;
int old_x, old_y;
bool left_button_down = false;
bool right_button_down = false;
int tex_index = 0;

float v_x = 0;
float v_y = 0;
float v_z = -0.5f;
float R = 0.5 * 0.5;


int time;
int frame = 0;
int timebase = 0;
int fps = 0;

GLuint campfire_tex;
GLuint fountain_tex;
GLuint house_tex[5];

Particle* init_flame(){
	Vector3d size(rand() % 30 * 0.02f, rand() % 30 * 0.02f, rand() % 30 * 0.02f);
	Vector3d velo(float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 9000000, 4.9 / 4000000, 1.0f*(rand() % 3 - 1) / 9000000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[tex_index];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 0.2f, false);
	return p;
}

Particle* init_snow() {
	Vector3d size(rand() % 50 * 0.04f, rand() % 50 * 0.04f, rand() % 50 * 0.04f);
	Vector3d velo(float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 400, float(rand() % 10 - 4) / 800);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 90000, -4.9 / 4000, 1.0f*(rand() % 3 - 1) / 90000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[1];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 0.2f, false);
	return p;
}

Particle* init_lazer() {
	Vector3d size(rand() % 50 * 0.01f, rand() % 50 * 0.01f, rand() % 50 * 0.01f);
	Vector3d velo(v_x, v_y, v_z);
	Vector3d acc(0, -4.9 / 400, 0.01);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[0];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 1.6f, false);
	return p;
}

Particle* init_waterfall() {
	Vector3d size(rand() % 50 * 0.01f, rand() % 50 * 0.01f, rand() % 50 * 0.01f);
	Vector3d velo(0.5f, 0, 0);
	Vector3d acc(-0.005f, -0.01, 0);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[3];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(0.0, 0.0, 0.6), tex, (float)(rand() % 50 + 10), 0.6f, false);
	return p;
}

Particle* init_fountain() {
	Vector3d size(rand() % 50 * 0.01f, rand() % 50 * 0.01f, rand() % 50 * 0.01f);
	Vector3d velo(0.00007f * (rand() % 2500 - rand() % 2500), 0.0004f * (rand() % 2000), 0.00007f * (rand() % 2500 - rand() % 2500));
	Vector3d acc(0, -0.04, 0);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[3];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(0.0, 0.0, 0.6), tex, (float)(rand() % 50 + 10), 0.6f, false);
	return p;
}

Particle* init_sun() {
	Vector3d size(rand() % 30 * 0.02f, rand() % 30 * 0.02f, rand() % 30 * 0.02f);
	Vector3d velo(0, 0, 0);
	Vector3d acc(0, 0, 0);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[0];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 0.2f, false);
	return p;
}

Particle* init_sun2() {
	Vector3d size(rand() % 30 * 0.02f, rand() % 30 * 0.02f, rand() % 30 * 0.02f);
	Vector3d velo(float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 9000000, 4.9 / 4000000, 1.0f*(rand() % 3 - 1) / 9000000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[2];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 0.2f, false);
	return p;
}


bool out_of_bound(Particle* p)
{
	Vector3d position = p->getPosition();
	if (position.y() < -1.0f) {
		return true;
	}
	return false;
}

void set_lighting() {
	GLfloat sun_light_position[] = { 0.0f, 30.0f, 0.0f, 1.0f };
	GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
	/* 初始化视角 */
	Vector3d pos(0.0, 2.0, 15.0);
	Vector3d target(0.0, 0.0, 0.0);
	Vector3d up(0.0, 1.0, 0.0);
	camera.initCamera(pos, target, up);
	


	//glEnable(GL_TEXTURE_2D);             /**< 开启纹理映射 */
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);    /**< 设置混合因子获得半透明效果 */
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5f);
	//glEnable(GL_BLEND);                 
									
	/* 初始化地面 */
	ground.init();

	/* 初始化天空 */
	sky.init();

	/* 初始化墙壁 */
	wall.init();

	/* 初始化栅栏 */
	fence.init();

	/* 初始化树木 */
	tree.init();
	flower.init();
	
	bool ret;
	char *filename[] = { "data\\fire.BMP","data\\snow.BMP","data\\gold.BMP","data\\drop.BMP"};
	for(int i = 0;i < 4;i++)
		ret = BuildTexture(filename[i], texture[i], 0, 0, 0);
	
	char *camp_filename[] = {"data\\brack.BMP"};
	campfire_tex = LoadTextures3(camp_filename[0]);

	char *house_filename[] = {"data\\wood3.BMP","data\\wood4.BMP","data\\roof.BMP" };
	for (int i = 0; i < 3; i++) {
		house_tex[i] = LoadTextures3(house_filename[i]);
	}

	char *fountain_filename[] = { "data\\wall2.BMP" };
	fountain_tex = LoadTextures3(fountain_filename[0]);


	char *fluid_filename[] = { "data\\water.BMP" };
	fluid_tex = LoadTextures3(fluid_filename[0]);

	flame = new ParticleSystem(400, -17.0f, 0.5f, -17.0f, -15.0f, 2.0f, -15.0f);
	flame->init_system(init_flame, out_of_bound);

	snow = new ParticleSystem(1000, -59, 25, -59, -1, 25, -4.5);
	snow->init_system(init_snow, out_of_bound);

	lazer = new ParticleSystem(400, 0, 5, 2, 0, 5, 2);
	lazer->init_system(init_lazer, out_of_bound);

	waterfall = new ParticleSystem(1000, -30, 15, 0, -30, 15, 10);
	waterfall->init_system(init_waterfall, out_of_bound);

	fountain = new ParticleSystem(1000, 28, 5, 21, 30, 5, 19);
	fountain->init_system(init_fountain, out_of_bound);

	sun = new ParticleSystem(400, -0.5, 29.5, 0.5, 0.5, 30.5, -0.5);
	sun->init_system(init_sun, out_of_bound);

	sun2 = new ParticleSystem(1000, -1, 29, 1, 1, 31, -1);
	sun2->init_system(init_sun2, out_of_bound);
	//box.load_obj("perfect.OBJ");
	mountain.load_obj("mountain.OBJ");
	pikachu.load_obj("pikachu.OBJ");
	dinosaur.load_obj("dino.OBJ");
	//villa.load_obj("house.OBJ");

	fluid = new Fluid(12, 12, 2, 1, 0.2, 0, fluid_tex);
	fountain1 = new Fluid(7, 7, 2, 1, 0.2, 0, fluid_tex);
	fountain2 = new Fluid(3, 3, 2, 1, 0.2, 0, fluid_tex);
	
}

void calculate_fps() {
	frame++; // increment the frame count since the last check
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 200) { // check every 1/5th of a second
		fps = (int)(frame * 1000 / (time - timebase)); // set the calculated FPS
		timebase = time;
		frame = 0;
	}
}

void draw_string(string s) {
	void * font = GLUT_BITMAP_9_BY_15;
	for (string::iterator i = s.begin(); i != s.end(); ++i) {
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}

void draw_text() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 512, 0.0, 512);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	string strings[5];
	strings[0] = "Computer Graphics Assignment : Particle System";
	strings[1] = "Controls:";
	strings[2] = "Movement: WASD, Pitch and Yaw: Left Mouse, Roll: Right Mouse";
	strings[3] = "Control dragon fire: IJKL, Reset Camera: R, Change particle: 0123";

	stringstream ss;
	ss << "frames per second: " << fps;
	strings[4] = ss.str();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	int height = 80;

	for (int i = 0; i < 5; i++) {
		glRasterPos2i(10, height -= 12);
		draw_string(strings[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display(void) {
	/* 初始化 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);

	glLoadIdentity();
	camera.setModelViewMatrix();
	
	calculate_fps();

	set_lighting();
	

	/* 绘制地面 */
	ground.draw();

	/* 绘制天空 */
	Vector3d pos = camera.myPosition();
	sky.render(pos.x(),pos.y(),pos.z());

	/* 绘制墙壁 */
	wall.draw();

	/* 绘制栅栏 */
	fence.draw();

	/* 绘制树木 */
	tree.draw();

	/* 绘制花朵 */
	flower.draw();

	/* 绘制篝火 */
	draw_campfire(campfire_tex);
	
	/* 绘制房子 */
	draw_house(-37, -1, 34, house_tex);
	
	/* 绘制喷泉 */
	draw_fountain(29, -1, 20, fountain_tex, fountain1, fountain2);

	/*
	glDisable(GL_TEXTURE);
	glPushMatrix();
	glTranslatef(2.0f, 5.0f, 0);
	glColor3f(0, 1.0f, 0);
	glutSolidSphere(2.0, 32, 32);
	glPopMatrix();
	glColor3f(1,1,1);
	*/
	
	
	glPushMatrix();
	glTranslatef(35, 3, -35);
	glScalef(0.06, 0.06, 0.06);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	//glDisable(GL_LIGHTING);
	//mountain.render_obj();
	//glEnable(GL_LIGHTING);
	glPopMatrix();

	draw_fake_lake(30, -1, -35, fountain_tex, fluid);
	

	/*
	glPushMatrix();
	glTranslated(-10, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	box.render_obj();
	glPopMatrix();
	*/
	
	/* pikachu */
	glPushMatrix();
	glTranslatef(-15, -1, 15);
	glScalef(0.01, 0.01, 0.01);
	pikachu.render_obj();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20, -1, 32);
	glRotatef(30, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	pikachu.render_obj();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-12, -1, 24);
	glRotatef(-50, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	pikachu.render_obj();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-30, -1, 8);
	glRotatef(120, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	pikachu.render_obj();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-50, -1, -10);
	glRotatef(60, 0, 1, 0);
	glScalef(0.025, 0.025, 0.025);
	dinosaur.render_obj();
	glPopMatrix();



	flame->update();
	//snow->update();
	//lazer->update();
	//waterfall->update();
	fountain->update();
	//sun2->update();
	//sun->update();


	/* 屏幕打印信息 */
	glDisable(GL_LIGHTING);
	draw_text();
	glEnable(GL_LIGHTING);

	//glFlush(); /**<强制执行所有的OpenGL命令 */
	glutSwapBuffers();
}

void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);    /**< 重新设置视口 */
	camera.setShape(45.0f, (GLfloat)width / (GLfloat)height, 0.1, 1700.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseClick(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			left_button_down = true;
			old_x = x;
			old_y = y;
		}
		else if (state == GLUT_UP) {
			left_button_down = false;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			right_button_down = true;
			old_x = x;
			old_y = y;
		}
		else if (state == GLUT_UP) {
			right_button_down = false;
		}
		break;

	default:
		break;
	}
}

void mouseMove(int x, int y) {
	int dx = x - old_x;
	int dy = y - old_y;
	old_x = x;
	old_y = y;
	if (left_button_down) {
		camera.rotateX(dx / 15.0f);
		camera.rotateY(dy / 15.0f);
		
	}
	else if (right_button_down) {
		camera.roll(dx / 15.0f);
	}

}

void keyDown(GLubyte key, int x, int y) {
	switch (key) {
	case '1':
		tex_index = 1;
		break;
	case '2':
		tex_index = 2;
		break;
	case '3':
		tex_index = 3;
		break;
	case '0':
		tex_index = 0;
		break;
	case 'w':case 'W':
		camera.horizontalMove(1.0f, 0.0f);
		break;
	case 's':case 'S':
		camera.horizontalMove(-1.0f, 0.0f);
		break;
	case 'a':case 'A':
		camera.horizontalMove(0.0f, 1.0f);
		break;
	case 'd':case 'D':
		camera.horizontalMove(0.0f, -1.0f);
		break;
	case 'q':case 'Q':
		camera.verticalMove(1.0f);
		break;
	case 'e':case 'E':
		camera.verticalMove(-1.0f);
		break;
	case 'r':case 'R':
		camera.resetCamera();
		break;
	case 'j':case 'J': {
		v_x -= 0.05f;
		float tmp_r = R - v_y * v_y;
		v_z = -pow((tmp_r - v_x*v_x), 0.5);
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'l':case 'L': {
		v_x += 0.05f;
		float tmp_r = R - v_y * v_y;
		v_z = -pow((tmp_r - v_x*v_x), 0.5);
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'i':case 'I': {
		R -= v_y *v_y;
		v_y += 0.05f;
		R += v_y * v_y;
		float tmp_r = R - v_x * v_x;
		v_z = -pow((tmp_r - v_x*v_x), 0.5);
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'k':case 'K': {
		R -= v_y *v_y;
		v_y -= 0.05f;
		R += v_y * v_y;
		float tmp_r = r - v_x * v_x;
		v_z = -pow((tmp_r - v_x*v_x), 0.5);
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 27:
		exit(0);
	default:
		break;
	}
}

void idle() {
	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("test particle system");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}