#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <cmath>

using namespace glm;

void
Abs_Entity::upload(const mat4& modelViewMat) const
{
	mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

void
Abs_Entity::load()
{
	mMesh->load();
}

void
Abs_Entity::unload()
{
	mMesh->unload();
}

EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

void
EntityWithColors::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

RGBAxes::RGBAxes(GLdouble l)
{
	mShader = Shader::get("vcolors");
	mMesh = Mesh::createRGBAxes(l);
	//load();
}

// Constructor with a color.
SingleColorEntity::SingleColorEntity(glm::dvec4 mColor)
{
	mShader = Shader::get("simple");
}

// Get the color of the entity.
glm::dvec4 SingleColorEntity::color() const { return mColor; }

// Set the color of the entity.
void SingleColorEntity::setColor(glm::dvec4 color) { mColor = color; }

// Render with mShader->setUniform("color", mColor).
void SingleColorEntity::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("color", glm::vec4(mColor));
		//mShader->setUniform("color", mColor);

		mMesh->render();
	}
}


// Create a regular polygon with num vertices, radius r and color mColor.
RegularPolygon::RegularPolygon(GLuint num, GLdouble r, glm::dvec4 mColor)
{
	RegularPolygon::setColor(mColor);
	mShader = Shader::get("simple");
	mMesh = Mesh::generateRegularPolygon(num, r);
	//load();
}



// Constructor
RGBTriangle::RGBTriangle(GLdouble l)
{
	
	mShader = Shader::get("vcolors");
	//rellenar el triangulo
	mMesh = Mesh::generateRGBTriangle(l);

//	load();

}


void RGBTriangle::setPos(glm::vec3 pos)
{
	mModelMat = glm::translate(mModelMat, pos);
}

void RGBTriangle::rotate()
{
	//Move to the origin
	glm::vec3 initialPos = glm::vec3(mModelMat[3]);
	glm::mat4 toOrigin = glm::translate(glm::mat4(1.0), -initialPos);
	//Rotate clockwise
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(18.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	//Move back to the initial position
	glm::mat4 toPos = glm::translate(glm::mat4(1.0), initialPos);

	mModelMat = toPos * rotateMat * toOrigin * mModelMat;

}

void RGBTriangle::orbit()
{
	// orbit the triangle around the origin counterclockwise
	glm::mat4 orbitMat = glm::rotate(glm::mat4(1.0), glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mModelMat = orbitMat * mModelMat;
}

//render the RGBTriangle
void RGBTriangle::render(const glm::mat4& modelViewMat) const
{

		if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();
		glDisable(GL_CULL_FACE);

	}
}

//update the RGBTriangle
void RGBTriangle::update()
{
	rotate();
	orbit();
}

// Constructor
RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mShader = Shader::get("vcolors");
	mMesh = Mesh::generateRGBRectangle(w, h);
	//load();
}

void RGBRectangle::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
		mMesh->render();
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		mMesh->render();
		glDisable(GL_CULL_FACE);
	}
}


//Constructor
Cube::Cube(GLdouble lenght, glm::dvec4 mColor)
{

	mShader = Shader::get("vcolors");
	//create cube
	mMesh = Mesh::generateCube(lenght);	//Cubo generado con 14 vértices y GL_TRIANGLE_STRIP como primitiva, usado en el apartado 15 de la práctica 1
	//mMesh = Mesh::generateCube2(lenght);	//Cubo generado con 36 vértices GL_TRIANGLES como primitiva, usado en el apartado 16 de la práctica 1

	//load();

}

void Cube::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("color", glm::vec4(mColor));
		//mShader->setUniform("color", mColor);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//mMesh->render();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT );
		mMesh->render();
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();
		glDisable(GL_CULL_FACE);

	}

}


// Constructor
RGBCube::RGBCube(GLdouble l)
{

	mShader = Shader::get("vcolors");
	//create RGB Cube
	mMesh = Mesh::generateRGBCubeTriangles(l);

	//load();

}

void RGBCube::render(const glm::mat4& modelViewMat) const {

	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();
		glCullFace(GL_FRONT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		mMesh->render();
		glDisable(GL_CULL_FACE);

	}


}

