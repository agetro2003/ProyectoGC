#include "Mesh.h"

#include <iostream>
#include <numbers>

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
{
}

Mesh::~Mesh()
{
	unload();
}

void
Mesh::draw() const
{
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::load() 
{
	assert(mVBO == NONE); // not already loaded

	if (vVertices.size() > 0) { // transfer data
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vVertices.size() * sizeof(vec3), vVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);

		if (vColors.size() > 0) {             // upload colors
			glGenBuffers(1, &mCBO);

			glBindBuffer(GL_ARRAY_BUFFER, mCBO);
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}
		if (vTexCoords.size() > 0) {
			glGenBuffers(1, &mTCO);
			glBindBuffer(GL_ARRAY_BUFFER, mTCO);
			glBufferData(GL_ARRAY_BUFFER, vTexCoords.size() * sizeof(vec2), vTexCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
			glEnableVertexAttribArray(2);
		}
		if (vNormals.size() > 0) {
			glGenBuffers(1, &mNBO);
			glBindBuffer(GL_ARRAY_BUFFER, mNBO);
			glBufferData(GL_ARRAY_BUFFER, vNormals.size() * sizeof(vec3), vNormals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
			glEnableVertexAttribArray(3);
		}
	}

}

void 
Mesh::unload() 
{
	if (mVAO != NONE) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		mVAO = NONE;
		mVBO = NONE;

		if (mCBO != NONE) {
			glDeleteBuffers(1, &mCBO);
			mCBO = NONE;
		}
		if (mTCO != NONE) {
			glDeleteBuffers(1, &mTCO);
			mTCO = NONE;
		}
		if (mNBO != NONE) {
			glDeleteBuffers(1, &mNBO);
			mNBO = NONE;
		}
	}
}

void
Mesh::render() const
{	
	assert(mVAO != NONE);

	glBindVertexArray(mVAO);
	draw();
}



//Ap54

IndexMesh::IndexMesh()
	: Mesh()
	, mIBO(NONE)
{

}


IndexMesh::~IndexMesh()
{
	unload();
}

void 
IndexMesh::load()  
{
	Mesh::load();
	glBindVertexArray (mVAO);
	glGenBuffers (1, &mIBO);
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , mIBO);
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER ,
		vIndexes.size() * sizeof(GLuint),
		vIndexes.data(), GL_STATIC_DRAW
		);

	glBindVertexArray (0);
}

void
IndexMesh::unload()   {
	Mesh::unload();
	if (mIBO != NONE) {
			glDeleteBuffers(1, &mIBO);
			mIBO = NONE;
		}
}

void 
IndexMesh::draw() const
{
	glDrawElements (
		mPrimitive, // primitiva ( GL_TRIANGLES , etc.)
		vIndexes.size(), //? número de índices
		GL_UNSIGNED_INT , // tipo de los índices
		nullptr //? offset en el VBO de índices
	);
}




Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
// Create a regular polygon with num vertices and radius r
Mesh *	
Mesh::generateRegularPolygon(GLuint num, GLdouble r)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = num;
	mesh->vVertices.reserve(mesh->mNumVertices);
	float alpha = 90;
	float step = 360 / num;
	for (int i = 0; i < num; i++) {
		double x = r * cos(radians(alpha));
		double y = r * sin(radians(alpha));
		mesh->vVertices.emplace_back(x, y, 0.0);
		alpha += step;
	}


	return mesh;

}

// Create a RGB triangle with radius r
Mesh*
Mesh::generateRGBTriangle(GLdouble r)
{
	Mesh* mesh = generateRegularPolygon(3, r);
	mesh->mPrimitive = GL_TRIANGLES;	
	mesh->vColors.reserve(mesh->mNumVertices);

	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0); // red
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0); // green
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0); // blue

	return mesh;
}

// Create a rectangle with width w and height h
Mesh*
Mesh::generateRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(w * 0.5, h * 0.5, 0.0);
	mesh->vVertices.emplace_back(-w * 0.5, h * 0.5, 0.0);
	mesh->vVertices.emplace_back(w * 0.5, -h * 0.5, 0.0);
	mesh->vVertices.emplace_back(-w * 0.5, -h * 0.5, 0.0);
	
	return mesh;
}

