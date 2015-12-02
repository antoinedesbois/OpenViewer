#ifndef VIEWERGL_H
#define VIEWERGL_H

#include "Viewer.h"

#include <QGLWidget>

class ViewerGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit ViewerGL(QWidget* parent =0);
    ~ViewerGL();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

	//Events
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

	//
	Viewer* getViewer() const;

public slots:
	void modelImportAccept(const ModelImportOptions* options);

private:
    Viewer* m_viewer;

};

#endif // VIEWERGL_H