//Funciones para rotación del apartado 17 ç8práctica 1)

void RGBCube::setPos(glm::vec3 pos)
{
	mModelMat = glm::translate(mModelMat, pos);
}

//Rotar en x
void RGBCube::rotatex( )
{
	glm::vec3 Pos = glm::vec3(mModelMat[3]);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMat = rotateMat * mModelMat;
}

//Rotar en y
void RGBCube::rotatey()
{
	glm::vec3 Pos = glm::vec3(mModelMat[3]);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMat = rotateMat * mModelMat;
}

//Rotar en z
void RGBCube::rotatez()
{
	glm::vec3 Pos = glm::vec3(mModelMat[3]);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mModelMat = rotateMat * mModelMat;
}

//Rota en x 180 grados, luego en y 180 grados en y, luego 180 grados en z y vuelve a empezar2
void RGBCube::update() {
	if (alpha < 180)
	{
		rotatex();
		alpha += 2;
	}
	else if (alpha < 360)
	{
		rotatez();
		alpha += 2;
	}
	else if (alpha < 540)
	{
		rotatey();
		alpha += 2;
	} 
	else 
	{
		alpha = 0;
		rotatex();
		alpha += 2;
	}
}


EntityWithTexture::EntityWithTexture()
{
	mShader = Shader::get("texture");
}

EntityWithTexture::~EntityWithTexture()
{
	delete mTexture;
	mTexture = nullptr;
}

void EntityWithTexture::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate);
		if (mTexture != nullptr) {
			mTexture->bind();
			mMesh->render();
			mTexture->unbind();
		}
		else {
			mMesh->render();
		}

	}
}

//Método para situar entidades con textura en posiciones distintas al centro de la escena
void EntityWithTexture::rearrange(glm::vec3 pos) {
	glm::mat4 translateMat = glm::translate(mModelMat, pos);
	mModelMat = translateMat * mModelMat;
}

// Constructor de la clase Ground para el apartado 20
Ground::Ground(GLdouble lenght) {

	mShader = Shader::get("texture");
	//create Ground
	mMesh = Mesh::generateRectangleTexCor(lenght, lenght);
	mTexture=new Texture();
	//ruta relativa de la imagen
	mTexture->load("../assets/images/baldosaC.png");

}


void Ground::render(const glm::mat4& modelViewMat) const {

	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate);
		glEnable(GL_CULL_FACE);
		
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);
	}
	
	
}

void Ground::rotate() {

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMat = rotateMat * mModelMat;
}


// Constructor de la clase Ground para el apartado 21
Ground2::Ground2(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {

	mShader = Shader::get("texture");
	//create Ground
	mMesh = Mesh::generaRectangleTexCor(w, h, rw, rh);
	mTexture = new Texture();
	//ruta relativa de la imagen
	mTexture->load("../assets/images/baldosaC.png");

}

void Ground2::render(const glm::mat4& modelViewMat) const {

	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);
	}


}

void Ground2::rotate() {

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMat = rotateMat * mModelMat;
}

//Constructor caja sin tapa apartados 22-25
BoxOutline::BoxOutline(GLdouble lenght, glm::dvec4 mColor)
{

	mShader = Shader::get("texture");
	//create cubo sin tapas
	mMesh = Mesh::generateBoxOutlineTexCor(lenght, lenght);
	mTexture = new Texture();
	mInsideTexture = new Texture();

	//ruta relativa de la imagen
	mTexture->load("../assets/images/container.jpg");
	mInsideTexture->load("../assets/images/papelE.png");

}

//Destructor de la caja sin tapas
BoxOutline::~BoxOutline() {
	delete mMesh;
	mMesh = nullptr;
	delete mInsideTexture;
}

//En el render se emplea una textura para el front y otra para el back
void BoxOutline::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate);
	

		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mInsideTexture != nullptr) {
			mInsideTexture->bind();
		}
		mMesh->render();
		if (mInsideTexture != nullptr) {
			mInsideTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);

	}

}

