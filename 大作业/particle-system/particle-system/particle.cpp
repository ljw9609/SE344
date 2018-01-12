#include "particle.h"
#define PI 3.141592653589793

void Particle::draw_with_tex(GLuint texture) {
	float r = 0;
	float g = 0;
	float b = 0;
	if (color.x() != 0 || color.y() != 0 || color.z() != 0) {
		r = color.x();
		g = color.y();
		b = color.z();
	}
	glDepthMask(GL_FALSE);
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(angle.x(), 1, 0, 0);
	glRotatef(angle.y(), 0, 1, 0);
	glRotatef(angle.z(), 0, 0, 1);
	glScalef(size.x(), size.y(), size.z());
	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (r != 0 || g != 0 || b!= 0) {
		glColor3f(r, g, b);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glPopMatrix();
	glDepthMask(GL_TRUE);
	//glutPostRedisplay();
}

void Particle::draw_with_color() {
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(angle.x(), 1, 0, 0);
	glRotatef(angle.y(), 0, 1, 0);
	glRotatef(angle.z(), 0, 0, 1);
	glScalef(size.x(), size.y(), size.z());

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(color.x(), color.y(), color.z());
	glutSolidSphere(0.5, 32, 32);
	/*
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -0.5f); 
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
	*/

	glPopMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
	//glDisable(GL_BLEND);
	//glutPostRedisplay();
}

Particle::Particle(){}

Particle::~Particle(){}

Particle::Particle(Vector3d init_velo, Vector3d init_acc, Vector3d init_size, Vector3d init_angle, bool init_has_tex,
	Vector3d init_color, GLuint init_tex, float init_life, float init_dec, bool init_is_forever, unsigned int init_mode) {
	velo = init_velo;
	acc = init_acc;
	size = init_size;
	angle = init_angle;
	has_tex = init_has_tex;
	if (has_tex == true)
		texture = init_tex;
	color = init_color;
	lifetime = init_life;
	dec = init_dec;
	is_forever = init_is_forever;
	motion_mode = init_mode;
	ftheta = 0.0f;
}

void Particle::init_particle(Vector3d init_velo, Vector3d init_acc, Vector3d init_pos, Vector3d init_size, Vector3d init_angle, bool init_has_tex,
	Vector3d init_color, GLuint init_tex, float init_life, float init_dec, bool init_is_forever) {
	velo = init_velo;
	acc = init_acc;
	position = init_pos;
	size = init_size;
	angle = init_angle;
	has_tex = init_has_tex;
	color = init_color;
	if (has_tex == true)
		texture = init_tex;
	else
		color = init_color;
	
	lifetime = init_life;
	dec = init_dec;
	is_forever = init_is_forever;
}

void Particle::draw() {
	if (lifetime > 0 || is_forever) {
		if (has_tex)
			draw_with_tex(texture);
		else
			draw_with_color();
	}
}

void Particle::update() {
	if (lifetime > 0 || is_forever) {
		position.x() += velo.x();
		position.y() += velo.y();
		position.z() += velo.z();

		float vx1 = velo.x();
		float vz1 = velo.z();
		float ax1 = acc.x();
		float az1 = acc.z();
		velo.x() += acc.x();
		velo.y() += acc.y();
		velo.z() += acc.z();

		float vx2 = velo.x();
		float vz2 = velo.z();

		switch (motion_mode) {
		case 2: {	//bubble sin
			if (velo.y() > 0.1 || velo.y() < -0.1)
				acc.y() *= -1.0f;
			break;
		}
		case 3: {	//fountain waterfall collision
			if (position.y() < 1.0f) {
				velo.y() *= -1.0f;
				velo.y() *= 0.5f;
			}
			break;
		}
		case 4: {	//bubble collision
			if (position.y() < 0.0f) {
				velo.y() *= -1.0f;
				velo.y() *= 0.8f;
			}
			//house front
			if (position.x() <= -27.0f && position.x() >= -47.0f) {
				if (position.z() > 26.5f) {
					velo.z() *= -1.0f;
					velo.z() *= 0.8f;
				}
			}
			//house right
			if (position.z() >= 26.5f && position.z() <= 41.5f) {
				if (position.x() < -27.0f) {
					velo.x() *= -1.0f;
					velo.z() *= 0.8f;
				}
			}
			//wall
			if (position.z() > 60.0f) {
				velo.z() *= -1.0f;
				velo.z() *= 0.8f;
			}
			if (position.x() < -60.0f) {
				velo.x() *= -1.0f;
				velo.x() *= 0.8f;
			}
			break;
		}
		
		case 5: {	//twister
			if (velo.x() > 0.1f || velo.x() < -0.1f)
				acc *= -1.0f;
			break;
		}
		default:
			break;
		}

		if (!is_forever)
			lifetime -= dec;
	}
}

void Particle::setPosition(Vector3d pos) {
	position = pos;
}

Vector3d Particle::getPosition() {
	return position;
}

float Particle::getLifetime() {
	return lifetime;
}

Vector3d Particle::getVelocity() {
	return velo;
}

bool Particle::isForever() {
	return is_forever;
}

ParticleSystem::ParticleSystem(){}

ParticleSystem::~ParticleSystem(){}

ParticleSystem::ParticleSystem(int init_num, float init_x1, float init_y1, float init_z1,
	float init_x2, float init_y2, float init_z2) {
	particle_num = init_num;
	x1 = init_x1;
	x2 = init_x2;
	y1 = init_y1;
	y2 = init_y2;
	z1 = init_z1;
	z2 = init_z2;
	particle_num = particle_num;
	data = new Particle*[init_num];
	dead = new bool[init_num];
	for (int i = 0; i < particle_num; i++) {
		dead[i] = false;
	}
	
}

void ParticleSystem::init_system(Particle* (init)(), bool(*judge)(Particle*)) {
	for (int i = 0; i < particle_num; i++) {
		f = init;
		out_of_bound = judge;
		data[i] = init();
		int pos_x = rand() % particle_num;
		int pos_y = rand() % particle_num;
		int pos_z = rand() % particle_num;
		Vector3d position;
		position.x() = 1.0f * pos_x / particle_num * (x2 - x1) + x1;
		position.y() = 1.0f * pos_y / particle_num * (y2 - y1) + y1;
		position.z() = 1.0f * pos_z / particle_num * (z2 - z1) + z1;
		data[i]->setPosition(position);
	}
}

void ParticleSystem::update() {
	for (int i = 0; i < particle_num; i++) {
		data[i]->draw();
		data[i]->update();
		if (data[i]->getLifetime() <= 0 || data[i]->isForever() && out_of_bound(data[i])) {
			delete data[i];
			data[i] = f();
			int pos_x = rand() % particle_num;
			int pos_y = rand() % particle_num;
			int pos_z = rand() % particle_num;
			Vector3d position;
			position.x() = 1.0f * pos_x / particle_num * (x2 - x1) + x1;
			position.y() = 1.0f * pos_y / particle_num * (y2 - y1) + y1;
			position.z() = 1.0f * pos_z / particle_num * (z2 - z1) + z1;
			data[i]->setPosition(position);
		}
		else if (out_of_bound(data[i])) {
			delete data[i];
			data[i] = f();
			int pos_x = rand() % particle_num;
			int pos_y = rand() % particle_num;
			int pos_z = rand() % particle_num;
			Vector3d position;
			position.x() = 1.0f * pos_x / particle_num * (x2 - x1) + x1;
			position.y() = 1.0f * pos_y / particle_num * (y2 - y1) + y1;
			position.z() = 1.0f * pos_z / particle_num * (z2 - z1) + z1;
			data[i]->setPosition(position);
		}
	}
}

void ParticleSystem::reset() {
	for (int i = 0; i < particle_num; i++) {
		dead[i] = false;
	}
	for (int i = 0; i < particle_num; i++) {
		data[i] = f();
		int pos_x = rand() % particle_num;
		int pos_y = rand() % particle_num;
		int pos_z = rand() % particle_num;
		Vector3d position;
		position.x() = 1.0f * pos_x / particle_num * (x2 - x1) + x1;
		position.y() = 1.0f * pos_y / particle_num * (y2 - y1) + y1;
		position.z() = 1.0f * pos_z / particle_num * (z2 - z1) + z1;
		data[i]->setPosition(position);
	}
	particle_num = particle_num;
}

void ParticleSystem::set_emit_position(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2) {
	x1 = _x1;
	y1 = _y1;
	z1 = _z1;
	x2 = _x2;
	y2 = _y2;
	z2 = _z2;
}



