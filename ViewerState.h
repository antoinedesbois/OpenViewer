
#ifndef __VIEWER_STATE_H__
#define __VIEWER_STATE_H__

#include "Viewer.h"
#include "Object.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "GLM/glm/glm.hpp"

class Viewer;

enum InteractionMode 
{
	NAVIGATION,
	TRANSFORMATION,
	LOCKED
};

enum ViewingMode
{
	NORMAL,
	WIREFRAME,
	LIGHT //We can view light and move them
};

enum TransformationMode
{
	ROTATION,
	TRANSLATION
};

struct ViewerOperation
{

};

class ViewerState
{
public:
	ViewerState(Viewer* viewer, int width, int height);



	void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action);

	InteractionMode getInteractionMode();
	bool* getKeys(){ return m_keys; }

	Object* getLastSelectedObject();

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

public:
	void setWindowDimension(int width, int height);

	//Events
	void handleMouseMoveEvent(QMouseEvent* e);
	void handleMousePressEvent(QMouseEvent* e);
	void handleMouseReleasedEvent(QMouseEvent* e);

	void handleKeyPressEvent(QKeyEvent* e);
	void handleKeyReleaseEvent(QKeyEvent* e);

private:
	void startMouseOperation();
	void updateOperationData();

private:
	Viewer* m_viewer;

	//Viewer modes
	InteractionMode m_interactionMode;
	ViewingMode m_viewingMode;
	TransformationMode m_transformationMode;

	// Operations
	ViewerOperation* m_currentOperation;

	// Interaction variables
	bool m_keys[1024]; // All the keys that can be pressed

	bool m_hasCurrentOperation;
	bool m_mouseIsClicked;
	bool m_firstClick;
	glm::vec2 m_lastMousePosition;
	glm::vec2 m_mousePosStartOp;
	Object* m_currentlySelectedObject;
	Object* m_lastSelectedObject;

	//viewer dimensions
	double m_height;
	double m_width;
};

#endif //__VIEWER_STATE_H__