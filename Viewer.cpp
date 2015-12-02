
#include "Viewer.h"
#include "ViewerState.h"
#include "Camera.h"
#include "Scene.h"
#include "DefaultTestLevel.h"
#include "SSSSTestLevel.h"
#include "FrameBuffer.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "ModelContainer.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>


void Viewer::setWindowDimension(int width, int height)
{
	m_state->setWindowDimension(width, height);
	glViewport(0, 0, width, height);
}

void Viewer::handleMousePressEvent(QMouseEvent* e)
{
	m_state->handleMousePressEvent(e);
}

void Viewer::handleMouseReleaseEvent(QMouseEvent* e)
{
	m_state->handleMouseReleasedEvent(e);
}

void Viewer::handleMouseMoveEvent(QMouseEvent* e)
{
	m_state->handleMouseMoveEvent(e);
}

void Viewer::handleKeyPressEvent(QKeyEvent* e)
{
	m_state->handleKeyPressEvent(e);
}

void Viewer::handleKeyReleaseEvent(QKeyEvent* e)
{
	m_state->handleKeyReleaseEvent(e);
}

//Viewer::mouse_button_callback_impl(int button, int action, int mods)
//{
	//m_state->handleMouseClick(window, button, action);
//}
//
//void Viewer::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
//{
//	m_camera->zoom(yoffset);
//}

// Viewer class
// According to how the viewerstate is currently made, shouldn't this be in viewerstate?
void Viewer::moveCameraBetweenFrame(double deltaTime)
{
	if (m_state->getInteractionMode() != LOCKED)
	{
		// Camera controls
		if (m_state->getKeys()[Qt::Key::Key_W])
			m_camera->move(Camera::CameraDirection::eForward, deltaTime);
		if (m_state->getKeys()[Qt::Key::Key_S])
			m_camera->move(Camera::CameraDirection::eBackward, deltaTime);
		if (m_state->getKeys()[Qt::Key::Key_A])
			m_camera->move(Camera::CameraDirection::eLeft, deltaTime);
		if (m_state->getKeys()[Qt::Key::Key_D])
			m_camera->move(Camera::CameraDirection::eRight, deltaTime);
		if (m_state->getKeys()[Qt::Key::Key_Z])
			m_camera->zoom(0.001);
		if (m_state->getKeys()[Qt::Key::Key_X])
			m_camera->zoom(-0.001);
	}
}

//void Viewer::setCallbacks()
//{
//	glfwSetErrorCallback(error_callback);
//	glfwSetKeyCallback(m_window, key_callback);
//	glfwSetWindowSizeCallback(m_window, window_size_callback);
//	glfwSetCursorPosCallback(m_window, mouse_callback);
//	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
//}

/*
* @brief : Manage window's creation and getting important monitor settings. 
*/
void Viewer::createWindow()
{
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor(); 
	//const GLFWvidmode* mode = glfwGetVideoMode(monitor); 

	//// Window's size
	//m_width = mode->width;
	//m_height = mode->height; 

	//// Window's hints
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) //Mac OSX

	//m_window = glfwCreateWindow(m_width, m_height, "Opengl4 Viewer", nullptr, nullptr);
	//if (!m_window)
	//{
	//	throw std::runtime_error("Failed to create a GLFW window");
	//	glfwTerminate();
	//	exit(EXIT_FAILURE);
	//}

	//glfwMakeContextCurrent(m_window);
	FrameBuffer::setDimensions(m_state->getWidth(), m_state->getHeight());
}

void Viewer::setupViewport()
{
	// Create Viewport
	glViewport(0, 0, m_state->getWidth(), m_state->getHeight());

	// Set GL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
}

/*
* @brief : Acts as the main in a non-object oriented OpenGL program.  
*/
Viewer::Viewer(int width, int height)
	: m_camera(nullptr)
	, m_viewingIsOver(false)
	, m_currentScene(nullptr)
{
	m_state = new ViewerState(this, width, height);
	
	createWindow();

	setupViewport(); 
	

	m_camera = new Camera(&glm::vec3(0.0f, 1.0f, 0.0f), &glm::vec3(0.0f, 0.0f, 50.0f), &glm::vec3(0, 0, 0));
	m_scenes.push_back(new DefaultTestLevel()); 
	//m_listener = new ConsoleListener();

	//start animation once scene is ready
	m_currentFrame = 0;

	auto sceneIterator = m_scenes.begin();
	assert(sceneIterator != m_scenes.end(), "No scene to render. Please initialize a scene.");
	m_currentScene = *sceneIterator;
	m_currentScene->Initialize();

	m_lastTime = Clock::now();
}

Viewer::~Viewer()
{
	//glfwDestroyWindow(m_window);

	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr; 
	}

	for (int i = 0; i < m_scenes.size(); ++i)
	{
		delete m_scenes[i];
		m_scenes[i] = nullptr;
	}
	
	delete m_state; m_state = nullptr; 

}

void Viewer::loop()
{
	// Initialize first level
	auto sceneIterator = m_scenes.begin(); 
	assert(sceneIterator != m_scenes.end(), "No scene to render. Please initialize a scene.");

	m_currentScene = *sceneIterator;
		
	// Verify if level is still active
	if (m_currentScene->getLevelIsDone())
	{
		m_currentScene->sceneTearDown();
		++sceneIterator;

		if (sceneIterator != m_scenes.end())
		{
			m_currentScene = *sceneIterator;
			m_currentScene->Initialize();
		}
		else
		{
			m_viewingIsOver = true; 
		}
	}

	GLfloat width = (GLfloat)m_state->getWidth();
	GLfloat height = (GLfloat)m_state->getHeight();
	glm::mat4 projection = glm::perspective(m_camera->getZoomLevel(), width / height, 0.01f, 200.0f);
	//glm::mat4 projection = glm::ortho(-960, 960, -590, 590);
	m_currentScene->setProjectionMatrix(projection);

	// Check and call events
	moveCameraBetweenFrame(0.1); //TODO

	// Update Matrix
	m_viewMatrix = m_camera->GetViewMatrix();

	// Level drawing
	m_currentScene->setViewMatrix(m_viewMatrix);

	Clock::time_point now = Clock::now();
	std::chrono::duration<double, std::ratio<1, 1000>> numberOfFrameSinceLast = now - m_lastTime;
	double numFrame = (numberOfFrameSinceLast.count() / (1000 / 24));
	if (numFrame > 1)
	{
		m_currentFrame += numFrame;
		m_lastTime = now;
	}

	m_currentScene->preDraw();
	m_currentScene->draw(m_currentFrame);
	m_currentScene->postDraw();
}

void Viewer::loadModel(const ModelImportOptions* options)
{
	m_currentScene->loadModel(options);
}

void Viewer::removeSelectedObject()
{
	Object* objToBeRemoved = m_state->getLastSelectedObject();
	m_currentScene->removeObject(objToBeRemoved);
}

void Viewer::removeBackground(const Vec3& color)
{
	ModelContainer* lastSelectedObject = dynamic_cast<ModelContainer*>(m_state->getLastSelectedObject());
	
	if (lastSelectedObject)
		m_currentScene->addBackgroundToBeRemoved(lastSelectedObject, color);
}

void Viewer::moveCamera(double xoffset, double yoffset)
{
	m_camera->rotate(xoffset, yoffset);
}

std::vector<std::string> Viewer::getSceneObjectsName()
{
	std::vector<std::string> objectNames;
	std::vector<Object*> objs = getCurrentScene()->getObjects();

	for each (Object* obj in objs)
	{
		objectNames.push_back(obj->getDisplayName());
	}

	return objectNames;
}