//Contructor de la caja con tapas que se abre apartaods opcionales 30-31
Box::Box(GLdouble lenght, glm::dvec4 mColor)
{
	l = lenght;
	mShader = Shader::get("texture");
	//create cubo sin tapas
	mMesh = Mesh::generateBoxOutlineTexCor(lenght, lenght);	//Mesh de la caja
	mMeshTapa = Mesh::generaRectangleTexCor(lenght, lenght, 1,1); //Mesh de la tapa
	mMeshBottom= Mesh::generaRectangleTexCor(lenght, lenght, 1, 1); //Mesh del fondo
	mTexture = new Texture();
	mInsideTexture = new Texture();

	//ruta relativa de la imagen
	mTexture->load("../assets/images/container.jpg");
	mInsideTexture->load("../assets/images/papelE.png");

	//ModelMat de la tapa, situada horizontalmente en la parte superior de la caja
	mModelMatTapa = glm::mat4(1.0);
	glm::mat4 translateMat = glm::translate(mModelMatTapa, glm::vec3(0, lenght*0.5, 0));
	glm::mat4 rotateMat=glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMatTapa = translateMat * rotateMat * mModelMatTapa;

	//ModelMat del fondo, situado horizontalmente en la parte inferior de la caja
	mModelMatBottom = glm::mat4(1.0);
	translateMat = glm::translate(mModelMatBottom, glm::vec3(0, -lenght*0.5, 0));
	rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMatBottom = translateMat *  rotateMat * mModelMatBottom;

}

//destructor de la caja con tapas
Box::~Box() {
	delete mMeshTapa;
	mMeshTapa = nullptr;
	delete mMeshBottom;
	mMeshBottom = nullptr;
	delete mInsideTexture;
}

//Se cargan y descargan las tres mallas
void
Box::load()
{
	mMesh->load();
	mMeshTapa->load();
	mMeshBottom->load();
}

void 
Box::unload()
{
	mMesh->unload();
	mMeshTapa->unload();
	mMeshBottom->unload();
}

void
Box::rearrange(glm::vec3 pos) {
	
	glm::mat4 translateMat = glm::translate(mModelMat, pos);
	mModelMat = translateMat * mModelMat;

	mModelMatTapa = translateMat * mModelMatTapa; 
	mModelMatBottom = translateMat * mModelMatBottom;

}
// Ajustar la posición de la tapa en z de acuerdo al ángulo de apertura
// Un ciclo completo crece de 0 a 180, de 180 pasa a 0 (-5 en realidad) y de ahí decrece a -180. Para reiniciar, de -180 pasa a 0
/*
* Ángulo 0 => z=0
* Ángulo 90 => z= -0.5*L 
* Ángulo 180 => z=-L
* Ángulo -90 => z= -0.5*L 
* Ángulo -180 => z=0
*/
GLdouble
Box::adjustZ() {
	GLdouble multiplicador;
	if (alpha >= 0) {
		multiplicador = (alpha * l) / 180;
	}
	else {
		multiplicador = (l * (alpha + 180)) / 180;
	}

	return multiplicador;
}
//Ajustar la posición de la tapa en y de acuerdo al ángulo de apertura 
/*
* Ángulo 0 => y=0
* Ángulo 90 => y= 0.5*L 
* Ángulo 180 => y=0
* Ángulo -90 => y=0.5*L
* Ángulo -180 => y=0
*/
GLdouble
Box::adjustY() {
	GLdouble multiplicador = 0;
	multiplicador = abs(alpha) / 180;

	
	if (multiplicador > 0.5) {
		multiplicador = 0.5 - (multiplicador - 0.5);
	}

	return multiplicador * l;
}

