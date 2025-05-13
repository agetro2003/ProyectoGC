#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init() = 0;
	//update gObjects 
	virtual void update();

	virtual bool handleKey(unsigned int key);
	virtual bool handleSpecialKey(int key, int scancode, int action, int mods);

	void render(Camera const& cam) const;

	// load or unload scene data into the GPU
	void load();
	void unload();

	void uploadLights(const glm::dmat4& mViewMat) const;

	DirLight *dirLight = new DirLight(); 
	bool lightON = true;
	Shader *shader = Shader::get("light");

	//void setNormals();


protected:
	void destroy();
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Texture*> gTextures;
	std::vector<Light*> gLights;


};

//scene0
class Scene0 : public Scene
{
public:
	Scene0() = default;
	~Scene0() = default;

	void init() override; 
};


//scene1
class Scene1 : public Scene {
public:
	Scene1() = default;
	~Scene1() = default;
	Persona* persona = new Persona();
	bool handleSpecialKey(int key, int scancode, int action, int mods) override;

	void init() override;
};

//scene2
class Scene2 : public Scene {
public:
	Scene2() = default;
	~Scene2() = default;

	void init() override;
};

//scene3
class Scene3 : public Scene {
public:
	Scene3() = default;
	~Scene3() = default;

	void init() override;
};

//scene4
class Scene4 : public Scene {
public:
	Scene4() = default;
	~Scene4() = default;

	void init() override;
};

//scene5
class Scene5 : public Scene {
public:
	Scene5() = default;
	~Scene5() = default;
	
	void init() override;
};

//scene6
class Scene6 : public Scene {
public:
	Scene6() = default;
	~Scene6() = default;
	void init() override;
};

//scene7
class Scene7 : public Scene {
public:
	Scene7() = default;
	~Scene7() = default;
	void init() override;
};

//scene8
class Scene8 : public Scene {
public:
	Scene8() = default;
	~Scene8() = default;
	void init() override;
	bool handleKey(unsigned int key) override;
	PosLight *posLight = new PosLight();
	SpotLight* spotLight = new SpotLight();
};

//scene9
class Scene9 : public Scene {
public:
	Scene9() = default;
	~Scene9() = default;
	void init() override;
};

#endif //_H_Scene_H_
