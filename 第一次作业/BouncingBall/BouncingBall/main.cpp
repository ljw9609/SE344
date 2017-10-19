#include "ObjLoader.h"
#include <iostream>
using namespace std;

string filePath = "";
ObjLoader objModel = ObjLoader(filePath);
static double PI = acos(-1);

#define monkey 1
#define ball 2
#define reset 3

int TIME = 33;
float dt = 0.15f;
float vy = 0.0f;
float vx = 0.0f;
float g = 9.8;
int ydirection = 3;     //1:down,2:up,3:static
int xdirection = 3;     //1:right,2:left,3:static

float xsite = 15;
float ysite = 1.5;
float rsize = 0.15 * 10;

float xstep = 0.0f;
float ystep = 0.0f;


void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("BouncingBall");

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0f, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

}

void display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0, 0, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(xsite, ysite, 0.0f);
	if (filePath == "") {
		glutSolidSphere(rsize, 40, 80);
	}
	else {
		objModel.Draw();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(0, 30, 0, 30 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	//gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myIdle()
{
	glutPostRedisplay();
}

void TimerFunction(int value)
{

	if (xdirection == 1) {
		if (xsite + rsize > 30) {
			vx = -(vx*0.8);
			xdirection = 2;
		}
	}
	else if (xdirection == 2) {
		if (xsite - rsize < 0) {
			vx = -(vx*0.8);
			xdirection = 1;
		}
	}
	else {
		vx = 0.0;
		xsite = xsite;
	}

	if (abs(vx) - 0.00<1e-6) {
		xdirection = 3;
		vx = 0.0f;
		xsite = xsite;
	}
	else {
		xsite += vx*dt;
		//printf("xsite=%f\n", xsite);
		if (ydirection == 3) {
			if (xdirection == 1) {
				vx = vx - 2 * dt;
				if (vx <= 0.0) {
					xdirection = 3;
				}
			}
			else if (xdirection == 2) {
				vx = vx + 2 * dt;
				if (vx >= 0.0) {
					xdirection = 3;
				}
			}
			//printf("vx=%f\n", vx);

		}
	}


	if (ysite >= 30 - rsize)        ysite = 30 - rsize;

	if (ydirection == 1) { //down
		ysite = ysite - (vy*dt + 0.5*g*dt*dt);
		//printf("1ysite=%f,vy=%f\n", ysite, vy);
		vy = vy + g*dt;
		if (vy - 0.00<1e-6) {
			ysite = rsize;
			vy = 0.0;
			ydirection = 3;
			//printf("ysite=%f,rsize=%f\n", ysite, rsize);
		}
		if (ysite - rsize<0) {
			ydirection = 2;
			vy = vy - 2.0;
		}
	}
	else if (ydirection == 2) {   //up
		ysite = ysite + (vy*dt - 0.5*g*dt*dt);
		//printf("2ysite=%f,vy=%f\n", ysite, vy);
		vy = vy - g*dt;
		if (vy <= 0.0) {
			ydirection = 1;
		}
		if (ysite + rsize >= 30) {
			ydirection = 1;
		}
	}
	else {
		ysite = rsize;
		vy = 0.0;
	}

	glutPostRedisplay();
	glutTimerFunc(TIME, TimerFunction, 1);
}

void OnMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		if (ysite == rsize && vy == 0.0) {
			ydirection = 2;
			vy = 20;
		}
	}
	if (button == GLUT_MIDDLE_BUTTON&&state == GLUT_DOWN) {
		if (ysite == rsize && vy == 0.0 && vx == 0.0) {
			ydirection = 2;
			xdirection = 2;
			vy = 20;
			vx = -10;
		}
	}
}

void specialKeyFunc(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F7: {
		g -= 0.2;
		printf("g=%f\n", g);
		break;
	}
	case GLUT_KEY_F9: {
		g += 0.2;
		printf("g=%f\n", g);
		break;
	}
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}
void keyBoardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case ' ': {
		vy = 20;
		ydirection = 2;
		break;
	}
	case 'r': {
		xdirection = 3;
		ydirection = 3;
		vx = 0;
		vy = 0;
		xsite = 15;
		ysite = rsize;
		break;
	}
	case 27:
		exit(0);
	}
	glutPostRedisplay();
}

void processMenuEvents(int option) {
	switch (option) {
	case monkey: {
		filePath = "data\\monkey.obj";
		objModel = ObjLoader(filePath);
		break;
	}
	case ball: {
		filePath = "";
		break;
	}
	case reset: {
		xdirection = 3;
		ydirection = 3;
		vx = 0;
		vy = 0;
		xsite = 15;
		ysite = rsize;
		break;
	}
	}
}

void createGlutMenu() {
	int menu;
	menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("monkey.obj", monkey);
	glutAddMenuEntry("ball", ball);
	glutAddMenuEntry("reset", reset);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char* argv[])
{
	bool use_model = false;
	string buf;
	cout << "Use obj model or not?(Y/N)?";
	cin >> buf;
	if (buf == "Y" || buf == "y") {
		cout << endl;
		cout << "Enter file address:";
		cin >> filePath;
		//cout << filePath << endl;
		objModel = ObjLoader(filePath);
	}
	else if (buf == "N" || buf == "n") {
		filePath = "";
	}
	else if (buf == "Q" || buf == "q") {
		exit(0);
	}
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(myIdle);
	glutMouseFunc(OnMouse);
	glutKeyboardFunc(keyBoardFunc);
	glutSpecialFunc(specialKeyFunc);
	glutTimerFunc(TIME, TimerFunction, 1);
	createGlutMenu();
	glutMainLoop();
	return 0;
}

