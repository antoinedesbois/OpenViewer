

#include "ObjectInformationWidget.h"


ObjectInformationWidget::ObjectInformationWidget(QWidget* parent, Viewer* viewer) :
	QWidget(parent),
	m_viewer(viewer),
	m_selectedItem(0)
{
	move(DEFAULT_POSITION_X, DEFAULT_POSITION_Y);
	resize(DEFAULT_SIZE_X, 600);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(refreshObjectInformation()));
	timer->start(100);

	//build the list of objects
	m_listObject = new QListWidget(this);
	m_listObject->resize(DEFAULT_SIZE_X, DEFAULT_SIZE_Y/4);
	m_listObject->setMaximumHeight(500);

	//populate list
	std::vector<std::string> listObjNames = getSceneObjectsName();
	for each (std::string objName in listObjNames)
	{
		m_listObject->addItem(QString(objName.c_str()));
	}

	m_globalLayout = new QVBoxLayout(this);
	m_labelLayout = new QGridLayout(this);


	//initialize other ui elements
	QFont fontLabels("Arial", 14, QFont::Normal);

	m_verticeNumberLabel = new QLabel(this);
	//m_verticeNumberLabel->setGeometry(QRect(25, 300, 200, 20));
	m_verticeNumberLabel->setText("Number of vertices: ");
	m_verticeNumberLabel->setFont(fontLabels);

	m_verticeNumber = new QLabel(this);
	//m_verticeNumber->setGeometry(QRect(300, 300, 200, 20));
	m_verticeNumber->setFont(fontLabels);

	m_positionLabel = new QLabel(this);
	//m_positionLabel->setGeometry(QRect(25, 400, 200, 20));
	m_positionLabel->setText("Position: ");
	m_positionLabel->setFont(fontLabels);

	m_position = new QLabel(this);
	//m_position->setGeometry(QRect(300, 400, 200, 20));
	m_position->setFont(fontLabels);

	m_boundingBoxLabel = new QLabel(this);
	m_boundingBoxLabel->setText("Bounding Box:");
	m_boundingBoxLabel->setFont(fontLabels);

	m_boundingBoxPMin = new QLabel(this);
	m_boundingBoxPMin->setFont(fontLabels);

	m_boundingBoxPMax = new QLabel(this);
	m_boundingBoxPMax->setFont(fontLabels);

	m_shaderLabel = new QLabel(this);
	m_shaderLabel->setText("Shader:");
	m_shaderLabel->setFont(fontLabels);

	m_shaderComboBox = new QComboBox(this);
	m_shaderComboBox->setFont(fontLabels);

	for each (std::string shaderStr in m_viewer->getCurrentScene()->getShaderList())
	{
		m_shaderComboBox->addItem(QString(shaderStr.c_str()));
	}

	m_postProcessesLabel = new QLabel(this);
	m_postProcessesLabel->setText("Post processes: ");
	m_postProcessesLabel->setFont(fontLabels);

	m_postProcessesComboBox = new CheckBoxList(this);
	m_postProcessesComboBox->setFont(fontLabels);

	for each (std::string postProcessStr in m_viewer->getCurrentScene()->getPostProcessList())
	{
		m_postProcessesComboBox->addItem(QString(postProcessStr.c_str()));
	}

	m_globalLayout->addWidget(m_listObject);
	m_globalLayout->addLayout(m_labelLayout);

	m_labelLayout->addWidget(m_verticeNumberLabel,1,0, Qt::Alignment::enum_type::AlignRight);
	m_labelLayout->addWidget(m_verticeNumber, 1, 1, Qt::Alignment::enum_type::AlignLeft);
	m_labelLayout->addWidget(m_positionLabel,2,0);
	m_labelLayout->addWidget(m_position,2,1);
	
	m_labelLayout->addWidget(m_boundingBoxLabel, 3, 0);
	m_labelLayout->addWidget(m_boundingBoxPMin, 3, 1);
	m_labelLayout->addWidget(m_boundingBoxPMax, 3, 2);

	m_labelLayout->addWidget(m_shaderLabel, 4, 0);
	m_labelLayout->addWidget(m_shaderComboBox, 4, 1);

	m_labelLayout->addWidget(m_postProcessesLabel, 5, 0);
	m_labelLayout->addWidget(m_postProcessesComboBox, 5, 1);

	connect(m_listObject, SIGNAL(itemClicked(QListWidgetItem*)),
		this, SLOT(itemSelected(QListWidgetItem*)));
	connect(m_shaderComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(shaderChanged(int)));
}