//Update de la caja con tapa, la tapa se abre y cierra
//Se lleva cuenta del ángulo de apertura en alpha, los 180 grados de la apertura se consideran positivos y los del cierre negativos
//Para que la tapa se abra correctamente se lleva la malla al origen, se alinea con el eje x, se rota sobre el eje x, se desalinea con el eje x y se devuelve a su posición
void 
Box::update() {
	glm::vec3 initialPos;
	glm::mat4 toOrigin, rotateAbrir, toPos, adjust, deadjust;
	//Matriz de desalineado con el eje x
	deadjust = glm::translate(glm::mat4(1.0), glm::vec3(0, -adjustY(), -adjustZ()));
	mModelMatTapa = deadjust * mModelMatTapa;

	//Los 180 grados de la apertura se rota en positivo sobre x
	if (alpha < 180 and alpha >= 0) {
		initialPos = glm::vec3(mModelMatTapa[3].x, mModelMatTapa[3].y, mModelMatTapa[3].z);
		toOrigin = glm::translate(glm::mat4(1.0), -initialPos);
		rotateAbrir = glm::rotate(glm::mat4(1.0), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		toPos = glm::translate(glm::mat4(1.0), initialPos);
		alpha += 5;
		mModelMatTapa = toPos  * rotateAbrir  * toOrigin  * mModelMatTapa ;
	}

	//Los 180 grados del cierre se rotan en negativo sobre x
	else if (alpha < 0 and alpha >= -180) {
		initialPos = glm::vec3(mModelMatTapa[3]);
		toOrigin = glm::translate(glm::mat4(1.0), -initialPos);
		rotateAbrir = glm::rotate(glm::mat4(1.0), glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		toPos = glm::translate(glm::mat4(1.0), initialPos);
		alpha -= 5;
		mModelMatTapa = toPos * rotateAbrir * toOrigin  * mModelMatTapa;
	}
	else if (alpha == 180) {
		alpha = -5;
	}
	else {
		alpha = 0;
	}

	//Matriz de alineado con el eje x
	adjust = glm::translate(glm::mat4(1.0), glm::vec3(0, adjustY(), adjustZ()));

	mModelMatTapa = adjust * mModelMatTapa;

}

//Se renderizan las tres mallas por separado con sus correspondientes matrices de posición
void Box::render(const glm::mat4& modelViewMat) const {

	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
	
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mInsideTexture != nullptr) {
			mInsideTexture->bind();
		}
		mMesh->render();
		if (mInsideTexture != nullptr) {
			mInsideTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);

	}

	if (mMeshTapa != nullptr) {
		mat4 aMatTapa = modelViewMat * mModelMatTapa; // glm matrix multiplication
		
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMatTapa);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mInsideTexture != nullptr) {
			mInsideTexture->bind();
		}
		mMeshTapa->render();
		if (mInsideTexture != nullptr) {
			mInsideTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMeshTapa->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);

	}

	if (mMeshBottom != nullptr) {
		mat4 aMatBottom = modelViewMat * mModelMatBottom; // glm matrix multiplication
		
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMatBottom);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mInsideTexture != nullptr) {
			mInsideTexture->bind();
		}
		mMeshBottom->render();
		if (mInsideTexture != nullptr) {
			mInsideTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMeshBottom->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);

	}
}

//Constructor estrella apartado 26-29
Star3D::Star3D(GLdouble re, GLuint np, GLdouble h) {
	mShader = Shader::get("texture");
	//create 3DStar
	//mMesh = Mesh::generateStar3D(re, np, h);
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
	mTexture = new Texture();

	//ruta relativa de la imagen
	mTexture->load("../assets/images/baldosaP.png");


} 

//Reder de la estrella doble: primero se renderiza la malla una vez, luego se rota 180 grados y se vuelve a renderizar para generar la estrella doble
void Star3D::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mTexture != nullptr) {
			mTexture->unbind();
		}

		
		aMat = modelViewMat * mModelMat * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//aMat = modelViewMat * glm::translate(glm::mat4(1.0), -glm::vec3(mModelMat[3])) * mModelMat;
		upload(aMat);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		

		glDisable(GL_CULL_FACE);
	}
}

