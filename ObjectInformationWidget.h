
#ifndef __OBJECT_INFORMATION_WIDGET_H__
#define __OBJECT_INFORMATION_WIDGET_H__

#include "Viewer.h"

#include <qwidget.h>
#include <qlistwidget.h>

class ObjectInformationWidget : public QWidget
{
public:
	ObjectInformationWidget(QWidget* parent, Viewer* viewer);

private:
	std::vector<std::string> getSceneObjectsName();

public:
	static const unsigned int DEFAULT_SIZE_X = 400;
	static const unsigned int DEFAULT_SIZE_Y = 1080;
	static const unsigned int DEFAULT_POSITION_X = 1920 - DEFAULT_SIZE_X;
	static const unsigned int DEFAULT_POSITION_Y = 1080 - DEFAULT_SIZE_Y;



private:
	Viewer* m_viewer;

	//ui
	QListWidget* m_listObject;
};

#endif //__OBJECT_INFORMATION_WIDGET_H__