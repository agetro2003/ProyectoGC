#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <numbers>

class Mesh
{
public:
	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generateRegularPolygon(GLuint num, GLdouble r); // creates a new regular polygon mesh
	static Mesh* generateRGBTriangle(GLdouble r); // creates a new RGB triangle mesh
	static Mesh* generateRectangle(GLdouble w, GLdouble h); // creates a new rectangle mesh
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h); // creates a new RGB rectangle mesh
	static Mesh* generateCube(GLdouble length); //generates cube mesh (TRIANGLE_STRIP)
	static Mesh* generateCube2(GLdouble length); //generates cube mesh (TRIANGLES)
	static Mesh* generateRGBCubeTriangles(GLdouble length);//cube with RGB faces
	static Mesh* generateGround(GLdouble lenght);//ground -Ap18
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h); //Ground con textura -Ap20
	static Mesh* generatePhotoTexCor(GLdouble w, GLdouble h); 
	static Mesh* generaRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh); //Ground textura repetida -Ap21
	static Mesh* generateBoxOutline(GLdouble width, GLdouble height); //Caja sin tapas -Ap22 (bien)
	static Mesh* generateBoxOutline2(GLdouble length); //Caja sin tapas -Ap22 (deprecated)
	static Mesh* generateBoxOutlineTexCor(GLdouble width, GLdouble height); //Caja con textura -Ap24
	static Mesh* generateStar3D(GLdouble re, GLuint np, GLdouble h); //Estrella -Ap26
	static Mesh* generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h); //Estrella con textura -Ap27
	static Mesh* generateGlassParapet(GLdouble width, GLdouble height); //apartado 32

	Mesh();
	virtual ~Mesh();

	Mesh(const Mesh& m) = delete;            // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; }; // number of elements
	std::vector<glm::vec3> const& vertices() const { return vVertices; };
	std::vector<glm::vec4> const& colors() const { return vColors; };

	//void load();
	//void unload();

	virtual void load();
	virtual void unload();

protected:
	GLuint mPrimitive =
	  GL_TRIANGLES;          // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0; // number of elements ( = vVertices.size())
	std::vector<glm::vec3> vVertices; // vertex array
	std::vector<glm::vec4> vColors;   // color array
	std::vector<glm::vec2> vTexCoords; //vector de coordenadas
	std::vector<glm::vec3> vNormals; // normal array

	virtual void draw() const;

	GLuint mVAO;  // vertex array object
	GLuint mNBO;  // normal buffer object
private:
	GLuint mVBO;  // vertex buffer object
	GLuint mCBO;  // color buffer object
	GLuint mTCO; // texture coordinates object
};


class IndexMesh : public Mesh {
	
public:
	IndexMesh();
	virtual ~IndexMesh();
	std::vector<GLuint> vIndexes;
	

	void load()  override;
	void unload()  override;
	void draw() const override;
	void buildNormalVectors();
	static IndexMesh* generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax = 2 * std::numbers::pi);
	static IndexMesh* generateIndexedBox(GLdouble l);
	static IndexMesh* generateWingAdvancedTIE(GLdouble x, GLdouble y, GLdouble z);
	static IndexMesh* generateWingAdvancedTIETexCor(GLdouble x, GLdouble y, GLdouble z);
	static IndexMesh* generateWallWithDoor(GLdouble width, GLdouble height); //apartado 33

private:
	GLuint mIBO;
};


#endif //_H_Scene_H_
