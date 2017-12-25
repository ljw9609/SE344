#include "fluid.h"

Fluid::Fluid(long n, long m, float d, float t, float c, float mu, int tex)
{

	texture = tex;
	width = n; //���  
	height = m;  //�߶�  
	long count = n * m;  //��������  

	buffer[0] = new Vector3d[count];  //������1  
	buffer[1] = new Vector3d[count];  //������2  
	renderBuffer = 0;  //��Ⱦ������  

					   //normal = new Vector3d[count]; //����  
					   //tangent = new Vector3d[count]; //����  


	for (int i = 0; i < 3; i++) {
		indices[i] = new int[2 * (n - 1)*(m - 1)];
	}
	for (int i = 0; i < 6; i++) {
		texcoords[i % 3][i / 3] = new float[2 * (n - 1)*(m - 1)];
	}

	// Precompute constants for Equation (15.25).    
	//Ԥ�ȼ��㣺������淽�̵Ķ���ʽϵ��  
	float f1 = c * c * t * t / (d * d);
	float f2 = 1.0F / (mu * t + 2);
	k1 = (4.0F - 8.0F * f1) * f2;
	k2 = (mu * t - 2) * f2;
	k3 = 2.0F * f1 * f2;

	// Initialize buffers.    
	//��ʼ��������  
	long a = 0;

	for (long j = 0; j < m; j++)
	{
		float y = d * j;
		for (long i = 0; i < n; i++)
		{
			if (i == 0 || j == 0 || i == n - 1 || j == m - 1) {
				buffer[0][a].x() = d * i;
				buffer[0][a].y() = y;
				buffer[0][a].z() = 0.0f;

			}
				
			else {
				int r = rand() % 2;
				if (r == 0) {
					buffer[0][a].x() = d * i;
					buffer[0][a].y() = y;
					buffer[0][a].z() = 1.0f;
				}
				else {
					buffer[0][a].x() = d * i;
					buffer[0][a].y() = y;
					buffer[0][a].z() = 0.0f;
				}
			}
			buffer[1][a] = buffer[0][a];
			//  normal[a].Set(0.0F, 0.0F, 2.0F * d);  
			//  tangent[a].Set(2.0F * d, 0.0F, 0.0F);  
			a++;
		}
	}

	a = 0;
	float w = 1.0f / width;
	float h = 1.0f / height;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < m - 1; j++) {
			long t = i*n + j;
			indices[0][a] = t;
			indices[1][a] = t + 1;
			indices[2][a] = t + n;

			texcoords[0][0][a] = i*w;
			texcoords[0][1][a] = j*h;

			texcoords[1][0][a] = (i + 1)*w;
			texcoords[1][1][a] = j*h;

			texcoords[2][0][a] = i*w;
			texcoords[2][1][a] = (j + 1)*h;

			a++;
		}
	}

	for (int i = 1; i < n; i++) {
		for (int j = 1; j < m; j++) {
			long t = i*n + j;
			indices[0][a] = t;
			indices[1][a] = t - 1;
			indices[2][a] = t - n;

			texcoords[0][0][a] = i*w;
			texcoords[0][1][a] = j*h;

			texcoords[1][0][a] = (i - 1)*w;
			texcoords[1][1][a] = j*h;

			texcoords[2][0][a] = i*w;
			texcoords[2][1][a] = (j - 1)*h;

			a++;
		}
	}
}

Fluid::~Fluid()
{
	//delete[] tangent;  
	//delete[] normal;  
	delete[] buffer[1];
	delete[] buffer[0];
}

void Fluid::Evaluate(void)
{
	// Apply Equation (15.25).    
	//����������淽��  
	for (long j = 1; j < height - 1; j++)
	{
		//��ǰ����λ��  
		const Vector3d *crnt = buffer[renderBuffer] + j * width;
		//ǰһ����λ��  
		Vector3d *prev = buffer[1 - renderBuffer] + j * width;

		// z(i,j,k+1) = k1 * z(i,j,k) + k2 * z(i,j,k-1) +  
		//     k3 * (z(i+1,j,k) + z(i-1,j,k) + z(i,j+1,k) + z(i,j-1,k)  

		for (long i = 1; i < width - 1; i++)
		{
			prev[i].z() = k1 * crnt[i].z() + k2 * prev[i].z() +
				k3 * (crnt[i + 1].z() + crnt[i - 1].z() +
					crnt[i + width].z() + crnt[i - width].z());
		}
	}

	// Swap buffers.  
	//����������  
	renderBuffer = 1 - renderBuffer;

	// Calculate normals and tangents.    
	//���㷨�ߺ�����  
	/*
	for (long j = 1; j < height - 1; j++)
	{
	const Vector3d *next = buffer[renderBuffer] + j * width;
	Vector3d *nrml = normal + j * width;
	Vector3d *tang = tangent + j * width;

	for (long i = 1; i < width - 1; i++)
	{
	nrml[i].x = next[i - 1].z - next[i + 1].z;
	nrml[i].y = next[i - width].z - next[i + width].z;
	tang[i].z = next[i + 1].z - next[i - 1].z;
	}
	}
	*/
}

void Fluid::draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //ѡ������texture[status]       
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < 2 * (height - 1)*(width - 1); i++) {
		/*  glNormal3f(normal[(int)indices[i].x].x,
		normal[(int)indices[i].x].y,
		normal[(int)indices[i].x].z);*/
		glTexCoord2f(texcoords[0][0][i], texcoords[0][1][i]);
		glVertex3f(buffer[renderBuffer][indices[0][i]].x(),
			buffer[renderBuffer][indices[0][i]].y(),
			buffer[renderBuffer][indices[0][i]].z());
		/*  glNormal3f(normal[(int)indices[i].y].x,
		normal[(int)indices[i].y].y,
		normal[(int)indices[i].y].z);*/
		glTexCoord2f(texcoords[1][0][i], texcoords[1][1][i]);
		glVertex3f(buffer[renderBuffer][indices[1][i]].x(),
			buffer[renderBuffer][indices[1][i]].y(),
			buffer[renderBuffer][indices[1][i]].z());
		/*  glNormal3f(normal[(int)indices[i].z].x,
		normal[(int)indices[i].z].y,
		normal[(int)indices[i].z].z);*/
		glTexCoord2f(texcoords[2][0][i], texcoords[2][1][i]);
		glVertex3f(buffer[renderBuffer][indices[2][i]].x(),
			buffer[renderBuffer][indices[2][i]].y(),
			buffer[renderBuffer][indices[2][i]].z());
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}