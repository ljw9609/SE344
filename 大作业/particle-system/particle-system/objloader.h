#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "utility.h"
#include <iostream>
#include <assert.h>
#include <list>
#include <vector>
using namespace std;

typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef void* LPVOID;

/*用于表示几何体顶点 geometric vertex*/
struct Vector3 {
	double x, y, z;
};

/*用于表示贴图顶点 texture vertex*/
struct UV {
	double u, v;
};

/*用于保存BMP位图*/
typedef struct tagBMP {
	LONG bmWidth;
	LONG bmHeight;
	LONG bmWidthBytes;
	WORD bmBitsPixel;
	LPVOID bmBits;
}BMP;

/*OBJ文件纹理*/
class ObjTexture {
private:
	GLuint id;
	int height;
	int width;
	BMP *bmp;

public:
	ObjTexture(const char *filename);
	GLuint getId();
};

/*OBJ文件材质*/
struct ObjMaterial {
	double Ka[3];	//ambient，环境反射
	double Kd[3];	//diffuse，漫反射
	double Ks[3];	//specular，镜反射

	double Tr;		//transparency,滤光投射
	double d;		//渐隐指数
	double Ns;		//反射指数
	double Ni;		//折射值
	int illum;		//光照模型

					/*纹理映射*/
	char map_Kd[256];
	char map_Ka[256];
	char map_Ks[256];
	char map_d[256];
	char map_Ns[256];
	char map_bump[256];	//凹凸纹理

	ObjTexture *t_Kd;
	ObjTexture *t_Ka;
	ObjTexture *t_Ks;
	ObjTexture *t_d;
	ObjTexture *t_Ns;
	ObjTexture *t_bump;

	char name[256];

};

/*用于表示三角形面片*/
struct Triangle
{
	int Vertex[3];	//几何顶点
	int Normal[3];	//法线
	int	UV_Pnt[3];	//纹理顶点
	ObjMaterial *mat;	//材质
};

typedef struct
{
	char *name;
	int p;
}GroupBound, ObjectBound;

/*OBJ读取*/
class ObjLoader {
private:
	int nVertex = 0;
	int nNormal = 0;
	int nVertUV = 0;
	int *groups;
	int *objects;
	int groupCount = 0, objectCount = 0;

	list<Vector3>		vertexList;
	list<Vector3>		normalList;
	list<UV>			uvList;
	list<Triangle>		triangleList;
	list<ObjectBound>	objectBounds;
	list<GroupBound>	groupBounds;
	list<ObjMaterial*>	matList;
	Vector3*			vertexArray;
	Vector3*			normalArray;
	UV*					uvArray;
	ObjMaterial*		currentMat;
	ObjMaterial*		mat = NULL;		

	void parse_obj_file(const char *str);
	ObjMaterial* find_material(const char *name) const;
	void read_mtl(const char *filename);
	void parse_mtl_file(const char *str);

public:
	ObjLoader();
	~ObjLoader();
	ObjLoader(const char *filename);
	void init_obj();
	void load_obj(const char *filename);
	void render_obj();
};


#endif