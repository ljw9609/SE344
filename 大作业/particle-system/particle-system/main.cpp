#include "camera.h"
#include "ground.h"
#include "sky.h"
#include "particle.h"
#include "utility.h"
#include "objloader.h"
#include "tree.h"
#include "fence.h"
#include "wall.h"
#include "drawScene.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;


/* 摄像机类 */
Camera camera;

/* 地面类 */
Ground ground;

/* 天空类 */
Sky sky;

/* 树木类 */
Tree tree;

/* 花朵类 */
Flower flower;

/* 栅栏类 */
Fence fence;

/* 围墙类 */
Wall wall;


/* obj模型类 */
ObjLoader mountain;
ObjLoader pikachu;
ObjLoader dinosaur;

/* 粒子系统类 */
GLuint texture[10];
ParticleSystem *flame;
ParticleSystem *snow;
ParticleSystem *lazer;
ParticleSystem *waterfall;
ParticleSystem *sun;
ParticleSystem *sun2;
ParticleSystem *fountain;
ParticleSystem *smoke;

/* 基本控制参数 */
int r, g, b;
int old_x, old_y;
bool left_button_down = false;
bool right_button_down = false;
int p_flame = -1;
int p_fountain = -1;
int p_waterfall = -1;
int p_bubble = 1;
int p_lazer = -1;
int p_snow = -1;
unsigned int mode[] = {2,4,5};
int tex_index = 4;
int mode_index = 1;
int music_stat = 1;
int current_light = -1;

#define FLAME 11
#define FOUNTAIN 12
#define WATERFALL 13
#define BUBBLE 14
#define LAZER 15
#define SNOW 16

/* 控制镭射光方向参数 */
float v_x = 0;
float v_y = 0;
float v_z = -1.2f;


/* 帧数计算参数 */
int time;
int frame = 0;
int timebase = 0;
int fps = 0;

/* 部分纹理句柄 */
GLuint campfire_tex;
GLuint fountain_tex[2];
GLuint house_tex[5];

Particle* init_flame(){
	Vector3d size(rand() % 30 * 0.02f, rand() % 30 * 0.02f, rand() % 30 * 0.02f);
	Vector3d velo(float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 9000000, 4.9 / 4000000, 1.0f*(rand() % 3 - 1) / 9000000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[0];
	Vector3d color(0, 0, 0);
	Particle* p = new Particle(velo, acc, size, angle, true, color, tex, (float)(rand() % 50 + 10), 0.2f, false,1);
	return p;
}

Particle* init_snow() {
	Vector3d size(rand() % 50 * 0.04f, rand() % 50 * 0.04f, rand() % 50 * 0.04f);
	Vector3d velo(float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 400, float(rand() % 10 - 4) / 800);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 90000, -0.003, 1.0f*(rand() % 3 - 1) / 90000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[1];
	Vector3d color(0, 0, 0);
	Particle* p = new Particle(velo, acc, size, angle, true, color, tex, (float)(rand() % 50 + 10), 0.2f, false,1);
	return p;
}

Particle* init_lazer() {
	Vector3d size(rand() % 50 * 0.01f, rand() % 50 * 0.01f, rand() % 50 * 0.01f);
	Vector3d velo(v_x, v_y, v_z);
	Vector3d acc(0.000007f * (rand() % 2500 - rand() % 2500), -0.01, 0.04);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[0];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 1.6f, false,1);
	return p;
}

Particle* init_waterfall() {
	Vector3d size(rand() % 50 * 0.02f, rand() % 50 * 0.02f, rand() % 50 * 0.02f);
	Vector3d velo(0, 0, 0.2f);
	Vector3d acc(0, -0.025, 0);
	Vector3d angle(rand() % 180, rand() % 180, rand() % 180);
	GLuint tex = texture[3];
	Vector3d color(0, 0, 0);
	Particle* p = new Particle(velo, acc, size, angle, true, color, tex, (float)(rand() % 50 + 10), 1.6f, false,3);
	return p;
}

Particle* init_fountain() {
	Vector3d size(rand() % 50 * 0.02f, rand() % 50 * 0.02f, rand() % 50 * 0.02f);
	Vector3d velo(0.00007f * (rand() % 2500 - rand() % 2500), 0.0004f * (rand() % 2500), 0.00007f * (rand() % 2500 - rand() % 2500));
	//Vector3d velo(0.00007f * (rand() % 2500 - rand() % 2500), 0.1f, 0.00007f * (rand() % 2500 - rand() % 2500));
	Vector3d acc(0, -0.04, 0);
	//Vector3d acc(0, -0.01, 0);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[3];
	//Vector3d color(rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f);
	Vector3d color(0, 0, 0);
	Particle* p = new Particle(velo, acc, size, angle, true, color, tex, (float)(rand() % 50 + 10), 0.6f, false,3);
	return p;
}