// Create a RGB rectangle with width w, height h, one vertex red, one blue and two green
Mesh*
Mesh::generateRGBRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = generateRectangle(w, h);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0); 

	return mesh;
}

//P1-Apartado 15 -> Cubo gnerado con 14 vértices y GL_TRIANGLE_STRIP como primitiva
Mesh*
Mesh::generateCube(GLdouble length) 
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 14;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble l = length * 0.5;

	//Cara z+ (0-1-2 + 1-2-3)
	mesh->vVertices.emplace_back(-l, l, l);	//0
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(l, -l, l);	//3

	//Cara y- (2-3-4)
	//Cara x+ (3-4-1 + 4-1-5)
	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(l, l, -l);	//5

	//Cara y+ (1-5-0 + 5-0-6)
	mesh->vVertices.emplace_back(-l, l, l);	//0
	mesh->vVertices.emplace_back(-l, l, -l);//6
	
	//Cara x- (0-6-2 + 6-2-7)
	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(-l, -l, -l);//7

	//Cara y- (2-7-4)
	//Cara z- (7-4-6 + 4-6-5)
	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(-l, l, -l);//6
	mesh->vVertices.emplace_back(l, l, -l);	//5

	return mesh;
}


//P1-Apartado 16 -> Cubo con 36 vértices y GL_TRIANGLES como primitiva para poder asignar colores a vértices (y no queden caras de colores mezclados)
Mesh*
Mesh::generateCube2(GLdouble length)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = 36;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble l = length * 0.5;

	//Cara z+ (0-1-2 + 1-2-3)
	mesh->vVertices.emplace_back(-l, l, l);	//0
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(-l, -l, l);//2

	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(l, -l, l);	//3

	//Cara y- (2-3-4 + 2-7-4)
	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(l, -l, l);	//3
	mesh->vVertices.emplace_back(l, -l, -l);//4

	mesh->vVertices.emplace_back(-l, -l, -l);//7
	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(l, -l, -l);//4
	
	//Cara x+ (3-4-1 + 4-1-5)
	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(l, -l, l);	//3
	mesh->vVertices.emplace_back(l, l, l);	//1

	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(l, l, -l);	//5
	
	//Cara y+ (1-5-0 + 5-0-6)
	mesh->vVertices.emplace_back(l, l, -l);	//5
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(-l, l, l);	//0

	mesh->vVertices.emplace_back(l, l, -l);	//5
	mesh->vVertices.emplace_back(-l, l, l);	//0
	mesh->vVertices.emplace_back(-l, l, -l);//6
	
	//Cara x- (0-6-2 + 6-2-7)
	mesh->vVertices.emplace_back(-l, l, -l);//6
	mesh->vVertices.emplace_back(-l, l, l);	//0
	mesh->vVertices.emplace_back(-l, -l, l);//2
	
	mesh->vVertices.emplace_back(-l, l, -l);//6
	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(-l, -l, -l);//7
	
	//Cara z- (7-4-6 + 4-6-5)
	mesh->vVertices.emplace_back(-l, -l, -l);//7
	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(-l, l, -l);//6

	mesh->vVertices.emplace_back(-l, l, -l);//6
	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(l, l, -l);	//5
	
	return mesh;
}

Mesh*
Mesh::generateRGBCubeTriangles(GLdouble length) {

	Mesh* mesh = generateCube2(length);
	mesh->vColors.reserve(mesh->mNumVertices);

	//Cara z+ (0-1-2 + 1-2-3) (roja)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//0
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//1
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//2

	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//2
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//1
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//3

	
	//Cara y- (2-3-4 + 2-7-4) (azul)
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//2
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//3
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//4

	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//7
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//4
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//2
	
	//Cara x+ (3-4-1 + 4-1-5) (verde)
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//4
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//3
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//1

	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//4
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//1
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//5

	//Cara y+ (1-5-0 + 5-0-6) (azul)
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//5
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//1
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//0

	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//5
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//0
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);//6

	//Cara x- (0-6-2 + 6-2-7) (verde)
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//6
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//0
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//2

	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//6
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//2
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);//7

	//Cara z- (7-4-6 + 4-6-5) (roja)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//7
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//4
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//6

	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//6
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//4
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);//5
	
	return mesh;

}

//Rectángulo sin texturas, Ap18
Mesh *
Mesh::generateGround(GLdouble lenght) {
	Mesh* mesh = generateRectangle(lenght, lenght);
	return mesh;
}

