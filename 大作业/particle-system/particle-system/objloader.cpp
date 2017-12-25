#include "objloader.h"
//#define DEBUG 1

/*load BMP*/
bool LoadBmp(BMP& bmp, const char* path)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	FILE *fp;
	errno_t rc = fopen_s(&fp, path, "rb");
	if (fp == NULL)
		return false;

	//read file
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	//init
	bmp.bmWidth = infoHeader.biWidth;
	bmp.bmHeight = infoHeader.biHeight;
	size_t lineSize = (infoHeader.biWidth * infoHeader.biBitCount + 31) / 8;
	bmp.bmWidthBytes = lineSize;
	bmp.bmBitsPixel = infoHeader.biBitCount;
	//pixels
	size_t size = lineSize * infoHeader.biHeight;
	bmp.bmBits = malloc(size);
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);
	fread(bmp.bmBits, size, 1, fp);

	fclose(fp);
	return true;
}

/*ObjTexture*/
ObjTexture::ObjTexture(const char *filename) {
	bmp = new BMP();
	if (!LoadBmp(*bmp, filename)){
		cout<<"bmp load failed"<<endl;
	}
	height = bmp->bmHeight;
	width = bmp->bmWidth;

	GLuint formerUsedTex;
	glGetIntegerv(GL_TEXTURE_2D, (GLint*)&formerUsedTex);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glGenTextures(1, &id);
#ifdef DEBUG
	printf("texture id:%u using image %s\n", id, filename);
#endif
	glBindTexture(GL_TEXTURE_2D, id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->bmWidth, bmp->bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp->bmBits);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bmp->bmWidth, bmp->bmHeight, GL_BGR, GL_UNSIGNED_BYTE, bmp->bmBits);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, formerUsedTex);
	glPopAttrib();

#ifdef DEBUG
	printf("BMP width:%d, height:%d\n", bmp->bmWidth, bmp->bmHeight);
#endif

	delete bmp;
}

GLuint ObjTexture::getId() {
	return id;
}


/*ObjLoader*/
ObjLoader::ObjLoader() {};

ObjLoader::~ObjLoader() {};

ObjLoader::ObjLoader(const char *filename) {
	init_obj();
	/*open obj file*/
	FILE *fp;
	errno_t rc = fopen_s(&fp, filename, "rb");
	if (fp == NULL) {
		cout << "load obj file error!" << endl;
		exit(1);
	}

	/*parse obj file*/
	char buf[1024];
	while (fgets(buf, 1024, fp)) {
		parse_obj_file(buf);
	}
	fclose(fp);


#ifdef DEBUG
	printf("nVertex : %d\nnNormal : %d\nnUV : %d\n", nVertex, nNormal, nVertUV);
#endif

	vertexArray = new Vector3[nVertex];
	normalArray = new Vector3[nNormal];
	uvArray = new UV[nVertUV];
	groups = new int[groupCount + 1];
	objects = new int[objectCount + 1];

	int i = 0;
	for (std::list<Vector3>::const_iterator it = vertexList.begin(); it != vertexList.end(); it++)
	{
		vertexArray[i++] = *it;
	}

	i = 0;
	for (std::list<Vector3>::const_iterator it = normalList.begin(); it != normalList.end(); it++)
		normalArray[i++] = *it;

	i = 0;
	for (std::list<UV>::const_iterator it = uvList.begin(); it != uvList.end(); it++)
		uvArray[i++] = *it;

	i = 0;
	for (std::list<GroupBound>::const_iterator it = groupBounds.begin(); it != groupBounds.end(); it++)
		groups[i++] = (*it).p;

	i = 0;
	for (std::list<ObjectBound>::const_iterator it = objectBounds.begin(); it != objectBounds.end(); it++)
		objects[i++] = (*it).p;
}

void ObjLoader::init_obj() {
	currentMat = NULL;
	/*create 0 bound*/
	ObjectBound e;
	e.name = NULL;
	e.p = 0;
	objectBounds.push_back(e);
	groupBounds.push_back(e);
}

