#include "IG1App.h"

#include <iostream>

using namespace std;

// static single instance (singleton pattern)
IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

// Print OpenGL errors and warnings
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar* message, const void* userParam)
{
	const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
		? "\x1b[31m[ERROR]\x1b[0m "
		: "\x1b[33m[WARNING]\x1b[0m ";
	cout << prefix << message << endl;
}

void
IG1App::close()
{
	cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(mWindow, true); // stops main loop
}

void
IG1App::run() // enters the main event processing loop
{
	if (mWindow == 0) // if not intilialized
		init();

	// IG1App main loop
	while (!glfwWindowShouldClose(mWindow)) {
		// Redisplay the window if needed
		if (mNeedsRedisplay) {
			display();
			mNeedsRedisplay = false;
		}

		// Update the objects of the scene
		if ((mUpdateEnabled || mUpdate || mUpdate2) && glfwGetTime() > mNextUpdate) {
			double time = glfwGetTime();
			if (m2Escenas) {
				if (mUpdate) {
					mScenes[4]->update();
				}
				if (mUpdate2) {
					mScenes[2]->update();
				}
			}
			else if (mUpdateEnabled) {
				mScenes[mCurrentScene]->update();

			}
			mNextUpdate = time + FRAME_DURATION;
			mNeedsRedisplay = true;
		}
		if (mUpdateEnabled || mUpdate || mUpdate2)
			glfwWaitEventsTimeout(mNextUpdate - glfwGetTime());
		else{
			// Stop and wait for new events
			glfwWaitEvents(); 
		}
		
	}

	destroy();
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();
	// create the scene after creating the context
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH);
	mCamera = new Camera(mViewPort);
	mScenes.push_back(new Scene0);
	mScenes.push_back(new Scene1);
	mScenes.push_back(new Scene2);
	mScenes.push_back(new Scene3);
	mScenes.push_back(new Scene4);
	mScenes.push_back(new Scene5);
	mScenes.push_back(new Scene6);
	mScenes.push_back(new Scene7);
	mScenes.push_back(new Scene8);
	mScenes.push_back(new Scene9);

	mCamera->set2D();
	for (int i = 0; i <= 9; i++) {
		mScenes[i]->init();
	}
	mScenes[0]->load();
	
	/*
	mScenes[0]->init();
	mScenes[0]->load();
	mScenes[1]->init();
	mScenes[2]->init();
	mScenes[3]->init();
	mScenes[4]->init();
	mScenes[5]->init();
	mScenes[6]->init();
	mScenes[7]->init();
	mScenes[8]->init();
	mScenes[9]->init();
	*/

	//se deben tener en mCameras tantas como el tamaño de mScenes +1
	for (int i = 0; i < mScenes.size() + 1; i++) {
		mCameras.push_back(new Camera(mViewPort));
		mCameras[i]->set2D();
	}

	/*mCameras.push_back(new Camera(mViewPort));
	mCameras.push_back(new Camera(mViewPort));
	mCameras.push_back(new Camera(mViewPort));
	mCameras.push_back(new Camera(mViewPort));
	mCameras.push_back(new Camera(mViewPort));
	mCameras.push_back(new Camera(mViewPort));

	mCameras[0]->set2D();
	mCameras[1]->set2D();
	mCameras[2]->set2D();
	mCameras[3]->set2D();
	mCameras[4]->set2D();
	mCameras[5]->set2D();
	*/


}