//Rectángulo con texturas, Ap20
Mesh*
Mesh::generatePhotoTexCor(GLdouble w, GLdouble h) {
	Mesh* mesh = generateRectangle(w, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	// Agregar texturas a los vértices creado en generateRectangle
	mesh->vTexCoords.emplace_back(1, 0);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(0, 1);


	return mesh;
}

//Rectángulo con texturas, Ap20
Mesh*
Mesh::generateRectangleTexCor(GLdouble w, GLdouble h) {
	Mesh* mesh = generateRectangle(w, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	// Agregar texturas a los vértices creado en generateRectangle
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 0);


	return mesh;
}

//Rectángulo con texturas que se repiten, Ap21
Mesh*
Mesh::generaRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	Mesh* mesh = generateRectangle(w, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	// Agregar texturas a los vértices creado en generateRectangle
	mesh->vTexCoords.emplace_back(rw, rh);
	mesh->vTexCoords.emplace_back(0, rh);
	mesh->vTexCoords.emplace_back(rw, 0);
	mesh->vTexCoords.emplace_back(0, 0);


	return mesh;
}

//cubo sin tapas sin textura Ap23
Mesh* 
Mesh::generateBoxOutline(GLdouble width, GLdouble height) {

	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble l = width * 0.5;
	GLdouble h = height * 0.5;
	//Cara x+ (3-1-4 + 1-4-5)
	mesh->vVertices.emplace_back(l, -h, l);	//3
	mesh->vVertices.emplace_back(l, h, l);	//1
	mesh->vVertices.emplace_back(l, -h, -l);//4
	mesh->vVertices.emplace_back(l, h, -l);	//5

	//Cara z- (4-5-7 + 5-7-6)
	mesh->vVertices.emplace_back(-l, -h, -l);//7
	mesh->vVertices.emplace_back(-l, h, -l);//6

	//Cara x- (7-6-2 + 6-2-0)
	mesh->vVertices.emplace_back(-l, -h, l);//2
	mesh->vVertices.emplace_back(-l, h, l);	//0

	//Cara z+ (2-0-3 + 0-3-1)
	mesh->vVertices.emplace_back(l, -h, l);	//3
	mesh->vVertices.emplace_back(l, h, l);	//1
	

	return mesh;
}

//cubo sin tapas sin textura Ap23 original (deprecated, siempre es un cubo)
Mesh*
Mesh::generateBoxOutline2(GLdouble length) {

	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble l = length * 0.5;

	//Cara x+ (3-1-4 + 1-4-5)
	mesh->vVertices.emplace_back(l, -l, l);	//3
	mesh->vVertices.emplace_back(l, l, l);	//1
	mesh->vVertices.emplace_back(l, -l, -l);//4
	mesh->vVertices.emplace_back(l, l, -l);	//5

	//Cara z- (4-5-7 + 5-7-6)
	mesh->vVertices.emplace_back(-l, -l, -l);//7
	mesh->vVertices.emplace_back(-l, l, -l);//6

	//Cara x- (7-6-2 + 6-2-0)
	mesh->vVertices.emplace_back(-l, -l, l);//2
	mesh->vVertices.emplace_back(-l, l, l);	//0

	//Cara z+ (2-0-3 + 0-3-1)
	mesh->vVertices.emplace_back(l, -l, l);	//3
	mesh->vVertices.emplace_back(l, l, l);	//1


	return mesh;
}

//Cubo sin tapa con textura Ap24,25
Mesh*
Mesh::generateBoxOutlineTexCor(GLdouble width, GLdouble height) {
	Mesh* mesh = generateBoxOutline(width, height);
	mesh->vTexCoords.reserve(mesh->mNumVertices);
	//Cara x+ (3-1-4 + 1-4-5)
	mesh->vTexCoords.emplace_back(0, 1);	//3
	mesh->vTexCoords.emplace_back(0, 0);	//1

	mesh->vTexCoords.emplace_back(1, 1);	//4
	mesh->vTexCoords.emplace_back(1, 0);	//5
	//Cara z- (4-5-7 + 5-7-6)
	mesh->vTexCoords.emplace_back(2, 1);	//7
	mesh->vTexCoords.emplace_back(2, 0);	//6
	//Cara x- (7-6-2 + 6-2-0)
	mesh->vTexCoords.emplace_back(3, 1);	//2
	mesh->vTexCoords.emplace_back(3, 0);	//0
	//Cara z+ (2-0-3 + 0-3-1)
	mesh->vTexCoords.emplace_back(4, 1);	//3
	mesh->vTexCoords.emplace_back(4, 0);	//1

	return mesh;
}

//Estrella apartado 26
Mesh* 
Mesh::generateStar3D(GLdouble re, GLuint np, GLdouble h) {

	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = np*2 +2;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble ri = re / 2;

	GLfloat grados = 360 / np;
	GLfloat grados_ext = 0;
	GLfloat grados_int = grados / 2;

	mesh->vVertices.emplace_back(0, 0, 0); //Vértice central de la estrella

	//Ecuación del radio del círculo -> re = sqrt(x*x + y*y)
	//Coordenadas x e y de un punto en su radio:
	//	cos(grados_ext)=x/re -> x=cos(grados_ext) *re
	//	sin(grados_ext)=y/re -> y=sin(grados_ext) *re

	//Para generar la estrella, se pone un vértice exterior y uno interior en sucesión tantas veces como puntas tenga la estrella
	//Cada uno se une al vértice central y al que sea el anterior para ir formando la estrella
	for (int i = 0; i < np; i++) {
		mesh->vVertices.emplace_back(cos(glm::radians(grados_ext)) * re, sin(glm::radians(grados_ext))* re, h);		//Vértice exterior
		mesh->vVertices.emplace_back(cos(glm::radians(grados_int)) * ri, sin(glm::radians(grados_int)) * ri, h);	//Vértice interior
		grados_ext += grados;
		grados_int += grados;
	}
	mesh->vVertices.emplace_back(cos(glm::radians(grados_ext)) * re, sin(glm::radians(grados_ext)) * re, h);		//Vértice exterior final para cerrar la figura
	return mesh;
}

//Cada lado de la textura de la estrella se divide en 4 y se van recorriendo lados (en orden superior, derecho, inferior e izquierdo)
//y asignando puntos consecutivos a un vértce exterior seguido de un vértice interior (seguido de un vértice exterior seguido de un vértice interior, etc.)
Mesh* 
Mesh::generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = Mesh::generateStar3D(re, np, h);
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	mesh->vTexCoords.emplace_back(0.5, 0.5); //Se asigna el centro de la textura al vértice central

	double x = 0, y = 0;

	for (int i = 0; i < np; i++) {
		mesh->vTexCoords.emplace_back(x,y); //Vértice exterior
		if (x == 0 and y < 1) {			//Lado superior de la imagen de la textura
			y += 0.25;
		}
		else if (y == 1 and x < 1) {	//Lado derecho de la imagen de la textura
			x += 0.25;
		}
		else if (x == 1 and y > 0) {	//Lado inferior de la imagen de la textura
			y -= 0.25;
		}
		else if (y == 0 and x > 0) {	//Lado izquierdo de la imagen de la textura
			x -= 0.25;
		}
		mesh->vTexCoords.emplace_back(x,y); //Vértice interior
		if (x == 0 and y < 1) {			//Lado superior de la imagen de la textura
			y += 0.25;
		}
		else if (y == 1 and x < 1) {	//Lado derecho de la imagen de la textura
			x += 0.25;
		}
		else if (x == 1 and y > 0) {	//Lado inferior de la imagen de la textura
			y -= 0.25;
		}
		else if (y == 0 and x > 0) {	//Lado izquierdo de la imagen de la textura
			x -= 0.25;
		}
		
	}
	mesh->vTexCoords.emplace_back(0, 0); //Vértice exteriror que cierra la figura

	return mesh;

}