std::vector<std::string> ObjectInformationWidget::getSceneObjectsName()
{
	return m_viewer->getSceneObjectsName();
}

void ObjectInformationWidget::shaderChanged(int test)
{
	//get selected object
	if (m_listObject->currentItem())
	{
		std::string currentObjName = m_listObject->currentItem()->text().toUtf8();
		std::string shaderName = m_shaderComboBox->currentText().toUtf8();

		m_viewer->getCurrentScene()->changeObjectShader(currentObjName, shaderName);
	}
}

void ObjectInformationWidget::itemSelected(QListWidgetItem * item)
{
	std::string objectName = item->text().toUtf8();

	//Get object basic information
	ObjectInformation info = m_viewer->objectBasicInformation(objectName);

	m_verticeNumber->setText(std::to_string(info.verticeNumber).c_str());

	std::string pos = "(" + std::to_string((int)info.position.x) + ", " + std::to_string((int)info.position.y) + ", " + std::to_string((int)info.position.z) + " )";
	m_position->setText(pos.c_str());

	std::string pMinPos = "(" + std::to_string((int)info.bbox.m_pMin.x) + ", " + std::to_string((int)info.bbox.m_pMin.y) + ", " + std::to_string((int)info.bbox.m_pMin.z) + " )";
	m_boundingBoxPMin->setText(pMinPos.c_str());

	std::string pMaxPos = "(" + std::to_string((int)info.bbox.m_pMax.x) + ", " + std::to_string((int)info.bbox.m_pMax.y) + ", " + std::to_string((int)info.bbox.m_pMax.z) + " )";
	m_boundingBoxPMax->setText(pMaxPos.c_str());

	std::vector<std::string> shaderList = m_viewer->getCurrentScene()->getShaderList();
	int index = 0;
	for (int i = 0; i < shaderList.size(); ++i)
	{
		if (shaderList[i] == info.shaderProgramStr)
		{
			index = i;
			break;
		}
	}

	m_shaderComboBox->setCurrentIndex(index);

	m_selectedItem = item;

}

void ObjectInformationWidget::refreshObjectList()
{
	m_selectedItem = 0;
	m_listObject->clear();

	std::vector<std::string> listObjNames = getSceneObjectsName();
	for each (std::string objName in listObjNames)
	{
		m_listObject->addItem(QString(objName.c_str()));
	}

}

void ObjectInformationWidget::refreshObjectInformation()
{
	if (m_selectedItem)
	{
		std::string objectName = m_selectedItem->text().toUtf8();
		ObjectInformation info = m_viewer->objectBasicInformation(objectName);

		std::string pos = "(" + std::to_string((int)info.position.x) + ", " + std::to_string((int)info.position.y) + ", " + std::to_string((int)info.position.z) + " )";
		m_position->setText(pos.c_str());

		std::string pMinPos = "(" + std::to_string((int)info.bbox.m_pMin.x) + ", " + std::to_string((int)info.bbox.m_pMin.y) + ", " + std::to_string((int)info.bbox.m_pMin.z) + " )";
		m_boundingBoxPMin->setText(pMinPos.c_str());

		std::string pMaxPos = "(" + std::to_string((int)info.bbox.m_pMax.x) + ", " + std::to_string((int)info.bbox.m_pMax.y) + ", " + std::to_string((int)info.bbox.m_pMax.z) + " )";
		m_boundingBoxPMax->setText(pMaxPos.c_str());
	}
}