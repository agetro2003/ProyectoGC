#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"
class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr) {};
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::mat4& modelViewMat) const = 0; // abstract method

	// modeling matrix
	glm::mat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::mat4 const& aMat) { mModelMat = aMat; };
	void move(glm::vec3 mov_direccion);
	void rotate(GLfloat angulo, glm::vec3 eje);

	// load or unload entity data into the GPU
	virtual void load();
	virtual void unload();

	//update the model matrix
	virtual void update() {};
	
	//int orbit_flag = 0;

protected:
	
	Mesh* mMesh = nullptr; // the mesh
	glm::mat4 mModelMat;  // modeling matrix
	Shader* mShader; // shader

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
	
};

class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	void render(const glm::mat4& modelViewMat) const override;
};

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

class SingleColorEntity : public Abs_Entity
{
	public:
		// Constructor
		explicit SingleColorEntity(glm::dvec4 mColor = glm::dvec4(1.0, 1.0, 1.0, 1.0));

		// Get color
		glm::dvec4 color() const;

		// Set color
		void setColor(glm::dvec4 color);

		// Render
		void render(const glm::mat4& modelViewMat) const override;


	protected:
		glm::dvec4 mColor;
};

class RegularPolygon : public SingleColorEntity
{
public: 
	// Constructor
	RegularPolygon(GLuint num, GLdouble r, glm::dvec4 mColor = glm::dvec4(1.0, 1.0, 1.0, 1.0));


};

class RGBTriangle : public EntityWithColors
{
	// Constructor
	public:
		explicit RGBTriangle(GLdouble l);
		void render(const glm::mat4& modelViewMat) const override;
		void update() override;
		// method to set the position of the triangle
		void setPos(glm::vec3 pos);
		// method to rotate the triangle over the z axis
		void rotate();
		//method to orbit the triangle around the origin
		void orbit();


};

class RGBRectangle : public EntityWithColors
{
	// Constructor
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
};

class Cube : public SingleColorEntity 
{
public:
	// Constructor
	Cube(GLdouble lenght, glm::dvec4 mColor = glm::dvec4(0.0, 0.0, 0.0, 1.0));
	void render(const glm::mat4& modelViewMat) const override;

};

class RGBCube : public EntityWithColors 
{
public:
	// Constructor
	RGBCube(GLdouble lenght);

	GLdouble alpha = 0;//Contador de ángulo para rotación

	void render(const glm::mat4& modelViewMat) const override;

	// method to set the position of the cube
	void setPos(glm::vec3 pos);
	// method to rotate the cube over the axis
	void rotatex();
	void rotatey();
	void rotatez();

	void update() override;

};


class EntityWithTexture : public Abs_Entity {
public:
	//Constructor
	explicit EntityWithTexture();
	void render(const glm::mat4& modelViewMat) const override;
	virtual void rearrange(glm::vec3 pos);
	Texture* mTexture;	
	//destructor
	~EntityWithTexture();

protected:
	
	bool mModulate=false;

};

class Ground :public EntityWithTexture
{
public:
	//Constructor
	Ground(GLdouble lenght);

	void render(const glm::mat4& modelViewMat) const override;
	void rotate();

};

class Ground2 : public EntityWithTexture
{
public:
	//constructor
	Ground2(GLdouble w, GLdouble h, GLuint rw, GLuint rh );

	void render(const glm::mat4& modelViewMat) const override;
	void rotate();

};

class BoxOutline : public EntityWithTexture
{
public:
	// Constructor
	BoxOutline(GLdouble lenght, glm::dvec4 mColor = glm::dvec4(0.0, 0.0, 0.0, 1.0));
	void render(const glm::mat4& modelViewMat) const override;
	Texture* mInsideTexture;
	//destructors
	~BoxOutline();

};

class Box : public EntityWithTexture
{
public:
	// Constructor
	Box(GLdouble lenght, glm::dvec4 mColor = glm::dvec4(0.0, 0.0, 0.0, 1.0));
	void render(const glm::mat4& modelViewMat) const override;
	Texture* mInsideTexture;
	GLdouble l;
	GLdouble alpha = 0;
	void load() override;
	void unload() override;
	void rearrange(glm::vec3 pos);
	void update();
	GLdouble adjustY();
	GLdouble adjustZ();
	//destructors
	~Box();

protected:
	Mesh* mMeshTapa = nullptr; 
	glm::mat4 mModelMatTapa;  // modeling matrix
	glm::mat4 mModelMatBottom;  // modeling matrix
	Mesh* mMeshBottom = nullptr; 


};


