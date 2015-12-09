#ifndef VIEWER_H
#define VIEWER_H

#include "Types.h"
#include "Camera.h"
#include "Scene.h"

#include <qevent.h>

#include <gl/glew.h>
//#include <GLFW/glfw3.h>
#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"

#include <vector>

// Forward declarations
class Camera;
class Scene; 
class ViewerState;
class ConsoleListener;

class Viewer
{
	static const int FRAME_PER_SECOND = 24;

	//UI
public:
	void setWindowDimension(int width, int height);

	void handleMousePressEvent(QMouseEvent* e);
	void handleMouseReleaseEvent(QMouseEvent* e);
	void handleMouseMoveEvent(QMouseEvent* e);

	void handleKeyPressEvent(QKeyEvent* e);
	void handleKeyReleaseEvent(QKeyEvent* e);
public : 

	// Ctors/Dtors
	Viewer(int width, int height);
	~Viewer();

	void loop();

	// Getters
	glm::mat4 getViewMatrix(){ return getCurrentScene()->getViewMatrix(); }
	glm::mat4 getProjectionMatrix(){ return m_projectionMatrix; }
	Camera* getCamera(){ return m_camera; }
	Scene*  getCurrentScene() { return m_currentScene; }
	std::vector<std::string> getSceneObjectsName();

	void setDrawLight(bool val) { m_currentScene->drawAllLights(val); }

	//interface between consoleListener and Scene
	void loadModel(const ModelImportOptions* options);
	void removeSelectedObject();
	void removeBackground(const Vec3& color);

	ObjectInformation objectBasicInformation(const std::string& objectName);

private : 


	// Methods
	void createWindow(); 
	void moveCameraBetweenFrame(double timeBetweenFrame);
	void moveCamera(double xoffset, double yoffset);
	void setupViewport(); 

	// Instance
	static Viewer* m_instance; 

	// Variables
	// -- State
	ViewerState* m_state; // HACK : This is not a proper Object Oriented State and it Operations should be handled by classes implementing different states...

	// -- Display variables
	Camera* m_camera; 

	GLboolean m_viewingIsOver;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix; 

	std::vector<Scene*> m_scenes;
	Scene* m_currentScene; 

	// -- Time and general computation variables
	GLfloat m_deltaTime;
	GLfloat m_lastFrameTime;

	Clock::time_point m_lastTime;
	int m_currentFrame;
};

#endif

