#include "camera.h"
#define Pi 3.1415926535

Camera::Camera() {}

Camera::~Camera() {}

void Camera::initCamera(const Vector3d &pos, const Vector3d &target, const Vector3d &up) {
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

void Camera::setModelViewMatrix() {
	double m[16];
	m[0] = u.x(); m[4] = u.y(); m[8]  = u.z(); m[12] = -m_pos.dot(u);
	m[1] = v.x(); m[5] = v.y(); m[9]  = v.z(); m[13] = -m_pos.dot(v);
	m[2] = n.x(); m[6] = n.y(); m[10] = n.z(); m[14] = -m_pos.dot(n);
	m[3] = 0;	  m[7] = 0;		m[11] = 0;     m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m);    
}

void  Camera::setShape(float viewAngle, float aspect, float Near, float Far) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();									 //���õ�ǰ����ģʽΪͶӰ���󲢹�һ��
	gluPerspective(viewAngle, aspect, Near, Far);        //��ͶӰ�������͸�ӱ任
}

/*
void Camera::slide(float du, float dv, float dn) {
	m_pos(0) = m_pos(0) + du*u.x() + dv*v.x() + dn*n.x();
	m_pos(1) = m_pos(1) + du*u.y() + dv*v.y() + dn*n.y();
	m_pos(2) = m_pos(2) + du*u.z() + dv*v.z() + dn*n.z();
	m_target(0) = m_target(0) + du*u.x() + dv*v.x() + dn*n.x();
	m_target(1) = m_target(0) + du*u.y() + dv*v.y() + dn*n.y();
	m_target(2) = m_target(0) + du*u.z() + dv*v.z() + dn*n.z();
	setModelViewMatrix();
}
*/

void Camera::roll(float angle) {
	float cs = cos(angle * Pi / 180);
	float sn = sin(angle * Pi / 180);
	Vector3d t(u);
	Vector3d s(v);
	u.x() = cs*t.x() - sn*s.x();
	u.y() = cs*t.y() - sn*s.y();
	u.z() = cs*t.z() - sn*s.z();

	v.x() = sn*t.x() + cs*s.x();
	v.y() = sn*t.y() + cs*s.y();
	v.z() = sn*t.z() + cs*s.z();

	setModelViewMatrix();          
}

void Camera::pitch(float angle) {
	float cs = cos(angle * Pi / 180);
	float sn = sin(angle * Pi / 180);
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

void Camera::yaw(float angle) {
	float cs = cos(angle * Pi / 180);
	float sn = sin(angle * Pi / 180);
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

float  Camera::getDist() {
	float dist = pow(m_pos.x(), 2) + pow(m_pos.y(), 2) + pow(m_pos.z(), 2);
	return pow(dist, 0.5);
}

void Camera::rotateX(float angle) {
	/*
	float d = getDist();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = -2 * d*sin(theta*Pi / 360);
	yaw(theta / 2);
	for (; cnt != 0; --cnt)
	{
		slide(slide_d, 0, 0);
		yaw(theta);
	}
	yaw(-theta / 2);
	*/
	float part_theta = 30;
	float theta = angle * part_theta * Pi / 180;
	yaw(theta);
}

void Camera::rotateY(float angle) {
	/*
	float d = getDist();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = 2 * d*sin(theta*Pi / 360);
	pitch(theta / 2);
	for (; cnt != 0; --cnt)
	{
		slide(0, slide_d, 0);
		pitch(theta);
	}
	pitch(-theta / 2);
	*/
	float part_theta = 30;
	float theta = angle * part_theta * Pi / 180;
	pitch(theta);
}

void Camera::rotateRoll(float angle) {
	float part_theta = 30;
	float theta = angle * part_theta * Pi / 180;
	roll(theta);
}

void Camera::horizontalMove(float forward, float left) {
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

void Camera::verticalMove(float up) {
	Vector3d up_vec;
	up_vec.x() = 0.0;
	up_vec.y() = up;
	up_vec.z() = 0.0;
	m_pos = m_pos + up_vec;
}

Vector3d Camera::myPosition() {
	return m_pos;
}

void Camera::resetCamera() {
	m_pos = backup[0];
	m_target = backup[1];
	m_up = backup[2];
	n = backup[3];
	u = backup[4];
	v = backup[5];
	setModelViewMatrix();
}