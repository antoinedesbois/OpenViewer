#include "viewergl.h"

#include <qtimer.h>

#include <iostream>

ViewerGL::ViewerGL(QWidget* parent):
    QGLWidget(parent)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);

	makeCurrent();

	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
		exit(EXIT_FAILURE);
	}
}

ViewerGL::~ViewerGL()
{
}

void ViewerGL::initializeGL()
{
	m_viewer = new Viewer(width(), height());
}

void ViewerGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    std::cout << "Test" << std::endl;
	m_viewer->loop();
}

void ViewerGL::resizeGL(int width, int height)
{
    std::cout << "resizing: " << width << " " << height << std::endl;
	m_viewer->setWindowDimension(width, height);
}

void ViewerGL::mousePressEvent(QMouseEvent* event)
{
	m_viewer->handleMousePressEvent(event);
}

void ViewerGL::mouseReleaseEvent(QMouseEvent* event)
{
	m_viewer->handleMouseReleaseEvent(event);
}

void ViewerGL::mouseMoveEvent(QMouseEvent* e)
{
	m_viewer->handleMouseMoveEvent(e);
}

void ViewerGL::keyPressEvent(QKeyEvent* e)
{
	grabKeyboard();
	m_viewer->handleKeyPressEvent(e);
}

void ViewerGL::keyReleaseEvent(QKeyEvent* e)
{
	if (!e->isAutoRepeat())
	{
		m_viewer->handleKeyReleaseEvent(e);
		releaseKeyboard();
	}
}

Viewer* ViewerGL::getViewer() const
{
	return m_viewer;
}

void ViewerGL::modelImportAccept(const ModelImportOptions* options)
{
	m_viewer->loadModel(options);
}