void ObjLoader::load_obj(const char *filename) {
	init_obj();
	/*open obj file*/
	FILE *fp;
	errno_t rc = fopen_s(&fp, filename, "rb");
	if (fp == NULL) {
		cout << "load obj file error!" << endl;
		exit(1);
	}

	/*parse obj file*/
	char buf[1024];
	while (fgets(buf, 1024, fp)) {
		parse_obj_file(buf);
	}
	fclose(fp);


#ifdef DEBUG
	printf("nVertex : %d\nnNormal : %d\nnUV : %d\n", nVertex, nNormal, nVertUV);
#endif

	vertexArray = new Vector3[nVertex];
	normalArray = new Vector3[nNormal];
	uvArray = new UV[nVertUV];
	groups = new int[groupCount + 1];
	objects = new int[objectCount + 1];

	int i = 0;
	for (std::list<Vector3>::const_iterator it = vertexList.begin(); it != vertexList.end(); it++)
	{
		vertexArray[i++] = *it;
	}

	i = 0;
	for (std::list<Vector3>::const_iterator it = normalList.begin(); it != normalList.end(); it++)
		normalArray[i++] = *it;

	i = 0;
	for (std::list<UV>::const_iterator it = uvList.begin(); it != uvList.end(); it++)
		uvArray[i++] = *it;

	i = 0;
	for (std::list<GroupBound>::const_iterator it = groupBounds.begin(); it != groupBounds.end(); it++)
		groups[i++] = (*it).p;

	i = 0;
	for (std::list<ObjectBound>::const_iterator it = objectBounds.begin(); it != objectBounds.end(); it++)
		objects[i++] = (*it).p;
}

