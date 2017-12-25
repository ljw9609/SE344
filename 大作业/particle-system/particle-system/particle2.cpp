#include "particle2.h"

void Particle::draw_with_tex(GLuint texture) {
	glDepthMask(GL_FALSE);
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(angle.x(), 1, 0, 0);
	glRotatef(angle.y(), 0, 1, 0);
	glRotatef(angle.z(), 0, 0, 1);
	glScalef(size.x(), size.y(), size.z());
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glColor4ub(0, 0, 0, 0);
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
	//glColor3f(0, 0, 0);
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
	Vector3d init_color, GLuint init_tex, float init_life, float init_dec, bool init_is_forever) {
	velo = init_velo;
	acc = init_acc;
	size = init_size;
	angle = init_angle;
	has_tex = init_has_tex;
	if (has_tex == true)
		texture = init_tex;
	else
		color = init_color;
	lifetime = init_life;
	dec = init_dec;
	is_forever = init_is_forever;
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

		velo.x() += acc.x();
		velo.y() += acc.y();
		velo.z() += acc.z();

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

bool Particle::isForever() {
	return is_forever;
}

ParticleSystem::ParticleSystem(){}

ParticleSystem::~ParticleSystem(){}

ParticleSystem::ParticleSystem(int init_speed, float init_x1, float init_y1, float init_z1,
	float init_x2, float init_y2, float init_z2) {
	emit_speed = init_speed;
	x1 = init_x1;
	x2 = init_x2;
	y1 = init_y1;
	y2 = init_y2;
	z1 = init_z1;
	z2 = init_z2;
	data = new Particle*[init_speed];
	dead = new bool[init_speed];
	for (int i = 0; i < emit_speed; i++) {
		dead[i] = false;
	}
	particle_num = emit_speed;
}

void ParticleSystem::init_system(Particle* (init)(), bool(*judge)(Particle*)) {
	for (int i = 0; i < emit_speed; i++) {
		f = init;
		out_of_bound = judge;
		data[i] = init();
		int pos_x = rand() % emit_speed;
		int pos_y = rand() % emit_speed;
		int pos_z = rand() % emit_speed;
		Vector3d position;
		position.x() = 1.0f * pos_x / emit_speed * (x2 - x1) + x1;
		position.y() = 1.0f * pos_y / emit_speed * (y2 - y1) + y1;
		position.z() = 1.0f * pos_z / emit_speed * (z2 - z1) + z1;
		data[i]->setPosition(position);
	}
}

void ParticleSystem::update() {
	for (int i = 0; i < emit_speed; i++) {
		data[i]->draw();
		data[i]->update();
		if (data[i]->getLifetime() <= 0 || data[i]->isForever() && out_of_bound(data[i])) {
			delete data[i];
			data[i] = f();
			int pos_x = rand() % emit_speed;
			int pos_y = rand() % emit_speed;
			int pos_z = rand() % emit_speed;
			Vector3d position;
			position.x() = 1.0f * pos_x / emit_speed * (x2 - x1) + x1;
			position.y() = 1.0f * pos_y / emit_speed * (y2 - y1) + y1;
			position.z() = 1.0f * pos_z / emit_speed * (z2 - z1) + z1;
			data[i]->setPosition(position);
		}
		else if (out_of_bound(data[i])) {
			delete data[i];
			data[i] = f();
			int pos_x = rand() % emit_speed;
			int pos_y = rand() % emit_speed;
			int pos_z = rand() % emit_speed;
			Vector3d position;
			position.x() = 1.0f * pos_x / emit_speed * (x2 - x1) + x1;
			position.y() = 1.0f * pos_y / emit_speed * (y2 - y1) + y1;
			position.z() = 1.0f * pos_z / emit_speed * (z2 - z1) + z1;
			data[i]->setPosition(position);
		}
	}
}

void ParticleSystem::reset() {
	for (int i = 0; i < emit_speed; i++) {
		dead[i] = false;
	}
	for (int i = 0; i < emit_speed; i++) {
		data[i] = f();
		int pos_x = rand() % emit_speed;
		int pos_y = rand() % emit_speed;
		int pos_z = rand() % emit_speed;
		Vector3d position;
		position.x() = 1.0f * pos_x / emit_speed * (x2 - x1) + x1;
		position.y() = 1.0f * pos_y / emit_speed * (y2 - y1) + y1;
		position.z() = 1.0f * pos_z / emit_speed * (z2 - z1) + z1;
		data[i]->setPosition(position);
	}
	particle_num = emit_speed;
}

/*
void ParticleSystem::disappear() {
	if (particle_num == 0)return;
	for (int i = 0; i < emit_speed; i++) {
		if (dead[i] == true)continue;
		data[i]->draw();
		data[i]->update();
		if (out_of_bound(data[i])) {
			delete data[i];
			particle_num--;
			dead[i] = true;
		}
	}
}

void ParticleSystem::show() {
	for (int i = 0; i < emit_speed; i++) {
		if (!dead[i]) {
			data[i]->draw();
		}
	}
}
*/