//update de las estrellas: rotación en y sobre el centro de la estrella y rotación en z sobre si misma
void Star3D::update()
{
	//Move to the origin
	glm::vec3 initialPos = glm::vec3(mModelMat[3]);
	glm::mat4 toOrigin = glm::translate(glm::mat4(1.0), -initialPos);
	glm::mat4 rotateMatY = glm::rotate(glm::mat4(1.0), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Move back to the initial position
	glm::mat4 toPos = glm::translate(glm::mat4(1.0), initialPos);
	
	glm::mat4 rotateMatZ = glm::rotate(glm::mat4(1.0), glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	mModelMat = toPos * rotateMatY * toOrigin * mModelMat *rotateMatZ;
}


//Constructor caja sin tapa con paredes translúcidas, apartado 32
GlassParapet::GlassParapet(GLdouble width, GLdouble height, glm::dvec4 mColor)
{

	mShader = Shader::get("texture:texture_alpha");
	//create cubo sin tapas
	mMesh = Mesh::generateGlassParapet(width, height);
	mTexture = new Texture();

	//ruta relativa de la imagen
	mTexture->load("../assets/images/windowV.jpg", 100);

}

void GlassParapet::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		glEnable(GL_CULL_FACE);
		
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);


		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}

//Ap-35, constructor de la foto, captura en su textura la imagen del contenido de la escena (600x600)
Photo::Photo(GLdouble lenght)
{
	mShader = Shader::get("texture");
	//create foto
	mMesh = Mesh::generatePhotoTexCor(lenght, lenght);
	mTexture = new Texture();
	l = 600;

}


void Photo::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate);
		mTexture->loadColorBuffer(l, l);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Condicional para verificar si hay textura
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_CULL_FACE);
	}
}

void Photo::update() {
	mTexture->loadColorBuffer(l, l);
	
}

void Photo::rotate(glm::vec3 pos) {

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 translateMat= glm::translate(mModelMat, pos);
	mModelMat = translateMat * rotateMat * mModelMat;
}


//Hierba del apartado opcional 33, emplea la imagen grass_alpha.png como textura
Grass::Grass(GLdouble lenght)
{
	mShader = Shader::get("texture:texture_alpha");
	//create foto
	mMesh = Mesh::generateRectangleTexCor(lenght, lenght);
	mTexture = new Texture();
	mTexture->load("../assets/images/grass_alpha.png");
}

//La malla del rectángulo de hierba original se renderiza un vez, luego se rota 120 grados, se renderiza de nuevo, se rota otros 120 grados y se renderiza una última vez para crear la hierba
void
Grass::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		if (mTexture != nullptr) {
			mTexture->bind();
		}
		mMesh->render();

		aMat = modelViewMat * mModelMat * glm::rotate(glm::mat4(1.0), glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//aMat = modelViewMat * glm::translate(glm::mat4(1.0), -glm::vec3(mModelMat[3])) * mModelMat;
		upload(aMat);

		mMesh->render();

		aMat = modelViewMat * mModelMat * glm::rotate(glm::mat4(1.0), glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		upload(aMat);

		mMesh->render();

		if (mTexture != nullptr) {
			mTexture->unbind();
		}
	}
}

EntityWithMaterial::EntityWithMaterial() {
	mShader = Shader::get("light");
}
void
EntityWithMaterial::render(const mat4& modelViewMat) const
{
	mShader->use();
	mMaterial.upload(*mShader);
	upload(modelViewMat * mModelMat);
	mMesh->render();
}


// Apartado 58
ColorMaterialEntity::ColorMaterialEntity(glm::vec3 mColor)
{
	mShader = Shader::get("light");
	mColor = mColor;
	mMaterial = Material(mColor);

}

ColorMaterialEntity::~ColorMaterialEntity() {
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
}

bool
ColorMaterialEntity::mShowNormals = false;

void 
ColorMaterialEntity::toggleShowNormals() {
	 mShowNormals = !mShowNormals; 
}

void 
ColorMaterialEntity::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mShader->use();
		mMaterial.upload(*mShader);
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);

	//	mShader->setUniform("color", glm::vec4(mColor));
		mMesh->render();
		if (mShowNormals) {
			Shader* mNormalShader = Shader::get("normals");
			mNormalShader->use();
			mNormalShader->setUniform("modelView", aMat);


			mMesh->render();
		}
	}
}

void
Abs_Entity::rotate(GLfloat angulo, glm::vec3 eje) {

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), angulo, eje);
	mModelMat = rotateMat * mModelMat;
}

