#include "camera.h"
#include "ground.h"
#include "sky.h"
#include "particle2.h"
#include "utility.h"

GLCamera camera;
Ground ground;
Sky sky;
ParticleSystem *flame;
GLuint texture[2];

int old_x, old_y;
bool left_button_down = false;
bool right_button_down = false;

Particle* init_flame()
{
	Vector3d size(rand() % 90 * 0.02f, rand() % 90 * 0.02f, rand() % 90 * 0.02f);
	Vector3d velo(float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600);
	Vector3d acc(1.0f*(rand() % 3 - 1) / 9000000, 4.9 / 4000000, 1.0f*(rand() % 3 - 1) / 9000000);
	Vector3d angle(rand() % 360, rand() % 360, rand() % 360);
	GLuint tex = texture[0];
	Particle* p = new Particle(velo, acc, size, angle, false, Vector3d(1.0, 0.0, 0.0) , tex, (float)(rand() % 50 + 10), 0.2f, false);
	return p;
}

bool out_of_bound(Particle* p)
{
	Vector3d position = p->getPosition();
	if (position.y() < 0.0f) {
		return true;
	}
	return false;
}

void init(void)
{
	/* ��ʼ���ӽ� */
	Vector3d pos(0.0, 0.0, 10.0);
	Vector3d target(0.0, 0.0, 0.0);
	Vector3d up(0.0, 1.0, 0.0);
	camera.initCamera(pos, target, up);



	/** �û��Զ���ĳ�ʼ������ */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);//ָ����Ȼ����������ֵ(������Ȼ��������ֵ����Ϊ1)
	glDepthFunc(GL_LEQUAL);//ָ��������Ȼ���Ƚ�ֵ(���½��������ֵ��ԭ����1�Ƚϣ�<=��ͨ����������)
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);             /**< ��������ӳ�� */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);    /**< ���û�����ӻ�ð�͸��Ч�� */
	glEnable(GL_BLEND);                  /**< ���û�� */

										 /**  �������� */
	//char *filename[] = {"data\\SAND1.BMP" };
	//texture[0] = LoadTextures3(filename[0]);

	/* ��ʼ������ */
	ground.init();

	/**/
	sky.init();

	/**/
	//flame = new ParticleSystem(400, 1.5f, -8.0f, -19.5f, 2.5f, -7.5f, -20.5f);
	//flame->init_system(init_flame, out_of_bound);

}



/** ��Ⱦ */
void display(void)
{
	/** �û��Զ���Ļ��ƹ��� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glLoadIdentity();
	camera.setModelViewMatrix();


	/* ���Ƶ��� */
	ground.draw();

	Vector3d pos = camera.myPosition();
	sky.render(pos.x(), pos.y(), pos.z());

	/** �������� */
	glEnable(GL_BLEND);
	//flame->show();
	//flame->update();
	glDisable(GL_BLEND);

	glFlush(); /**<ǿ��ִ�����е�OpenGL���� */
}

void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);    /**< ���������ӿ� */
	camera.setShape(45.0f, (GLfloat)width / (GLfloat)height, 0.1, 1700.0f);
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1700.0f);
	*/
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
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("test particle system");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	//glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
