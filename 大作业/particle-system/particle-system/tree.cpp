#include "tree.h"

Tree::Tree(){}

Tree::~Tree(){}

void Tree::init() {
	char *filename[] = { "data\\brack.BMP","data\\tree2.BMP" };
	bool rc;
	for (int i = 0; i < 2; i++) {
		rc = BuildTexture(filename[i], texture[i], 0, 0, 0);
	}
}

Vector3d Tree::set_size(double _height, double _width, double _levels) {
	double height, width, levels;
	height = _height > 5.0 ? _height : 5.0;
	height = _height < 25.0 ? _height : 25.0;
	width = _width > 6.0 ? 6.0 : _width;
	width = _width < 2.0 ? 2.0 : _width;
	levels = _levels > 8 ? 8 : _levels;
	levels = _levels < 3 ? 3 : _levels;
	Vector3d ret(height, width, levels);
	return ret;
}

void Tree::draw_single_tree(Vector3d _pos, Vector3d _size) {
	Vector3d position(_pos);
	Vector3d size(_size);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslated(position.x(), position.y(), position.z());
	glRotatef(-90, 1.0, 0.0, 0.0);

	/*使用glut二次曲面函数作图*/
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);	//创建平滑的法向量
	gluQuadricTexture(quadric, GL_TRUE);	//使用纹理

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	gluCylinder(quadric, 0.5f, 0.4f, size.x() * 0.4, 32, 32);	//树干

	glTranslatef(0.0, 0.0, size.x() * 0.3);
	glBindTexture(GL_TEXTURE_2D, texture[1]);					//树梢
	double curHeight = size.x() * 0.6, curWidth = size.y();
	for (int i = 0; i < size.z(); i++) {
		gluCylinder(quadric, curWidth, 0.0f, curHeight, 32, 32);
		glTranslatef(0.0, 0.0, curHeight * 0.7);
		curHeight = curHeight / 2;
		curWidth = curWidth / 2;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, 1, 1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Tree::draw() {
	Vector3d pos1(0, -1.0f, -5.0f);
	Vector3d size1 = set_size(15.0, 4.0, 3.0);
	Vector3d size2 = set_size(11.0, 3.0, 4.0);

	for (double i = pos1.z(); i > -55.0f; i -= 7.0f) {
		Vector3d tmp1(pos1.x(), pos1.y(), i);
		Vector3d tmp2(pos1.x(), pos1.y(), -i);
		draw_single_tree(tmp1, size1);
		if(i > -39.0f)
			draw_single_tree(tmp2, size2);
	}
}


AFlower::AFlower(){
	growth = -4 + 0.01*(rand() % 800);

	petals = 3 + rand() % 3;
	petal_color.blue = 0.01*(rand() % 100);
	petal_color.green = 0.01*(rand() % 100);
	petal_color.red = 0.01*(rand() % 100);

	stalk_color.blue = 0.2 + 0.01*(rand() % 80);
	stalk_color.green = 0.2 + 0.01*(rand() % 80);
	stalk_color.red = 0.0;

	petal_radius = 0.01*(15 + rand() % 15);
	stalk_angle = 1.0 + (rand() % 10);
	stalk_size = 20 * petal_radius;

	x_axis_stalk = 0.01*(rand() % 100);
	y_axis_stalk = 0.01*(rand() % 100);
	z_axis_stalk = 0.01*(rand() % 100);
}

AFlower::~AFlower() {};

void AFlower::draw(float x, float y, float z) {
	float x1, angle, radian_angle, y1 = 0.0, z1, x2, y2, z2;
	float r = 2.0;
	float petal = 0, angle_petal = 90;
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-90, 1.0, 0.0, 0.0);//making the stalk straight upwards
	glColor3f(stalk_color.red, stalk_color.green, stalk_color.blue);
	
	for (int i = 0; i<stalk_size; i++){
		
		glRotatef(stalk_angle, x_axis_stalk, y_axis_stalk, z_axis_stalk);
		gluCylinder(quadratic, 0.08*petal_radius, 0.08*petal_radius, 5 * petal_radius, 5, 5);

		glTranslatef(0.0, 0.0, 5 * petal_radius);//going to draw next stem;
		if (i>(stalk_size - 4) && i<stalk_size - 1){
			for (petal = 0; petal < stalk_size - 2; petal++) {
				glRotatef(360 / (stalk_size - 2), 0.0, 0.0, 1.0);

				radian_angle = 0;
				y1 = 0;
				glBegin(GL_QUAD_STRIP);
				for (angle = 0; angle <= 180; y1 -= 0.08) {
					x1 = petal_radius*sinf(radian_angle);
					z1 = petal_radius*cosf(radian_angle) - petal_radius;

					glVertex3f(-x1, y1, z1);
					glVertex3f(x1, y1, z1);

					if (angle > 159)
						angle += 10;
					else
						angle += 20;
					radian_angle = (3.1416 / 180)*angle;
				}
				glEnd();
			}
		}
	}
	glColor3f(0.80, 0.0, 0.0);
	gluCylinder(quadratic, 0.08*petal_radius, 0.4*petal_radius, 0.05, 5, 3);
	glRotatef(90, 1.0, 0.0, 0.0);

	glColor3f(petal_color.red, petal_color.green, petal_color.blue);

	int i = 0;

	for (petal = 0; petal<petals; petal++){
		glRotatef(360 / petals, 0.0, 1.0, 0.0);

		radian_angle = 0;
		glBegin(GL_QUAD_STRIP);
		for (angle = 0; angle <= 180; y1 = growth*petal_radius*sinf(radian_angle)){
			x1 = petal_radius*sinf(radian_angle);
			z1 = petal_radius*cosf(radian_angle) - petal_radius;

			glVertex3f(-x1, y1, z1);
			glVertex3f(x1, y1, z1);

			if (angle>159)
				angle += 10;
			else 
				angle += 20;
			radian_angle = (3.1416 / 180)*angle;
		}
		glEnd();
	}
	glColor3f(1, 1, 1);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
}

Flower::Flower() {};

Flower::~Flower() {};

void Flower::init() {
	for (int i = 0; i < 100; i++) {
		data[i] = new AFlower();
		x[i] = rand() % 19 + 2;
		y[i] = -1;
		z[i] = (rand() % 25 + 5);
	}
}

void Flower::draw() {
	for (int i = 0; i < 100; i++) 
		data[i]->draw(x[i], y[i], z[i]);
}