void
Abs_Entity::move(glm::vec3 mov_direccion) {

	glm::mat4 translateMat = glm::translate(glm::mat4(1.0), mov_direccion);
	mModelMat = translateMat * mModelMat;
}
void 
ColorMaterialEntity::setColor(glm::vec3 color) {
	mColor = color;
	mMaterial = Material(color);
	
}

// Apartado 56
Torus::Torus(GLdouble R, GLdouble r, GLuint nPoints, GLuint nSamples)
{
	std::vector<vec2> profile;
	GLdouble t0 = 360.0 / nPoints;
	GLdouble ti = t0;

	for (GLint i = 0; i <= nPoints; i++)
	{
		profile.push_back ({ r * cos(radians(ti)) + R, r * sin(radians(ti)) });
		ti += t0;
	}
	mMesh = IndexMesh::generateByRevolution(profile, nSamples);
}

//Apartado 64
Sphere::Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians, GLfloat angleMax)
{
	std::vector<vec2> profile;
	GLdouble t0 = 180.0 / nParallels;
	GLdouble ti = -90;
	for (GLint i = 0; i <= nParallels; i++)
	{
		profile.push_back({ radius * cos(radians(ti)), radius * sin(radians(ti)) });
		ti += t0;
	}

	mMesh = IndexMesh::generateByRevolution(profile, nMeridians, angleMax);
}

Disk::Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, GLfloat angleMax) {
	std::vector<vec2> perfil;

	GLdouble t0 = (R - r) / nRings;
	GLdouble ti = r;
	for (GLint i = 0; i <= nRings; i++)
	{
		perfil.push_back({ ti, 0 });
		ti += t0;
	}

	mMesh = IndexMesh::generateByRevolution(perfil, nSamples, angleMax);
}


Cone::Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples)
{
	std::vector<vec2> perfil;
	// Perfil (r, y) para y E [h/2,..., -h/2]
	GLdouble a = (R * R - r * r)/h;
	GLdouble b = (R * R + r * r) * 0.5;
	GLdouble t0 = h / nRings;
	GLdouble ti = -h / 2;
	for (GLint i = 0; i <= nRings; i++)
	{
		perfil.push_back( { a*ti + b, ti });
		ti += t0;
	}

	mMesh = IndexMesh::generateByRevolution(perfil, nSamples);
}

//Ap 60
IndexedBox::IndexedBox(GLdouble lenght)
{
	mMesh = IndexMesh::generateIndexedBox(lenght);
	mShowNormals = false;
}

//Ap 65 Entidad compuesta
CompoundEntity::CompoundEntity() {
	mShader = Shader::get("simple_light");
}

CompoundEntity::~CompoundEntity() {
	destroy();
}

void
CompoundEntity::destroy() {
	for (Abs_Entity* ae : gObjects) {
		delete ae;
	}
}

void 
CompoundEntity::addEntity(Abs_Entity* ae) {
	gObjects.push_back(ae);
}

void
CompoundEntity::render(const glm::mat4& modelViewMat) const
{
	for (Abs_Entity* obj : gObjects) {
		obj->render(modelViewMat * mModelMat);
	}		
}

void
CompoundEntity::update()
{
	for (Abs_Entity* obj : gObjects)
		obj->update();
}

void
CompoundEntity::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void
CompoundEntity::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();
}

void 
CompoundEntity::scale(glm::vec3 scale_vec){

	for (Abs_Entity* obj : gObjects) {
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), scale_vec);
		obj->setModelMat( scaleMat * obj->modelMat()); 
	}

}


void
CompoundEntity::rotate(GLfloat angulo, glm::vec3 eje) {

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), angulo, eje);
	mModelMat = rotateMat * mModelMat;
	
	/*
	for (Abs_Entity* obj : gObjects) {
		glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), angulo, eje);
		obj->setModelMat(rotateMat * obj->modelMat());
	}
	*/

}

NodoFicticio::NodoFicticio() {
	mShader = Shader::get("simple_light");
}

void 
NodoFicticio::render(const glm::mat4& modelViewMat) const
{
	for (Abs_Entity* obj : gObjects) {
		obj->render(modelViewMat * mModelMat);
	}
}