void
IG1App::iniWinOpenGL()
{ // Initialization
	cout << "Starting GLFW...\n";
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // enable on macOS

	// Create window with its associated OpenGL context, return window's pointer
	mWindow = glfwCreateWindow(mWinW, mWinH, "IG1App", nullptr, nullptr);

	if (mWindow == nullptr) {
		glfwTerminate();
		throw std::logic_error("cannot create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);

	// Initialize OpenGL extension library
	if (GLenum err = glewInit(); err != GLEW_OK) {
		glfwTerminate();
		throw std::logic_error("Error while loading extensions: "s +
		                       reinterpret_cast<const char*>(glewGetErrorString(err)));
	}

	// Callback registration
	glfwSetWindowSizeCallback(mWindow, s_resize);
	glfwSetCharCallback(mWindow, s_key);
	glfwSetKeyCallback(mWindow, s_specialkey);
	glfwSetWindowRefreshCallback(mWindow, s_display);

	glfwSetMouseButtonCallback(mWindow, s_mouse);
	glfwSetCursorPosCallback(mWindow, s_motion);
	glfwSetScrollCallback(mWindow, s_mouseWheel);

	// Error message callback (all messages)
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	glDebugMessageCallback(debugCallback, nullptr);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void
IG1App::destroy()
{ // release memory and resources
	for (Scene* scene : mScenes)
		delete scene;
	mScenes.clear();

	for (Camera* camera : mCameras)
		delete camera;
	mCameras.clear();

	delete mCamera;
	mCamera = nullptr;
	delete mViewPort;
	mViewPort = nullptr;

	glfwTerminate();
}

void
IG1App::display() const
{ // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer	

	//Ap49, 2 vistas de la misma escena
	if (m2Vistas && !m2Escenas) {
		//Vista izquierda, 3D
		Viewport auxVP = *mViewPort;
		mViewPort->setSize(mWinW / 2, mWinH);
		mCameras[mCurrentCamera]->setSize(mViewPort->width(), mViewPort->height());
		mViewPort->setPos(0, 0);
		if (mCameras[mCurrentCamera]->viewType != 2) {
			mCameras[mCurrentCamera]->set3D();
		}
		if (!mbOrto3) {
			mCameras[mCurrentCamera]->changePrj();
		}
		mScenes[mCurrentScene]->render(*mCameras[mCurrentCamera]);
		if (!mbOrto3) {
			mCameras[mCurrentCamera]->changePrj();
		}

		//Vista derecha, cenital
		mCameras[mCurrentCamera+1]->setSize(mViewPort->width(), mViewPort->height());
		mViewPort->setPos(mWinW / 2, 0);
		if (mCameras[mCurrentCamera+1]->viewType != 3) {
			mCameras[mCurrentCamera+1]->setCenital();
		}
		if (!mbOrto4) {
			mCameras[mCurrentCamera+1]->changePrj();
		}
		mScenes[mCurrentScene]->render(*mCameras[mCurrentCamera+1]);
		if (!mbOrto4) {
			mCameras[mCurrentCamera+1]->changePrj();
		}

		*mViewPort = auxVP;

	}
	//Ap 52 y 53, 2 vistas de escenas distintas (la 4 a la izquierda y la 2 a la derecha)
	else if (!m2Vistas && m2Escenas) {
		//Escena izquierda
		Viewport auxVP = *mViewPort;
		mViewPort->setSize(mWinW / 2, mWinH);
		mCameras[4]->setSize(mViewPort->width(), mViewPort->height());
		mViewPort->setPos(0, 0);
		if (mCameras[4]->viewType != 2) {
			mCameras[4]->set3D();
		}
		if (mCurrentScene != 4) {
			mScenes[4]->load();
		}
		if (!mbOrto1) {
			mCameras[4]->changePrj();
		}
		mScenes[4]->render(*mCameras[4]);
		if (!mbOrto1) {
			mCameras[4]->changePrj();
		}
		//Escena derecha
		Viewport *secondViewport= mViewPort;
		secondViewport->setSize(mWinW / 2, mWinH);
		mCameras[2]->setSize(mViewPort->width(), mViewPort->height());
		secondViewport->setPos(mWinW / 2, 0);
		if (mCameras[2]->viewType != 1) {
			mCameras[2]->set2D();
		}
		if (mCurrentScene != 2) {
			mScenes[2]->load();
		}
		if (!mbOrto2) {
			mCameras[2]->changePrj();
		}
		mScenes[2]->render(*mCameras[2]);
		if (!mbOrto2) {
			mCameras[2]->changePrj();
		}
		if (mCurrentScene != 4) {
			mScenes[4]->unload();
		}
		if (mCurrentScene != 2) {
			mScenes[2]->unload();
		}
		*mViewPort = auxVP;
	}
	else {
		mScenes[mCurrentScene]->render(*mCamera); // uploads the viewport and camera to the GPU
	}


	glfwSwapBuffers(mWindow); // swaps the front and back buffer


}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

void
IG1App::key(unsigned int key)
{
	Texture* textura = new Texture();

	bool need_redisplay = true;

	switch (key) {
		case '+':
			mCamera->setScale(+0.01); // zoom in  (increases the scale)
			break;
		case '-':
			mCamera->setScale(-0.01); // zoom out (decreases the scale)
			break;
		case 'l':
			mCamera->set3D();
			break;
		case 'o':
			mCamera->set2D();
			break;
			//update the scene
		case 'u':
			if (m2Escenas) {
				if (mMouseCoord.x < mWinW / 2) {
					mUpdate = !mUpdate;
				}
				else {
					mUpdate2 = !mUpdate2;
				}
			}
			else {
				mUpdateEnabled = !mUpdateEnabled;

			}
			break;
			//Ap 36 -> capture the scene
		case 'F':
			//creamos textura
			textura->loadColorBuffer(mWinW, mWinH);
			//la guardamos en imagen .bpm
			textura->download("../assets/images/captura.bmp");
			break;
/*case 'a':
			mCamera->moveLR(1);
			break;
		case 'd':
			mCamera->moveLR(-1);
			break;
		case 'w':
			mCamera->moveUD(1);
			break;
		case 's':
			mCamera->moveUD(-1);
			break;
		case 'W':
			mCamera->moveFB(1);
			break;*/
		case 'S':
			mCamera->moveFB(-1);
			break;
		case 'p':
			//Cambia la perspectiva
			//en cada escena
			if (m2Escenas) {
				if(mMouseCoord.x < mWinW / 2) {
					mbOrto1 = !mbOrto1;
				}
				else {
					mbOrto2 = !mbOrto2;
				}
			}
			//en cada vista
			else if (m2Vistas) {
				if (mMouseCoord.x < mWinW / 2) {
					mbOrto3 = !mbOrto3;
				}
				else {
					mbOrto4 = !mbOrto4;
				}
			}
			else{
				mCamera->changePrj();
			}
			
			break;
		/*
		case 't':
			mCamera->orbit(1,0);
			break;
		case 'T':
			mCamera->setCenital();
			break;
		*/
		case 'k':
			//Cambia a 2 vistas y borra la flag de 2 escenas si está activa
			m2Vistas = !m2Vistas; 
			if (m2Escenas) {
				m2Escenas = !m2Escenas;
			}
			//limpia los estados de las cámaras
			for (int i = 0; i < mCameras.size(); i++) {
				mCameras[i]->viewType = 0;
			}
			break;
		case 'K':
			//Cambia a 2 escenas y borra la flag de 2 vistas si está activa
			m2Escenas = !m2Escenas;
			if (m2Vistas) {
				m2Vistas = !m2Vistas;
			}
			//limpia los estados de las cámaras
			for (int i = 0; i < mCameras.size(); i++) {
				mCameras[i]->viewType = 0;
			}
			break;
		case 'N':
			//Activa las normales de las entidades ColorMaterialEntity de la escena actual
			//mScenes[mCurrentScene]->setNormals();
			ColorMaterialEntity::toggleShowNormals();
			break;
		case 'r':
			mScenes[mCurrentScene]->dirLight->setEnabled(!mScenes[mCurrentScene]->dirLight->enabled());
		default:
			if (key >= '0' && key <= '9' && !changeScene(key - '0'))
				cout << "[NOTE] There is no scene " << char(key) << ".\n";
			else
				need_redisplay = mScenes[mCurrentScene]->handleKey(key);
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
	delete textura;

}

void
IG1App::specialkey(int key, int scancode, int action, int mods)
{
	// Only interested in press events
	if (action == GLFW_RELEASE)
		return;

	bool need_redisplay = true;

	// Handle keyboard input
	// (key reference: https://www.glfw.org/docs/3.4/group__keys.html)
	switch (key) {
		case GLFW_KEY_ESCAPE:                     // Escape key
			glfwSetWindowShouldClose(mWindow, true); // stops main loop
			break;
		case GLFW_KEY_RIGHT:
			if (mods & GLFW_MOD_CONTROL)
				//mCamera->pitch(-1); // rotates -1 on the X axis
				mCamera->rollReal(-1);
			else
				//mCamera->pitch(1); // rotates 1 on the X axis
				mCamera->yawReal(1);
			break;
		case GLFW_KEY_LEFT:
			if (mods & GLFW_MOD_CONTROL)
				//mCamera->yaw(1); // rotates 1 on the Y axis
				mCamera->rollReal(1);
			else
				//mCamera->yaw(-1); // rotate -1 on the Y axis
				mCamera->yawReal(-1);
			break;
		case GLFW_KEY_UP:
			//mCamera->roll(1); // rotates 1 on the Z axis
			mCamera->pitchReal(2);
			break;
		case GLFW_KEY_DOWN:
			//mCamera->roll(-1); // rotates -1 on the Z axis
			mCamera->pitchReal(-2);
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			mouse(key, action, mods);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouse(key, action, mods);
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			mouse(key, action, mods);
			break;
		default:
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

bool
IG1App::changeScene(size_t sceneNr)
{
	// Check whether the scene exists
	if (sceneNr >= mScenes.size())
		return false;

	// Change only if a different scene
	if (sceneNr != mCurrentScene) {
		mScenes[mCurrentScene]->unload();
		mCurrentScene = sceneNr;
		mCurrentCamera = sceneNr;
		mScenes[mCurrentScene]->load();

		switch (mCurrentScene) {
			case 2:
				FRAME_DURATION = 0.08;
				break;
			case 7:
				if (!m2Escenas and !m2Vistas) {
					glClearColor(0.0, 0.0, 0.0, 1.0); // New background color: negro
					glEnable(GL_DEPTH_TEST);
				}
				break;
			case 8:
				if (!m2Escenas and !m2Vistas) {
					glClearColor(0.0, 0.0, 0.0, 1.0); // New background color: negro
					glEnable(GL_DEPTH_TEST);
				}
				break;
			default:
				glClearColor(0.6, 0.7, 0.8, 1.0); // New background color (alpha=1 -> opaque)
				glEnable(GL_DEPTH_TEST);
				FRAME_DURATION = 0.04;
				break;
		}

		//Para que se refresque al cambiar de escena
		mNeedsRedisplay = true;

	}

	return true;
}

//Ap51
//Captura de posición y botón del ratón pulsado
void 
IG1App::mouse(int button, int state, int mods) {
	glfwGetCursorPos(mWindow, &mMouseCoord.x, &mMouseCoord.y);

	//conversión de la variable mMouseCoord.y
	int height;
	glfwGetWindowSize(mWindow, nullptr, &height);
	mMouseCoord.y = height - mMouseCoord.y;
	
	//Se distingue entre pulsar el ratón y despulsarlo para los eventos de movimiento
	if (state) {
		mMouseButt = button;
	}
	else {
		mMouseButt = -1;
	}
	//printf("%d %f %f %d\n", mMouseButt, mMouseCoord.x, mMouseCoord.y, state);
}

void
IG1App::motion_aux(glm::dvec2 mp, int cam) {
	if (mMouseButt == 0) {
		mCameras[cam]->orbit(mp.x * 0.05, -mp.y);
	}
	else if (mMouseButt == 1) {
		mCameras[cam]->moveUD(-mp.y * 0.25);
		mCameras[cam]->moveLR(mp.x * 0.25);
	}
}

//Movimientos de la cámara con el ratón, mientras se mantienen pulsados, el botón izquierdo (0) orbita la cámara y el derecho (1) la desplaza
void
IG1App::motion(double x, double y) {
	glm::dvec2 input = { x, y };
	glm::dvec2 mp = mMouseCoord - input;
	mMouseCoord = input;

	//Movimiento para 2 escenas, ap 53
	if (m2Escenas) {
		if (mMouseCoord.x < mWinW / 2) {
			motion_aux(mp, 4);
		}
		else {
			motion_aux(mp, 2);
		}
	}
	//Movimiento para 2 vistas, la orbita no está pensada para usarse en la vista cenital, ap 53
	else if (m2Vistas) {
		if (mMouseCoord.x < mWinW / 2) {
			motion_aux(mp, mCurrentCamera);
		}
		else {
			motion_aux(mp, mCurrentCamera + 1);
		}
	}
	//Movimiento básico, ap 51
	else {
		if (mMouseButt == 0) {
			mCamera->orbit(mp.x * 0.05, -mp.y);
		}
		else if (mMouseButt == 1) {
			mCamera->moveUD(-mp.y * 0.25);
			mCamera->moveLR(mp.x * 0.25);
		}
	}
	mNeedsRedisplay = true;
}

void
IG1App::aux_mouseWheel(double dy, int cam) {
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL)) {
		mCameras[cam]->setScale(dy * 0.05);
	}
	else {
		mCameras[cam]->moveFB(dy * 5);
	}
}

//Movimiento de la rueda, manteniendo pulsado CONTROL escala la escena, sin pulsar CONTROL mueve el FB de la cámara
void
IG1App::mouseWheel(double dx, double dy) {
	//printf("%f %f %d %d\n", dx, dy, glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL), glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL));
	//Movimiento para 2 escenas, ap 53
	if (m2Escenas) {
		if (mMouseCoord.x < mWinW / 2) {
			aux_mouseWheel(dy, 4);
		}
		else {
			aux_mouseWheel(dy, 2);
		}
	}
	//Movimiento para 2 vistas, ap 53
	else if (m2Vistas) {
		if (mMouseCoord.x < mWinW / 2) {
			aux_mouseWheel(dy, mCurrentCamera);
		}
		else {
			aux_mouseWheel(dy, mCurrentCamera+1);
		}
	}
	//Movimiento básico, ap 51
	else {
		if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL)) {
			mCamera->setScale(dy * 0.05);
		}
		else {
			mCamera->moveFB(dy * 5);
		}
	}
	mNeedsRedisplay = true;
}