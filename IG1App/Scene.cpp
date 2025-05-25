#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <typeinfo>

using namespace glm;

void 
Scene::init()
{
	setGL(); // OpenGL settings
	//apartado 74
	dirLight->setAmb(glm::vec4(.25, .25, .25, 1));
	dirLight->setDiff(glm::vec4(.6, .6, .6, 1));
	dirLight->setSpec(glm::vec4(0, 0.2, 0, 1));
	dirLight->setEnabled(true);
	dirLight->setDirection(glm::vec4(-1.0, -1.0, -1.0, 0.0));
	//dirLight->setDirection(glm::vec4(glm::normalize(cam.viewMat() * glm::vec4(-1.0, -1.0, -1.0, 0.0))));
	gLights.push_back(dirLight);
}

void
Scene0::init()
{
	Scene::init();
	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(400.0));
	//Heptagono negro
	//gObjects.push_back(new RegularPolygon(7, 200.0, glm::dvec4(0.0, 0.0, 0.0, 1.0)));

	//Triangulo verde
	//gObjects.push_back(new RegularPolygon(3, 200.0, glm::dvec4(0.0, 1.0, 0.0, 1.0)));
	//Circunferencia magenta
	//gObjects.push_back(new RegularPolygon(360, 200.0, glm::dvec4(1.0, 0.0, 1.0, 1.0)));

	//RGB Triangle
	//gObjects.push_back(new RGBTriangle(200));
	//RGB Rectangle
	//gObjects.push_back(new RGBRectangle(500, 200));

	//apartado 10 
	//gObjects.push_back(new RegularPolygon(360, 200.0, glm::dvec4(1.0, 1.0, 1.0, 1.0)));
	//gObjects.push_back(new RGBTriangle(40));
	//gObjects.push_back(new RGBRectangle(400, 200));
	
	//Cubo Apartado 15
	//gObjects.push_back(new Cube(250));
	

	//ap71
	Sphere* tatooine = new Sphere(110, 100, 200);
	tatooine->setColor(glm::dvec4(1.0, 233.0 / 255.0, 0.0, 1.0));
	tatooine->move(glm::vec3(200.0f, 0.0f, 0.0f));
	gObjects.push_back(tatooine);

	Sphere* tatooine2 = new Sphere(110, 100, 200);
	Material oro =  Material();
	oro.setGold();
	tatooine2->setMaterial(oro);
	tatooine2->move(glm::vec3(0.0f, 0.0f, 200.0f));
	gObjects.push_back(tatooine2);

	
}

void
Scene1::init()
{
	//setGL(); // OpenGL settings

	
	gObjects.push_back(new RGBAxes(400.0));
	//Triangulo cian
	//gObjects.push_back(new RegularPolygon(3, 200.0, glm::dvec4(0.0, 1.0, 1.0, 1.0)));
	//Circunferencia magenta
//	gObjects.push_back(new RegularPolygon(360, 200.0, glm::dvec4(1.0, 0.0, 1.0, 1.0)));
	


	WallWithTexCor* terrain = new WallWithTexCor(2000.0, 2000.0, "../assets/images/terrain.jpg", 2);
	terrain->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	terrain->move(glm::vec3(0, -1.0, 0));
	gObjects.push_back(terrain);


	Farola* farola = new Farola();
	farola->move(glm::vec3(100, 0, 300));
	gObjects.push_back(farola);

	Farola* farola2 = new Farola();
	farola2->move(glm::vec3(-100, 0, 300));
	gObjects.push_back(farola2);
	

	Mesa* mesa = new Mesa();
	mesa->move(glm::vec3(100, 15, 100));
	gObjects.push_back(mesa);


	lampara->move(glm::vec3(100, 35, 100));
	gObjects.push_back(lampara);

	gObjects.push_back(persona);
	persona->scale(glm::vec3(0.2, 0.2, 0.2));
	persona->move(glm::vec3(0, 34, 0));


	
	door->move(glm::vec3(30, 51.0, -200));
	gObjects.push_back(door);

	Forniture* forniture = new Forniture();
	forniture->move(glm::vec3(-100, 10, 100));
	gObjects.push_back(forniture);

	Sky* sky = new Sky(2000.0);


	gObjects.push_back(sky);

	Habitacion* habitacion = new Habitacion(400.0, 160);
	habitacion->move(glm::vec3(0, 1, 0));
	gObjects.push_back(habitacion);

	gCoObjects.push_back(new CoalitionEntity({ mesa, 60 }));
	gCoObjects.push_back(new CoalitionEntity({ forniture, 80 }));
	gCoObjects.push_back(new CoalitionEntity({ habitacion, 400 }));

}

