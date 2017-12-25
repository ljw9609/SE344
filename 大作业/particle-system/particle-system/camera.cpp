#include "camera.h"

GLCamera::GLCamera() {}

GLCamera::~GLCamera() {}

void GLCamera::initCamera(const Vector3d &pos, const Vector3d &target, const Vector3d &up) {
	m_pos = pos;
	m_target = target;
	m_up = up;
	n = Vector3d(pos.x() - target.x(), pos.y() - target.y(), pos.z() - target.z());
	u = Vector3d(up.cross(n).x(), up.cross(n).y(), up.cross(n).z());
	v = Vector3d(n.cross(u).x(), n.cross(u).y(), n.cross(u).z());


	n.normalize();
	u.normalize();
	v.normalize();

	backup.push_back(m_pos);
	backup.push_back(m_target);
	backup.push_back(m_up);
	backup.push_back(n);
	backup.push_back(u);
	backup.push_back(v);
	setModelViewMatrix();
}

void GLCamera::setModelViewMatrix() {
	double m[16];
	m[0] = u.x(); m[4] = u.y(); m[8] = u.z(); m[12] = -m_pos.dot(u);
	m[1] = v.x(); m[5] = v.y(); m[9] = v.z(); m[13] = -m_pos.dot(v);
	m[2] = n.x(); m[6] = n.y(); m[10] = n.z(); m[14] = -m_pos.dot(n);
	m[3] = 0;  m[7] = 0;  m[11] = 0;  m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m);     //用M矩阵替换原视点矩阵
}

void  GLCamera::setShape(float viewAngle, float aspect, float Near, float Far) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                                   //设置当前矩阵模式为投影矩阵并归一化
	gluPerspective(viewAngle, aspect, Near, Far);        //对投影矩阵进行透视变换
}

void GLCamera::slide(float du, float dv, float dn) {
	//std::cout<<"u.x:"<<u.x()<<std::endl;
	m_pos(0) = m_pos(0) + du*u.x() + dv*v.x() + dn*n.x();
	m_pos(1) = m_pos(1) + du*u.y() + dv*v.y() + dn*n.y();
	m_pos(2) = m_pos(2) + du*u.z() + dv*v.z() + dn*n.z();
	m_target(0) = m_target(0) + du*u.x() + dv*v.x() + dn*n.x();
	m_target(1) = m_target(0) + du*u.y() + dv*v.y() + dn*n.y();
	m_target(2) = m_target(0) + du*u.z() + dv*v.z() + dn*n.z();
	setModelViewMatrix();
}

void GLCamera::roll(float angle) {
	float cs = cos(angle*3.14159265 / 180);
	float sn = sin(angle*3.14159265 / 180);
	Vector3d t(u);
	Vector3d s(v);
	u.x() = cs*t.x() - sn*s.x();
	u.y() = cs*t.y() - sn*s.y();
	u.z() = cs*t.z() - sn*s.z();

	v.x() = sn*t.x() + cs*s.x();
	v.y() = sn*t.y() + cs*s.y();
	v.z() = sn*t.z() + cs*s.z();

	setModelViewMatrix();          //每次计算完坐标轴变化后调用此函数更新视点矩阵
}

void GLCamera::pitch(float angle) {
	float cs = cos(angle*3.14159265 / 180);
	float sn = sin(angle*3.14159265 / 180);
	Vector3d t(v);
	Vector3d s(n);

	v.x() = cs*t.x() - sn*s.x();
	v.y() = cs*t.y() - sn*s.y();
	v.z() = cs*t.z() - sn*s.z();

	n.x() = sn*t.x() + cs*s.x();
	n.y() = sn*t.y() + cs*s.y();
	n.z() = sn*t.z() + cs*s.z();


	setModelViewMatrix();
}

void GLCamera::yaw(float angle) {
	float cs = cos(angle*3.14159265 / 180);
	float sn = sin(angle*3.14159265 / 180);
	Vector3d t(n);
	Vector3d s(u);

	n.x() = cs*t.x() - sn*s.x();
	n.y() = cs*t.y() - sn*s.y();
	n.z() = cs*t.z() - sn*s.z();

	u.x() = sn*t.x() + cs*s.x();
	u.y() = sn*t.y() + cs*s.y();
	u.z() = sn*t.z() + cs*s.z();

	setModelViewMatrix();
}


float  GLCamera::getDist() {
	float dist = pow(m_pos.x(), 2) + pow(m_pos.y(), 2) + pow(m_pos.z(), 2);
	return pow(dist, 0.5);
}


void GLCamera::rotateX(float angle) {
	float d = getDist();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = -2 * d*sin(theta*3.14159265 / 360);
	yaw(theta / 2);
	for (; cnt != 0; --cnt)
	{
		slide(slide_d, 0, 0);
		yaw(theta);
	}
	yaw(-theta / 2);
}

void GLCamera::rotateY(float angle) {
	float d = getDist();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = 2 * d*sin(theta*3.14159265 / 360);
	pitch(theta / 2);
	for (; cnt != 0; --cnt)
	{
		slide(0, slide_d, 0);
		pitch(theta);
	}
	pitch(-theta / 2);
}

void GLCamera::horizontalMove(float forward, float left) {
	n.normalize();
	u.normalize();
	Vector3d norm_n(n);
	Vector3d norm_u(u);
	Vector3d horizontal_n;
	Vector3d horizontal_u;
	horizontal_n.x() = norm_n.x();
	horizontal_n.z() = norm_n.z();
	horizontal_n.y() = 0.0;
	horizontal_u.x() = norm_u.x();
	horizontal_u.z() = norm_u.z();
	horizontal_u.y() = 0.0;

	m_pos = m_pos + horizontal_n * (-forward);
	m_pos = m_pos + horizontal_u * (-left);
}

void GLCamera::verticalMove(float up) {
	Vector3d up_vec;
	up_vec.x() = 0.0;
	up_vec.y() = up;
	up_vec.z() = 0.0;
	m_pos = m_pos + up_vec;
}

Vector3d GLCamera::myPosition() {
	return m_pos;
}

void GLCamera::resetCamera() {
	m_pos = backup[0];
	m_target = backup[1];
	m_up = backup[2];
	n = backup[3];
	u = backup[4];
	v = backup[5];
	setModelViewMatrix();
}