void ObjLoader::parse_obj_file(const char *str) {
	/*prepare command*/
	char cmd[256];
	int i;
	for (i = 0; str[i] && str[i] != ' ' && i < 10; i++)
		cmd[i] = str[i];
	if (str[i] && i<8)
		cmd[i] = 0;
	else 
		return;


#ifdef DEBUG
	printf("Reading %s\n", str);
#endif
	/*几何体顶点 Geometric vertices*/
	if (strcmp(cmd, "v") == 0){
		Vector3 v;
		sscanf_s(str + i + 1, "%lf %lf %lf", &v.x, &v.y, &v.z);
		vertexList.push_back(v);
		nVertex++;
	}
	/*顶点法线 Vertex normals*/
	else if (strcmp(cmd, "vn") == 0){
		Vector3 vn;
		sscanf_s(str + i + 1, "%lf %lf %lf", &vn.x, &vn.y, &vn.z);
		normalList.push_back(vn);
		nNormal++;
	}
	/*贴图坐标点 Texture vertices*/
	else if (strcmp(cmd, "vt") == 0){
		UV vt;
		sscanf_s(str + i + 1, "%lf %lf", &vt.u, &vt.v);
		uvList.push_back(vt);
		nVertUV++;
	}
	/*面 Face*/
	else if (strcmp(cmd, "f") == 0){
		int token[3], i;
		for (i = 0; str[i] != ' '; i++);
			token[0] = i + 1;
		for (i += 1; str[i] != ' '; i++);
			token[1] = i + 1;
		for (i += 1; str[i] != ' '; i++);
			token[2] = i + 1;

		Triangle t;
		for (int j = 0; j < 3; j++){
			t.UV_Pnt[j] = t.Normal[j] = -1;
			sscanf_s(str + token[j], "%d/%d/%d", &t.Vertex[j], &t.UV_Pnt[j], &t.Normal[j]);
		}

		t.mat = currentMat;
#ifdef DEBUG
		printf("Face <%d-%d-%d>\n", t.Vertex[0], t.Vertex[1], t.Vertex[2]);
		if (t.mat != NULL)
			printf("Material : %s\n", t.mat->name);
#endif
		triangleList.push_back(t);
	}
	/*对象名称 Object*/
	else if (strcmp(cmd, "o") == 0){
		ObjectBound ob;
		ob.name = new char[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r'); i++)
			ob.name[j++] = str[i];
		ob.name[j] = 0;
#ifdef DEBUG
		printf("new object : %s\n", ob.name);
#endif
		ob.p = nVertex;
		objectBounds.push_back(ob);
	}
	/*组名称 Group*/
	else if (strcmp(cmd, "g") == 0){
		GroupBound gb;
		gb.name = new char[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r'); i++)
			gb.name[j++] = str[i];
		gb.name[j] = 0;

		gb.p = nVertex;
		groupBounds.push_back(gb);
	}
	/*材质名称 Material name*/
	else if (strcmp(cmd, "usemtl") == 0){
		char matNameStr[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r'); i++)
			matNameStr[j++] = str[i];
		matNameStr[j] = 0;
		currentMat = find_material(matNameStr);
#ifdef DEBUG
		if (currentMat != NULL) 
			printf("switch to material %s\n", currentMat->name);
#endif
	}
	/*材质库 Materail library*/
	else if (strcmp(cmd, "mtllib") == 0){
		char buf[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r'); i++)
			buf[j++] = str[i];
		buf[j] = 0;
#ifdef DEBUG
		printf("mtllib %s\n", buf);
#endif
		read_mtl(buf);
	}
	/*评论行*/
	else
	{
		
	}
}

ObjMaterial* ObjLoader::find_material(const char *name) const{
	ObjMaterial *mat;
	list<ObjMaterial*>::const_iterator it;
	for (it = matList.begin(); it != matList.end(); it++){
		if (strcmp((*it)->name, name) == 0){
			mat = *it;
			return mat;
		}
	}
	return NULL;
}

void ObjLoader::read_mtl(const char *filename) {
	mat = NULL;
	FILE *fp;
	errno_t rc = fopen_s(&fp, filename, "rb");

	if (fp == NULL) {
		cout << "load mtl file error!" << endl;
		exit(1);
	}

	char buf[1024];
	while (fgets(buf, 1024, fp)){
		parse_mtl_file(buf);
	}

	if (mat != NULL){
#ifdef DEBUG
		printf("new material added %s\n", mat->name);
#endif
		matList.push_back(mat);
	}
	mat = NULL;

	fclose(fp);
}

void ObjLoader::parse_mtl_file(const char *str) {
	char cmd[256];
	int i;
	for (i = 0; str[i] && str[i] != ' ' && i < 10; i++)
		cmd[i] = str[i];
	if (str[i] && i<8)
		cmd[i] = 0;
	else 
		return;

	/*新材质*/
	if (strcmp(cmd, "newmtl") == 0){
		if (mat != NULL){
#ifdef DEBUG
			printf("new material added %s\n", mat->name);
#endif
			matList.push_back(mat);
		}
		mat = new ObjMaterial();

		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r') && str[i] != ' '; i++)
			mat->name[j++] = str[i];
		mat->name[j] = 0;
		mat->Tr = 0;
		mat->t_bump = mat->t_d = mat->t_Ka = mat->t_Kd = mat->t_Ks = mat->t_Ns = NULL;
	}
	/*反射指数*/
	else if (strcmp(cmd, "Ns") == 0){
		sscanf_s(str + i + 1, "%lf", &mat->Ns);
	}
	/*环境反射*/
	else if (strcmp(cmd, "Ka") == 0){
		sscanf_s(str + i + 1, "%lf%lf%lf", &mat->Ka[0], &mat->Ka[1], &mat->Ka[2]);
	}
	/*漫反射*/
	else if (strcmp(cmd, "Kd") == 0){
		sscanf_s(str + i + 1, "%lf%lf%lf", &mat->Kd[0], &mat->Kd[1], &mat->Kd[2]);
	}
	/*镜反射*/
	else if (strcmp(cmd, "Ks") == 0){
		sscanf_s(str + i + 1, "%lf%lf%lf", &mat->Ks[0], &mat->Ks[1], &mat->Ks[2]);
	}
	/*渐隐指数*/
	else if (strcmp(cmd, "d") == 0){
		sscanf_s(str + i + 1, "%lf", &mat->d);
	}
	/*光照模型*/
	else if (strcmp(cmd, "illum") == 0){
		sscanf_s(str + i + 1, "%d", &mat->illum);
	}
	/*环境反射指定纹理*/
	else if (strcmp(cmd, "map_Ka") == 0) {
		char buf[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r') && str[i] != ' '; i++)
			buf[j++] = str[i];
		buf[j] = 0;

		mat->t_Ka = new ObjTexture(buf);
	}
	/*漫反射指定纹理*/
	else if (strcmp(cmd, "map_Kd") == 0){
		char buf[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r') && str[i] != ' '; i++)
			buf[j++] = str[i];
		buf[j] = 0;

		mat->t_Kd = new ObjTexture(buf);
	}
	/*镜反射指定纹理*/
	else if (strcmp(cmd, "map_Ks") == 0) {
		char buf[256];
		int j = 0;
		for (i += 1; str[i] && (str[i] != '\n' && str[i] != '\r') && str[i] != ' '; i++)
			buf[j++] = str[i];
		buf[j] = 0;

		mat->t_Ks = new ObjTexture(buf);
	}
}

void ObjLoader::render_obj(){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	for (list<Triangle>::const_iterator it = triangleList.begin(); it != triangleList.end(); it++){
		if (it->mat != NULL){
			float Ka[4] = { it->mat->Ka[0], it->mat->Ka[1], it->mat->Ka[2], it->mat->Tr };
			float Ks[4] = { it->mat->Ks[0], it->mat->Ks[1], it->mat->Ks[2], it->mat->Tr };
			float Kd[4] = { it->mat->Kd[0], it->mat->Kd[1], it->mat->Kd[2], it->mat->Tr };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
			if (it->mat->t_Kd != NULL){
				glBindTexture(GL_TEXTURE_2D, it->mat->t_Kd->getId());
			}
		}
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 3; i++){
			if (it->mat != NULL && it->mat->t_Kd != NULL){
				glTexCoord2d(uvArray[(*it).UV_Pnt[i] - 1].u, uvArray[(*it).UV_Pnt[i] - 1].v);
			}
			glVertex3d(vertexArray[(*it).Vertex[i] - 1].x, vertexArray[(*it).Vertex[i] - 1].y, vertexArray[(*it).Vertex[i] - 1].z);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
}