bool 
Scene1::getCoalitionWithPersona(Abs_Entity* el, int dis, bool isDoor) {
	vec3 elPos = glm::vec3(el->modelMat()[3]);
	vec3 personaPos = glm::vec3(persona->modelMat()[3]);
	int halfDis = dis * 0.5;
	int support = 0;
	if (isDoor) {
		support = -30;
	}
	printf("currentDir: %d\n", persona->currentDir);
	if (persona->currentDir == 1) {
		// mira a -z
		return (elPos.z < personaPos.z && elPos.z > personaPos.z - dis
			&& elPos.x + support < personaPos.x + halfDis && elPos.x + support > personaPos.x - halfDis
			);
	}
	else if (persona->currentDir == 2) {
		// mira a -x
		if (isDoor) {
			return false;
		}
		return (elPos.x < personaPos.x && elPos.x > personaPos.x - dis
			&& elPos.z < personaPos.z + halfDis && elPos.z > personaPos.z - halfDis
			);
	}
	else if (persona->currentDir == 3) {
		if (isDoor) {
			return false;
		}
		//mira a +z
		return (elPos.z > personaPos.z && elPos.z < personaPos.z + dis
			&& elPos.x < personaPos.x + halfDis && elPos.x > personaPos.x - halfDis
			);
	}
	else if (persona->currentDir == 4) {
	
		if (isDoor) {
			return false;
		}
		//mira a +x		
		return (elPos.x > personaPos.x && elPos.x < personaPos.x + dis
			&& elPos.z < personaPos.z + halfDis && elPos.z > personaPos.z - halfDis
			);
	}
	return false;
}


bool
Scene1::handleKey(unsigned int key) {
	switch (key) {
	case 'e':
		persona->setTouch(true);
		if (getCoalitionWithPersona(lampara, 80)) {
			lampara->changeFoco();
			return true;
		}
		else if (getCoalitionWithPersona(door, 50, true)) {
			door->setDoorOpen(!door->doorOpen);
			return true;
		}
		return true;
	default:
		return false;
		break;
	}
}

bool
Scene1::handleSpecialKey(int key, int scancode, int action, int mods) {
	
	if (!persona->touch) {
		switch (key) {
		case 87: // tecla W
			persona->walk(1, action, gCoObjects, door->doorOpen);
			return true;
			break;
		case 65: // tecla A
			persona->walk(2, action, gCoObjects, door->doorOpen);
			return true;
			break;
		case 83: // tecla S
			persona->walk(3, action, gCoObjects, door->doorOpen);

			return true;
			break;
		case 68: // tecla D
			persona->walk(4, action, gCoObjects, door->doorOpen);

			return true;
			break;
		default:
			return false;
			break;
		}
	}
	else {
		return false;
	}
}

void
Scene2::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));
	//apartado 10 
	// declarar triangulo en una variable
	RGBTriangle* t = new RGBTriangle(40);
	// setear la posicion del triangulo
	t->setPos(glm::vec3(200, 0, 0));
	gObjects.push_back(t);
	gObjects.push_back(new RegularPolygon(360, 200.0, glm::dvec4(1.0, 1.0, 1.0, 1.0)));
	gObjects.push_back(new RGBRectangle(400, 200));

}

void
Scene3::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));
	//cubo del apartado 15
	//gObjects.push_back(new Cube(250));
	//cubo del apartado 16/17
	GLdouble lenght = 250;
	GLdouble l = lenght / 2;
	RGBCube* c = new RGBCube(lenght);
	c->setPos(glm::vec3(l, l, -l));
	gObjects.push_back(c);

}