class Star3D : public EntityWithTexture
{
public:
	//constructor
	Star3D(GLdouble re, GLuint np, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update();

};

class GlassParapet : public EntityWithTexture
{
public:
	// Constructor
	GlassParapet(GLdouble width, GLdouble height,  glm::dvec4 mColor = glm::dvec4(0.0, 0.0, 0.0, 1.0));
	void render(const glm::mat4& modelViewMat) const override;
};

class Photo : public EntityWithTexture
{
public:
	Photo(GLdouble lenght);

	void update();	
	GLsizei l=50;

	void render(const glm::mat4& modelViewMat) const override;

	void rotate(glm::vec3 pos);

};

class Grass : public EntityWithTexture
{
public:
	Grass(GLdouble lenght);
	void render(const glm::mat4& modelViewMat) const override;
};

class EntityWithMaterial : public Abs_Entity
{
public:
	EntityWithMaterial();
	void setMaterial(const Material& m) { mMaterial = m; };
	void render(const glm::mat4& modelViewMat) const override;

protected:
	Material mMaterial;

};

class ColorMaterialEntity : public EntityWithMaterial
{
public:
	ColorMaterialEntity(glm::vec3 mColor = glm::vec3(1.0, 1.0, 1.0));
	~ColorMaterialEntity();
	void render(const glm::mat4& modelViewMat) const override;
	static bool mShowNormals;
	static void toggleShowNormals();
	void setShader(Shader* shader) { mShader = shader; }
	Texture* mTexture = nullptr;
	void setColor(glm::vec3 color);
protected:
	bool mModulate = false;
	glm::vec3 mColor;


};

class Torus : public ColorMaterialEntity
{
	public: 
		// Constructor
		Torus(GLdouble R, GLdouble r, GLuint nPoints = 40, GLuint nSamples = 40);

};

class Sphere : public ColorMaterialEntity
{
public:
	// Constructor
	Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians, GLfloat angleMax = 2 * std::numbers::pi);
};

class Disk : public ColorMaterialEntity
{
public:
	// Constructor
	Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, GLfloat angleMax = 2 * std::numbers::pi);
};


class  Cone : public ColorMaterialEntity
{
public:
	// Constructor
	Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples);
};

class IndexedBox : public ColorMaterialEntity
{
public:
	// Constructor
	IndexedBox(GLdouble lenght);
};


class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity();
	~CompoundEntity();
	void addEntity(Abs_Entity* ae);
	void load() override;
	void unload() override;
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
	void scale(glm::vec3 scale_vec);
	void rotate(GLfloat angulo, glm::vec3 eje);
	std::vector<Abs_Entity*> getEntities() { return gObjects; };


protected:
	std::vector<Abs_Entity*> gObjects;
	void destroy();


};

class AdvancedTIE : public CompoundEntity
{
public:
	AdvancedTIE();
	~AdvancedTIE();
	void render(const glm::mat4& modelViewMat) const override;
	SpotLight* foco = new SpotLight({ 0, 0, 0 }, 1);
	void changeFoco();
};

class WingAdvancedTIE : public ColorMaterialEntity
{
public:
	WingAdvancedTIE(GLdouble x, GLdouble y, GLdouble z);
	void render(const glm::mat4& modelViewMat) const override;
};

class NodoFicticio : public CompoundEntity 
{
public:
	NodoFicticio();
	void render(const glm::mat4& modelViewMat) const override;
	void orbit();
	void rotate();
};

class Granjero : public CompoundEntity 
{
public:
	Granjero();
};


class Persona : public CompoundEntity
{
public: 
	Persona();
	void walk(GLint dir, GLint action);
	int currentDir = 4 ;

	CompoundEntity* brazo1Node = new CompoundEntity();
	CompoundEntity* brazo2Node = new CompoundEntity();
	CompoundEntity* pierna1Node = new CompoundEntity();
	CompoundEntity* pierna2Node = new CompoundEntity();

	int angle = 0;
	
};
class WallWithDoor : public ColorMaterialEntity
{
public:
	WallWithDoor(GLdouble width, GLdouble height, glm::dvec4 mColor = glm::dvec4(0.0, 0.0, 0.0, 1.0));
};


#endif //_H_Entities_H_