//Caja translúcida Ap32
Mesh* 
Mesh::generateGlassParapet(GLdouble width, GLdouble height) {
	Mesh* mesh = Mesh::generateBoxOutlineTexCor( width,  height);
	return mesh;

}



//Ap55
IndexMesh*
IndexMesh::generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax) {

	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	int tamPerfil = profile.size();
	mesh->vVertices.reserve(nSamples * tamPerfil);

	// Genera los vértices de las muestras
	GLdouble theta1 = angleMax / nSamples;


	for (int i = 0; i <= nSamples; ++i) { // muestra i-ésima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
	}

	for (int i = 0; i < nSamples; ++i) // caras i a i + 1
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara

			if (profile[j].x != 0.0) // triángulo inferior
				for (auto [s, t] : { pair{i, j}, {i, j + 1}, {i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);

			if (profile[j + 1].x != 0.0) // triángulo superior
				for (auto [s, t] : { pair{i, j + 1}, {i + 1, j + 1}, {i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
		}


	mesh->mNumVertices = mesh->vVertices.size();
	mesh->buildNormalVectors();

	return mesh;

}

IndexMesh*
IndexMesh::generateIndexedBox(GLdouble l) {
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;
	GLdouble half = l * 0.5;
	// indices que definen la caja
	std::vector<GLuint> indices = {
		0, 1, 2, 2, 1, 3, 
		2, 3, 4, 4, 3, 5,
		4, 5, 6, 6, 5, 7,
		6, 7, 0, 0, 7, 1,
		4, 6, 2, 2, 6, 0,
		1, 7, 3, 3, 7, 5
	};

	mesh->vIndexes = indices;


	// vertices de la caja
	std::vector<vec3> vertices = {
		{half, half, -half}, //0
		{half, -half, -half }, //1 
		{half, half, half }, //2
		{half, -half, half }, //3
		{-half, half, half }, //4
		{-half, -half, half }, //5
		{-half, half, -half }, //6
		{-half, -half, -half }  //7
	};
	mesh->vVertices = vertices;


	// calculo de las normales
	mesh->buildNormalVectors();

	mesh->mNumVertices = mesh->vVertices.size();
	// imprimir las normales
	for (size_t i = 0; i < mesh->vNormals.size(); ++i) {
		std::cout << "Normal " << i << ": " << mesh->vNormals[i].x << ", " << mesh->vNormals[i].y << ", " << mesh->vNormals[i].z << std::endl;
	}
	return mesh;
}


//Ap 66
IndexMesh*
IndexMesh::generateWingAdvancedTIE(GLdouble x, GLdouble y, GLdouble z) {

	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;

	GLdouble my = y * 0.5; 

	std::vector<GLuint> indices = {
		//cuadrado superior
		0, 2, 1, 
		0, 3, 2,
		//cuadrado central
		2, 3, 4,
		3, 5, 4,
		//cuadrado inferior
		4, 5, 6,
		5, 7, 6	
	};

	mesh->vIndexes = indices;

	std::vector<vec3> vertices = {
		{-x, y, 0}, //0
		{x, y, 0}, //1
		{x, my, z}, //2
		{-x, my, z}, //3
		{x, -my, z}, //4
 		{-x, -my, z}, //5
		{x, -y, 0}, //6
		{-x, -y, 0} //7
		
	};

	mesh->vVertices = vertices;
	mesh->mNumVertices = mesh->vVertices.size();
	return mesh;
}

IndexMesh*
IndexMesh::generateWingAdvancedTIETexCor(GLdouble x, GLdouble y, GLdouble z) {
	IndexMesh* mesh = IndexMesh::generateWingAdvancedTIE(x, y, z);
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	std::vector<vec2> vertices = {
		{0,1}, //0
		{1,1}, //1
		{1,0.66}, //2
		{0,0.66}, //3
		{1,0.33}, //4
		{0,0.33}, //5
		{1,0}, //6
		{0,0} //7

	};

	mesh->vTexCoords = vertices;
	return mesh;
}


void IndexMesh::buildNormalVectors() {
	// calculo de las normales
	// Construir el vector de normales del mismo tamaño que el de vertices
	vNormals.resize(vVertices.size(), vec3(0.0));

	// Inicializar el vector de normales a cero
	// Calcular la normal de cada triángulo
	for (size_t k = 0; k < vIndexes.size(); k += 3) {
		vec3 normal = normalize(cross(
			vVertices[vIndexes[k + 2]] - vVertices[vIndexes[k]],
			vVertices[vIndexes[k + 1]] - vVertices[vIndexes[k]]
		));
		vNormals[vIndexes[k]] += normal;
		vNormals[vIndexes[k + 1]] += normal;
		vNormals[vIndexes[k + 2]] += normal;
	}

	// Normalizar las normales
	for (vec3& n : vNormals) {
		n = normalize(n);
	}

}