void
Scene4::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));

	//Ground* ground = new Ground(300.0);  //Ap20
	
	//Ap21, suelo con baldosas
	Ground2* ground = new Ground2(400.0, 400.0, 4,4); 
	//Rotar el Ground para que descanse sobre el plano Y = 0
	ground->rotate();
	gObjects.push_back(ground);
	
	//Ap22-25, caja sin tapas con texturas distintas por fuera y dentro
	//BoxOutline* box = new BoxOutline(250.0);
	BoxOutline* boxOut = new BoxOutline(50.0);
	boxOut->rearrange(glm::vec3(100, 30, 100));
	gObjects.push_back(boxOut);
	
	//Ap 30-31, caja con tapa que se abre y cierra
	Box* box = new Box(50.0);
	box->rearrange(glm::vec3(-100, 30, -100));
	gObjects.push_back(box);
	
	//Ap26-29, estrella doble de 8 puntas que rota
	//Star3D* star = new Star3D(180, 8, 175);
	Star3D* star = new Star3D(36, 8, 35);
	star->rearrange(glm::vec3(100, 110, 100));
	gObjects.push_back(star);

	//Ap35, foto de la escena
	Photo* photo = new Photo(75);
	photo->rotate(glm::vec3(0, 10, 0));
	gObjects.push_back(photo);

	//Ap33, hierba
	Grass* grass = new Grass(50.0);
	grass->rearrange(glm::vec3(-120, 25, 120));
	gObjects.push_back(grass);
	
	//Ap32, caja sin tapas con textura translúcida
	//GlassParapet* glass = new GlassParapet(250.0);
	GlassParapet* glass = new GlassParapet(420.0, 105.0);
	gObjects.push_back(glass);

}

void
Scene5::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));

	Torus* torus = new Torus(100, 50);
	torus->setColor(glm::dvec4(0.0, 1.0, 0.0, 1.0));
	gObjects.push_back(torus);
}

void
Scene6::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));

	IndexedBox* box = new IndexedBox(200);
	box->setColor(glm::dvec4(0.0, 1.0, 0.0, 1.0));
	gObjects.push_back(box);
}

//Apartado 66 -> caza TIE
void
Scene7::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));

	
	/*Sphere* sphere = new Sphere(100, 360, 360);
	sphere->setColor(glm::dvec4(0.0, 1.0, 0.0, 1.0));

	Disk* disk = new Disk(100, 200, 200, 360);
	disk->setColor(glm::dvec4(0.0, 0.0, 1.0, 1.0));

	Cone* cone = new Cone(160, 10, 20, 200, 200);
	cone->setColor(glm::dvec4(0.0, 0.0, 1.0, 1.0));
	
	gObjects.push_back(disk);
	gObjects.push_back(sphere);
	gObjects.push_back(cone);
	*/
	
	AdvancedTIE* adv_tie = new AdvancedTIE();
	gObjects.push_back(adv_tie);

}

//Apartados 67-68 -> caza TIE en Tatooine
void
Scene8::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));

	//apartado 75
	posLight->setAmb(glm::vec4(.25, .25, .25, 1));
	posLight->setDiff(glm::vec4(1.0, 1.0, 1.0, 1));
	posLight->setSpec(glm::vec4(0, 0.2, 0, 1));
	posLight->setEnabled(true);
	posLight->setPosition(glm::vec3(1500.0, 0.0, 0.0));
	gLights.push_back(posLight);

	//apartado 76
	spotLight->setAmb(glm::vec4(.25, .25, .25, 1));
	spotLight->setDiff(glm::vec4(.6, .6, .6, 1));
	spotLight->setSpec(glm::vec4(0, 0.2, 0, 1));
	spotLight->setEnabled(true);
	spotLight->setPosition(glm::vec3(0.0, 0.0, 500.0));
	spotLight->setCutoff(10, 12);
	gLights.push_back(spotLight);

	AdvancedTIE* adv_tie = new AdvancedTIE();
	adv_tie->scale(glm::vec3(0.25f, 0.25f, 0.25f));
	//adv_tie->scale(glm::vec3(0.25f, 0.25f, 0.25f));
	//adv_tie->move(glm::vec3(0.0f, 225.0f, 0.0f));
	//gObjects.push_back(adv_tie);

	//Ap 68
	NodoFicticio* inventedNode = new NodoFicticio();
	inventedNode->addEntity(adv_tie);
	adv_tie->setModelMat(glm::translate(adv_tie->modelMat(), glm::vec3(0.0f, 250.0f, 0.0f)));
	gObjects.push_back(inventedNode);

	//Ap 67
	Sphere* tatooine = new Sphere(200, 100, 200);
	tatooine->setColor(glm::dvec4(1.0, 233.0 / 255.0, 0.0, 1.0));
	gObjects.push_back(tatooine);
}

