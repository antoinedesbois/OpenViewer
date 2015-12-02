
#include "ViewerState.h"
#include "Scene.h"
#include "Camera.h"
#include "Object.h"

#include <QMouseEvent>

#include <GL/glew.h>
#include "glm/glm/gtc/matrix_transform.hpp"


//forward declaration
class Camera;
class Scene;
class Viewer;

ViewerState::ViewerState(Viewer* viewer, int width, int height) :
	m_viewingMode(ViewingMode::NORMAL),
	m_interactionMode(InteractionMode::NAVIGATION),
	m_transformationMode(TransformationMode::TRANSLATION),
	m_currentlySelectedObject(nullptr),
	m_height(height),
	m_width(width),
	m_viewer(viewer)
{
	for (int i = 0; i < 1024; ++i)
	{
		m_keys[i] = false;
	}
}

void ViewerState::setWindowDimension(int width, int height)
{
	m_width = width;
	m_height = height;
}

void ViewerState::handleKeyPressEvent(QKeyEvent* e)
{
	//-------------------------------------------------------------------------
	// Mode Independent
	//-------------------------------------------------------------------------
	int key = e->key();

	//-------------------------------------------------------------------------
	// Viewing modes
	//-------------------------------------------------------------------------

	// Wireframe management
	if (key == Qt::Key::Key_F)
	{
		if (m_viewingMode == ViewingMode::NORMAL)
		{
			m_viewingMode = ViewingMode::WIREFRAME;
			m_viewer->getCurrentScene()->drawAllLights(false);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (m_viewingMode == ViewingMode::WIREFRAME)
		{
			m_viewingMode = ViewingMode::LIGHT;
			m_viewer->getCurrentScene()->drawAllLights(true);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (m_viewingMode == ViewingMode::LIGHT)
		{
			m_viewingMode = ViewingMode::NORMAL;
			m_viewer->getCurrentScene()->drawAllLights(false);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}

	//-------------------------------------------------------------------------
	// Interaction and Transformation Modes
	//-------------------------------------------------------------------------

	//Interaction Mode management
	else if (key == Qt::Key::Key_T) //Transformation
	{
		m_interactionMode = TRANSFORMATION;
		m_transformationMode = TRANSLATION;
	}
	else if (key == Qt::Key::Key_N) //Navigation
	{
		m_interactionMode = NAVIGATION;
	}
	else if (key == Qt::Key::Key_L) //Locked
	{
		m_interactionMode = LOCKED;
	}
	else if (key == Qt::Key::Key_R)
	{
		m_transformationMode = ROTATION;
	}

	//-------------------------------------------------------------------------
	// Keep track of pressed keys 
	//-------------------------------------------------------------------------
	// Realtime (key held) interaction management
	if (key >= 0 && key < 1024)
	{
		m_keys[key] = true;
	}
}

void ViewerState::handleKeyReleaseEvent(QKeyEvent* e)
{
	int key = e->key();
	m_keys[key] = false;
}

void ViewerState::handleMouseMoveEvent(QMouseEvent* e)
{
	double xPos = e->localPos().x();
	double yPos = e->localPos().y();

	if (m_hasCurrentOperation)
	{
		double xoffset = xPos - m_lastMousePosition.x;
		double yoffset = m_lastMousePosition.y - yPos; // Y coordinates range from bottom to top

		double xoffsetSinceOperationStart = xPos - m_mousePosStartOp.x;
		double yoffsetSinceOperationStart = yPos - m_mousePosStartOp.y;

		if (m_interactionMode == InteractionMode::NAVIGATION)
			m_viewer->getCamera()->rotate(xoffset, yoffset);

		else if (m_interactionMode == InteractionMode::TRANSFORMATION && m_currentlySelectedObject)
		{
			//transform currently clicked object
			if (m_transformationMode == TransformationMode::ROTATION)
			{
				//in order to be precise, we compute a new matrix from the start each time
				//instead of adding small deltas to the matrix

				double rotationX = yoffsetSinceOperationStart / 360;
				double rotationY = xoffsetSinceOperationStart / 360;

				glm::mat4x4 m1;
				glm::mat4x4 m2;

				glm::vec4 v_x = glm::vec4(1, 0, 0, 0) * m_currentlySelectedObject->getModelMatrixStart();
				glm::vec4 v_y = glm::vec4(0, 1, 0, 0) * m_currentlySelectedObject->getModelMatrixStart();

				m1 = glm::rotate(m1, float(rotationX), Vec3(v_x.x, v_x.y, v_x.z));
				m2 = glm::rotate(m2, float(rotationY), Vec3(v_y.x, v_y.y, v_y.z));

				m_currentlySelectedObject->rotate(m1*m2);

			}
			else if (m_transformationMode == TransformationMode::TRANSLATION)
			{
				//we move on the camera plane 
				Matrix4x4 viewMat = m_viewer->getCamera()->GetViewMatrix();
				glm::vec4 v(xoffset / 100, yoffset / 100, 0, 0);
				m_currentlySelectedObject->translate(Vec3(v*viewMat));
			}
		}
	}

	m_lastMousePosition.x = xPos;
	m_lastMousePosition.y = yPos;
}

void ViewerState::handleMousePressEvent(QMouseEvent* event)
{
	double mouseX = event->localPos().x();
	double mouseY = event->localPos().y();
	Qt::MouseButton button = event->button();

	if (button == Qt::MouseButton::LeftButton)
	{
		m_hasCurrentOperation = true;
		m_mousePosStartOp = glm::vec2(mouseX, mouseY);
		m_lastMousePosition.x = mouseX;
		m_lastMousePosition.y = mouseY;
	}

	//check if we select an object
	if (m_interactionMode == InteractionMode::TRANSFORMATION)
	{
		//create ray
		float x = (2.0f * mouseX) / m_width - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / m_height;
		float z = 1.0f;
		glm::vec3 ray(x,y,z);
		glm::vec4 ray_clip(ray.x, ray.y, -1.0, 1.0);

		glm::vec4 ray_eye = glm::inverse(m_viewer->getProjectionMatrix()) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
		glm::vec4 ray_wor4 = (glm::inverse(m_viewer->getViewMatrix()) * ray_eye);

		glm::vec3 ray_wor(ray_wor4.x, ray_wor4.y, ray_wor4.z);
		// don't forget to normalise the vector at some point
		ray_wor = glm::normalize(ray_wor);

		Ray r(m_viewer->getCamera()->getPosition(),
				ray_wor, 
				std::numeric_limits<double>::lowest(), 
				std::numeric_limits<double>::max());

		//for each object in the scene, trace ray and check if it intersect the bounding box
		std::vector<Object*> objects;
		std::vector<Object*> sceneObjects = m_viewer->getCurrentScene()->getObjects();
		std::vector<Object*> sceneLights = *(reinterpret_cast<std::vector<Object*>*>(&m_viewer->getCurrentScene()->getLights()));
		objects.reserve(sceneObjects.size() + sceneLights.size());
		objects.insert(objects.end(), sceneObjects.begin(), sceneObjects.end());
		objects.insert(objects.end(), sceneLights.begin(), sceneLights.end()); 

		double minT = std::numeric_limits<double>::max();
		for (int i = 0; i < objects.size(); ++i)
		{
			double t0, t1;
			bool intersect = objects[i]->intersect(r, t0, t1);
			if (intersect && objects[i]->isVisible() && t0 < minT)
			{
				minT = t0;
				m_currentlySelectedObject = objects[i];
				m_lastSelectedObject = m_currentlySelectedObject;
			}
		}
		if (minT == std::numeric_limits<double>::max())
			m_currentlySelectedObject = nullptr;
	}
}

void ViewerState::handleMouseReleasedEvent(QMouseEvent* event)
{
	Qt::MouseButton button = event->button();
	if (button == Qt::MouseButton::LeftButton)
	{
		if (m_currentlySelectedObject)
			m_currentlySelectedObject->updateTransformationData();
		m_hasCurrentOperation = false;
	}
}

InteractionMode ViewerState::getInteractionMode()
{
	return m_interactionMode;
}

Object* ViewerState::getLastSelectedObject()
{
	return m_lastSelectedObject;
}