void
NodoFicticio::orbit() {
	setModelMat(glm::rotate(modelMat(), glm::radians(3.0f), glm::vec3(1, 0, 0)));
}

void
NodoFicticio::rotate() {
	setModelMat(glm::rotate(modelMat(), glm::radians(3.0f), glm::vec3(0, 1, 0)));
}

//Apartado 66
AdvancedTIE::AdvancedTIE(){
	mShader = Shader::get("light");
	foco->setAmb(glm::vec4(.25, .25, .25, 1));
	foco->setDiff(glm::vec4(.7, .7, .7, 1));
	foco->setSpec(glm::vec4(0, 0.2, 0, 1));
	foco->setEnabled(true);
	foco->setPosition(glm::vec4(0, 0, 0, 1));
	foco->setDirection(glm::vec3(0, -1, 0));
	foco->setCutoff(45, 50);
	//foco->setCutoff(10, 60);
	
	glm::dvec4 color = glm::dvec4(0.0, 65.0/255.0, 106.0/255.0, 1.0);
	Sphere* core = new Sphere(100, 180, 180);
	core->setColor(color);
    addEntity(core);

	Cone* eje = new Cone(380, 3.5, 3.5, 20, 20);
	eje->setColor(color);
	eje->rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	addEntity(eje);

	CompoundEntity* morroCompuesto = new CompoundEntity();

	Cone* morro = new Cone(150, 5, 5, 20, 20);
	morro->setColor(color);
	morro->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	morro->move(glm::vec3(0.0f, 0.0f, 60.0f));
	//addEntity(morro);
	morroCompuesto->addEntity(morro);

	Disk* tapa_morro = new Disk(24.75, 0, 20, 360);
	tapa_morro->setColor(color);
	tapa_morro->rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	tapa_morro->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	tapa_morro->move(glm::vec3(0.0f, 0.0f, 135.0f));
	//addEntity(tapa_morro);
	morroCompuesto->addEntity(tapa_morro);

	addEntity(morroCompuesto);

	WingAdvancedTIE* ala_izq = new WingAdvancedTIE(100, 120, 100);
	//ala_izq->setColor(glm::dvec4(0.0, 65.0, 106.0, 1.0));
	ala_izq->rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ala_izq->move(glm::vec3(-75.0f, 0.0f, 0.0f));
	addEntity(ala_izq);

	WingAdvancedTIE* ala_der = new WingAdvancedTIE(100, 120, 100);
	//ala_der->setColor(glm::dvec4(0.0, 65.0, 106.0, 1.0));
	ala_der->rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ala_der->move(glm::vec3(75.0f, 0.0f, 0.0f));
	addEntity(ala_der);

}

AdvancedTIE::~AdvancedTIE() {
	delete foco;
	foco = nullptr;

}

void
AdvancedTIE::render(const glm::mat4& modelViewMat) const {
	mShader->use();
	foco->upload(*mShader, modelViewMat * mModelMat);
	
		for (Abs_Entity* obj : gObjects) {
			obj->render(modelViewMat * mModelMat);
		}

}

void
AdvancedTIE::changeFoco() {
	foco->setEnabled(!foco->enabled());
}

WingAdvancedTIE::WingAdvancedTIE(GLdouble x, GLdouble y, GLdouble z) {
	mShader= Shader::get("texture:texture_alpha");
	
	mMesh = IndexMesh::generateWingAdvancedTIETexCor(x, y, z);
	mTexture = new Texture();
	mTexture->load("../assets/images/noche.jpg", 150);
}

void
WingAdvancedTIE::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mShader->setUniform("modulate", mModulate);
		if (mTexture != nullptr) {
			
			mTexture->bind();
		}
		mMesh->render();
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		if (mTexture != nullptr) {
			mTexture->unbind();
		}

		if (mShowNormals) {
			Shader* mNormalShader = Shader::get("normals");
			mNormalShader->use();
			mNormalShader->setUniform("modelView", aMat);


			mMesh->render();
		}

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

	}
}