bool 
Scene8::handleKey(unsigned int key) {

	//printf("key: %c\n", key);
	switch (key) {
		case 'f':
			for (Abs_Entity* el : gObjects) {
				NodoFicticio* node = dynamic_cast<NodoFicticio*>(el);
				if (node)
					node->rotate();
			}
			//gObjects[2]->setModelMat(glm::rotate(gObjects[2]->modelMat(), glm::radians(3.0f), glm::vec3(0, 0, 1)));
			return true;
			break;
		case 'g':
			for (Abs_Entity* el : gObjects) {
				NodoFicticio* node = dynamic_cast<NodoFicticio*>(el);
				if (node)
					node->orbit();

			}
			return true;
			break;
		case 't':
			posLight->setEnabled(!posLight->enabled());
			break;
		case 'y':
			spotLight->setEnabled(!spotLight->enabled());
			break;
		case 'h':
			for (Abs_Entity* el : gObjects) {
				NodoFicticio* node = dynamic_cast<NodoFicticio*>(el);
				if (node)
					for (Abs_Entity* el2 : node->getEntities()) {
						AdvancedTIE* adv_tie = dynamic_cast<AdvancedTIE*>(el2);
						if (adv_tie)
							adv_tie->changeFoco();
					}
			}
			break;
		default:
			return false;
			break;
	}
}

//Apartado 69 -> Granjero
void
Scene9::init()
{
	Scene::init();
	gObjects.push_back(new RGBAxes(400.0));

	Granjero* granjero = new Granjero();
	granjero->rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	gObjects.push_back(granjero);

}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects) {	/*
		auto* objWithTexture = dynamic_cast<EntityWithTexture*>(el);
		if (objWithTexture) {
			delete objWithTexture->mTexture;
			auto* objWithInsideTexture = dynamic_cast<BoxOutline*>(el);
			if (objWithInsideTexture) {
				delete objWithInsideTexture->mInsideTexture;
			}
		}*/
		delete el;
	}

	for (Light* el : gLights)
		delete el;
		


	gObjects.clear();

	for (Texture* el : gTextures)
		delete el;

	gTextures.clear();
}

void
Scene::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void
Scene::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();

	shader->use();
	for (Light* l : gLights)
		l->unload(*shader);
}

void
Scene::setGL()
{
	// OpenGL basic setting
	//glClearColor(1.0, 1.0, 1.0, 1.0); // background color (alpha=1 -> opaque) 
	glClearColor(0.6, 0.7, 0.8, 1.0); // New background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
//	glEnable(GL_TEXTURE_2D);
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
	//glDisable(GL_TEXTURE_2D);
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();
	uploadLights(cam.viewMat());

	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());	
	
}

//Apartado 73
void 
Scene::uploadLights(const glm::dmat4 & mViewMat) const
{
	//Shader* shader2 = Shader::get("light");
///	shader2->use();
	shader->use();
	for (Light* l : gLights){
		l->upload(*shader, mViewMat);
	}
}



void 
Scene::update()
{
	//update gObjects
	for (Abs_Entity* el : gObjects) {
	
		el->update();
	}
}

bool 
Scene::handleKey(unsigned int key) {
	return false;
}

bool
Scene::handleSpecialKey(int key, int scancode, int action, int mods) {
	return false;
}