Particle* init_sun() {
	Vector3d size(rand() % 30 * 0.02f, rand() % 30 * 0.02f, rand() % 30 * 0.02f);
	Vector3d velo(0, 0, 0);
	Vector3d acc(0, 0, 0);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[0];
	Particle* p = new Particle(velo, acc, size, angle, true, Vector3d(1.0, 0.0, 0.0), tex, (float)(rand() % 50 + 10), 0.2f, false,1);
	return p;
}

Particle* init_sun2() {
	Vector3d size(rand() % 30 * 0.02f, rand() % 30 * 0.02f, rand() % 30 * 0.02f);
	Vector3d velo(float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 9000000, 4.9 / 4000000, 1.0f*(rand() % 3 - 1) / 9000000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[2];
	Vector3d color(rand() % 10 * 0.1f, rand() % 4 * 0.1f, 0);
	Particle* p = new Particle(velo, acc, size, angle, true, color, tex, (float)(rand() % 50 + 10), 0.2f, false,1);
	return p;
}

Particle* init_smoke() {
	Vector3d size(0.2f, 0.2f, 0.2f);
	Vector3d velo1(0.00007f * (rand() % 2500 - rand() % 2500), 0.1f, 0.00007f * (rand() % 2500 - rand() % 2500));
	Vector3d velo2(0.06f,0.06f,0.06f);
	Vector3d velo[] = {velo1,velo1,velo2};
	Vector3d acc1(0, -0.006f, 0);
	Vector3d acc2(0.1f, 0, 0.1f);
	Vector3d acc[] = { acc1,acc1,acc2 };

	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[tex_index];
	Vector3d color(0, 0, 0);
	Particle* p = new Particle(velo[mode_index], acc[mode_index], size, angle, true, color, tex, (float)(rand() % 50 + 10), 0.2f, false, mode[mode_index]);
	return p;
}

bool out_of_bound(Particle* p){
	Vector3d position = p->getPosition();
	if (position.y() < -1.0f) {
		return true;
	}
	return false;
}

bool out_of_bound2(Particle* p) {
	Vector3d position = p->getPosition();
	Vector3d velo = p->getVelocity();
	if (position.y() < -1.0f || position.z() < -60.0f || position.x() < -60.0f 
		|| (-0.0001 < velo.z() && velo.z() < 0.0001)){
		return true;
	}
	return false;
}

bool out_of_bound3(Particle* p) {
	Vector3d position = p->getPosition();
	if (position.x() < 23.0f || position.x() > 35.0f || position.z() > 26.0f || position.z() < 14.0f)
		return true;
	return false;
}

void set_lighting() {
	GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f};
	GLfloat sun_light_position[] = { 0.0f, 30.0f, 0.0f, 1.0f };
	GLfloat sun_light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat cut_off[] = { 45.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, cut_off);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	
	glLightfv(GL_LIGHT1, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun_light_specular);

	
	glEnable(GL_LIGHTING);
	if (current_light == 1) {
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT0);
	}
	else if (current_light == -1) {
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	glEnable(GL_DEPTH_TEST);
}