//Apartado 69
Granjero::Granjero() {

	Sphere* cabeza = new Sphere(150, 180, 180);
	cabeza->setColor(glm::dvec4(1.0, 0.5, 0.0, 1.0));
	addEntity(cabeza);

	Disk* sombrero = new Disk(200, 0, 20, 360);
	sombrero->setColor(glm::dvec4(1.0, 0.0, 0.0, 1.0));
	sombrero->rotate(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	sombrero->move(glm::vec3(0.0f, 110.0f, 0.0f));
	addEntity(sombrero);

	Cone* ojo_der = new Cone(30, 3.5, 0, 20, 20);
	ojo_der->setColor(glm::dvec4(0.0, 0.0, 1.0, 1.0));
	ojo_der->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ojo_der->move(glm::vec3(50.0f, 60.0f, 135.0f));
	addEntity(ojo_der);

	Cone* ojo_izq = new Cone(30, 3.5, 0, 20, 20);
	ojo_izq->setColor(glm::dvec4(0.0, 0.5, 1.0, 1.0));
	ojo_izq->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ojo_izq->move(glm::vec3(-50.0f, 60.0f, 135.0f));
	addEntity(ojo_izq);

	Disk* boca = new Disk(125, 0, 20, 360, std::numbers::pi);
	boca->setColor(glm::dvec4(0.0, 1.0, 0.0, 1.0));
	boca->rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	boca->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	boca->rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	boca->move(glm::vec3(0.0f, 0.0f, 100.0f));
	addEntity(boca);
}

Persona::Persona() {
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
	Sphere* cabeza = new Sphere(80, 60, 60);
	cabeza->setColor(color);
	cabeza->move(glm::vec3(0.0f, 100.0f, 0.0f));
	addEntity(cabeza);

	Sphere* ojo1 = new Sphere(20, 60, 60);
	ojo1->setColor(glm::vec3(1, 1, 1));
	ojo1->move(glm::vec3(70.0f, 120.0f, 30.0f));
	addEntity(ojo1);

	Sphere* pupila1 = new Sphere(10, 60, 60);
	pupila1 -> setColor(glm::vec3(0, 0, 0));
	pupila1->move(glm::vec3(85.0f, 120.0f, 30.0f));
	addEntity(pupila1);

	Sphere* ojo2 = new Sphere(20, 60, 60);
	ojo2->setColor(glm::vec3(1, 1, 1));
	ojo2->move(glm::vec3(70.0f, 120.0f, -30.0f));
	addEntity(ojo2);

	Sphere* pupila2 = new Sphere(10, 60, 60);
	pupila2->setColor(glm::vec3(0, 0, 0));
	pupila2->move(glm::vec3(85.0f, 120.0f, -30.0f));
	addEntity(pupila2);


	Cone* cuerpo = new Cone(150, 8, 8, 60, 60);
	cuerpo->setColor(color);
	addEntity(cuerpo);
	Cone* pierna1 = new Cone(150, 2, 2, 60, 60);
	pierna1->move(glm::vec3(0.0f, -100.0f, 30.0f));
	pierna1->setColor(color);
	addEntity(pierna1);
	Cone* pierna2 = new Cone(150, 2, 2, 60, 60);
	pierna2->setColor(color);
	pierna2->move(glm::vec3(0.0f, -100.0f, -30.0f));

	addEntity(pierna2);

	Cone* brazo1 = new Cone(80, 2, 2, 60, 60);
	brazo1->rotate(-45, glm::vec3(1, 0, 0));

	brazo1->setColor(color);
	brazo1->move(glm::vec3(0.0f,0.0f,90.0f));

	addEntity(brazo1);
	Cone* brazo2 = new Cone(80, 2, 2, 60, 60);
	brazo2->setColor(color);
	brazo2->rotate(45, glm::vec3(1, 0, 0));
	brazo2->move(glm::vec3(0.0f,0.0f, -90.0f));

	addEntity(brazo2);

}

void
Persona::walk(GLint dir) {
	switch (dir) {
	case 1: 
		move(glm::vec3(0, 0, -1));
		break;
	case 2:
		move(glm::vec3(-1, 0, 0));
		break;
	case 3:
		move(glm::vec3(0, 0, 1));
		break;
	case 4:
		move(glm::vec3(1, 0, 0));
		break;
	}
	currentDir = dir;
}

