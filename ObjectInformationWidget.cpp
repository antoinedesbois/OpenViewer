

#include "ObjectInformationWidget.h"


ObjectInformationWidget::ObjectInformationWidget(QWidget* parent, Viewer* viewer) :
	QWidget(parent),
	m_viewer(viewer)
{
	move(DEFAULT_POSITION_X, DEFAULT_POSITION_Y);
	resize(DEFAULT_SIZE_X, DEFAULT_SIZE_Y);

	//build the list of objects
	m_listObject = new QListWidget(this);
	m_listObject->resize(DEFAULT_SIZE_X, DEFAULT_SIZE_Y/4);
	
	//populate list
	std::vector<std::string> listObjNames = getSceneObjectsName();
	for each (std::string objName in listObjNames)
	{
		m_listObject->addItem(QString(objName.c_str()));
	}
}


std::vector<std::string> ObjectInformationWidget::getSceneObjectsName()
{
	return m_viewer->getSceneObjectsName();
}