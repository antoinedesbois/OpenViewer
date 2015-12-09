
#ifndef __OBJECT_INFORMATION_WIDGET_H__
#define __OBJECT_INFORMATION_WIDGET_H__

#include "Viewer.h"
#include "CheckBoxList.h"

#include <qwidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistwidget.h>
#include <qcombobox.h>

class ObjectInformationWidget : public QWidget
{
	Q_OBJECT
public:
	ObjectInformationWidget(QWidget* parent, Viewer* viewer);

private:
	std::vector<std::string> getSceneObjectsName();

public:
	static const unsigned int DEFAULT_SIZE_X = 500;
	static const unsigned int DEFAULT_SIZE_Y = 1080;
	static const unsigned int DEFAULT_POSITION_X = 1920 - DEFAULT_SIZE_X;
	static const unsigned int DEFAULT_POSITION_Y = 1080 - DEFAULT_SIZE_Y;

public slots:
	void itemSelected(QListWidgetItem* item);
	void shaderChanged(int item);
	void refreshObjectList();
	void refreshObjectInformation();

signals:
	void currentIndexChanged(int item);


private:
	Viewer* m_viewer;

	//------------ ui -------------
	QListWidget* m_listObject;

	QGridLayout* m_labelLayout;
	QVBoxLayout* m_globalLayout;

	//object info
	QLabel*	m_verticeNumberLabel;
	QLabel*	m_verticeNumber;

	QLabel* m_positionLabel;
	QLabel*	m_position;

	QLabel* m_boundingBoxLabel;
	QLabel* m_boundingBoxPMin;
	QLabel* m_boundingBoxPMax;

	QLabel *m_shaderLabel;
	QComboBox *m_shaderComboBox;

	QLabel *m_postProcessesLabel;
	CheckBoxList *m_postProcessesComboBox;
	//post processes --> qlist with checkable items

	QListWidgetItem * m_selectedItem;
};

#endif //__OBJECT_INFORMATION_WIDGET_H__