void init(void){
	/* 初始化视角 */
	Vector3d pos(-4.0, 10.0, 30.0);
	Vector3d target(-4.0, 7.0, 0.0);
	Vector3d up(0.0, 1.0, 0.0);
	camera.initCamera(pos, target, up);
										
	/* 初始化地面 */
	ground.init();

	/* 初始化天空 */
	sky.init();

	/* 初始化墙壁 */
	wall.init();

	/* 初始化栅栏 */
	fence.init();

	/* 初始化树木，花 */
	tree.init();
	flower.init();
	

	/* 初始化纹理读取 */
	bool ret;
	char *filename[] = { "data\\fire.BMP","data\\snow.BMP","data\\gold.BMP","data\\bubble.BMP","data\\circle.BMP"};
	for(int i = 0;i < 5;i++)
		ret = BuildTexture(filename[i], texture[i], 0, 0, 0);
	
	char *camp_filename[] = {"data\\brack.BMP"};
	campfire_tex = LoadTextures3(camp_filename[0]);

	char *house_filename[] = {"data\\wood3.BMP","data\\wood4.BMP","data\\roof.BMP" };
	for (int i = 0; i < 3; i++) {
		house_tex[i] = LoadTextures3(house_filename[i]);
	}

	char *fountain_filename[] = { "data\\wall2.BMP", "data\\water.BMP"};
	for (int i = 0; i < 2; i++) {
		fountain_tex[i] = LoadTextures3(fountain_filename[i]);
	}
	


	/* 初始化粒子系统 */
	flame = new ParticleSystem(400, -17.0f, 0.5f, -17.0f, -15.0f, 2.0f, -15.0f);
	flame->init_system(init_flame, out_of_bound);

	snow = new ParticleSystem(1000, -59, 25, -59, -1, 25, -4.5);
	snow->init_system(init_snow, out_of_bound);

	lazer = new ParticleSystem(400, -46.25, 9.8, -19.75, -45.75, 9.8, -20.25);
	lazer->init_system(init_lazer, out_of_bound2);

	waterfall = new ParticleSystem(1000, 27, 12, -30, 32, 12, -30);
	waterfall->init_system(init_waterfall, out_of_bound);

	fountain = new ParticleSystem(1000, 28, 5, 21, 30, 5, 19);
	fountain->init_system(init_fountain, out_of_bound3);

	sun = new ParticleSystem(400, -0.5, 29.5, 0.5, 0.5, 30.5, -0.5);
	sun->init_system(init_sun, out_of_bound);

	sun2 = new ParticleSystem(1000, -1, 29, 1, 1, 31, -1);
	sun2->init_system(init_sun2, out_of_bound);

	smoke = new ParticleSystem(1000, -25, 2, 20, -25, 2, 20);
	smoke->init_system(init_smoke, out_of_bound);
	
	/* 初始化obj模型 */
	mountain.load_obj("mountain.OBJ");
	pikachu.load_obj("pikachu.OBJ");
	dinosaur.load_obj("dino.OBJ");
	
	if (music_stat == 1) {
		PlaySound("data\\bgm.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}

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
	strings[1] = "Movement: WASD, Pitch and Yaw: Left Mouse, Roll: Right Mouse";
	strings[2] = "Control lazer bean: IJKL, Reset Camera: R, Music On/Off: M, Open menu:Middle Mouse";
	strings[3] = "Change particle texture: C, Change particle motion: x, Change light: Z";

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
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);


	/* 设置摄像机 */
	glLoadIdentity();
	camera.setModelViewMatrix();
	
	/* 计算帧数 */
	calculate_fps();

	/* 设置光照 */
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
	draw_fountain(29, -1, 20, fountain_tex);

	/* 绘制假山 */	
	glPushMatrix();
	glTranslatef(35, 3, -35);
	glScalef(0.06, 0.06, 0.06);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	mountain.render_obj();
	glPopMatrix();
	draw_fake_lake(30, -1, -35, fountain_tex);
	
	/* 绘制皮卡丘 */
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
	
	/* 绘制恐龙 */
	glPushMatrix();
	glTranslatef(-46, -1, -10);
	glRotatef(90, 0, 1, 0);
	glScalef(0.02, 0.02, 0.02);
	dinosaur.render_obj();
	glPopMatrix();

	/* 绘制粒子系统 */
	
	if(p_flame == 1)
		flame->update();
	if(p_snow == 1)
		snow->update();
	if(p_waterfall == 1)
		waterfall->update();
	if(p_fountain == 1)
		fountain->update();
	if(p_bubble == 1)
		smoke->update();
	if (p_lazer == 1)
		lazer->update();
	sun2->update();
	


	/* 屏幕打印信息 */
	glDisable(GL_LIGHTING);
	draw_text();
	glEnable(GL_LIGHTING);

	/* 交换前后缓存 */
	glutSwapBuffers();
}

void ChangeSize(int width, int height){
	glViewport(0, 0, width, height);    
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
		camera.rotateX(dx / 7.0f);
		camera.rotateY(dy / 7.0f);
		
	}
	else if (right_button_down) {
		camera.rotateRoll(dx / 7.0f);
	}
	
}

void keyDown(GLubyte key, int x, int y) {
	switch (key) {
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
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'l':case 'L': {
		v_x += 0.05f;
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'i':case 'I': {
		v_y += 0.05f;
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'k':case 'K': {
		v_y -= 0.05f;
		cout << "current vx = " << v_x << ",vy = " << v_y << ",vz = " << v_z << endl;
		break;
	}
	case 'm':case 'M': {
		music_stat *= -1;
		if (music_stat == 1) {
			PlaySound("data\\bgm.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		}
		else if (music_stat == -1) {
			PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME);
		}
		break;
	}
	case 'c':case 'C': {
		tex_index += 1;
		if (tex_index > 4)
			tex_index = 0;
		break;
	}
	case 'x':case 'X': {
		mode_index++;
		if (mode_index > 2)
			mode_index = 0;
		break;
	}
	case 'z':case 'Z': {
		current_light *= -1;
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

void menuEvent(int option) {
	switch (option) {
	case FLAME: {
		p_flame *= -1;
		break;
	}
	case SNOW: {
		p_snow *= -1;
		break;
	}
	case WATERFALL: {
		p_waterfall *= -1;
		break;
	}
	case FOUNTAIN: {
		p_fountain *= -1;
		break;
	}
	case BUBBLE: {
		p_bubble *= -1;
		break;
	}
	case LAZER: {
		p_lazer *= -1;
		break;
	}
	}
}

void createMenu() {
	int menu;
	menu = glutCreateMenu(menuEvent);

	glutAddMenuEntry("campfire", FLAME);
	glutAddMenuEntry("snow", SNOW);
	glutAddMenuEntry("fountain", FOUNTAIN);
	glutAddMenuEntry("waterfall", WATERFALL);
	glutAddMenuEntry("lazer", LAZER);
	glutAddMenuEntry("bubble", BUBBLE);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("图形学大作业");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);
	createMenu();
	glutMainLoop();
	return 0;
}