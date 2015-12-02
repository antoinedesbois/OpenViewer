

#include "ImportDialog.h"


ImportDialog::ImportDialog(ViewerGL* viewerGLWidget, const std::vector<std::string>& shaderList) :
	m_shaderList(shaderList),
	m_viewerGLWidget(viewerGLWidget)
{
	setWindowTitle(tr("Import Dialog"));


	// OK/Cancel buttons
	m_buttonBox = new QDialogButtonBox(this);
	m_buttonBox->setGeometry(QRect(30, 240, 341, 32));
	m_buttonBox->setOrientation(Qt::Horizontal);
	m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

	//File 
	m_fileLabel = new QLabel(this);
	m_fileLabel->setGeometry(QRect(50, 25, 200, 20));
	m_fileLabel->setText("File to import");

	m_fileLineEdit = new QLineEdit(this);
	m_fileLineEdit->setGeometry(QRect(150, 25, 200, 20));

	//Object Position
	m_positionLabel = new QLabel(this);
	m_positionLabel->setGeometry(QRect(50, 50, 200, 20));
	m_positionLabel->setText("Object Position");

	m_posXSpinBox = new QSpinBox(this);
	m_posYSpinBox = new QSpinBox(this);
	m_posZSpinBox = new QSpinBox(this);
	m_posXSpinBox->setGeometry(QRect(150, 50, 50, 20));
	m_posYSpinBox->setGeometry(QRect(200, 50, 50, 20));
	m_posZSpinBox->setGeometry(QRect(250, 50, 50, 20));
	m_posXSpinBox->setMaximum(1000);
	m_posYSpinBox->setMaximum(1000);
	m_posZSpinBox->setMaximum(1000);
	m_posXSpinBox->setMinimum(-1000);
	m_posYSpinBox->setMinimum(-1000);
	m_posZSpinBox->setMinimum(-1000);

	//Shader
	m_shaderLabel = new QLabel(this);
	m_shaderLabel->setGeometry(QRect(50, 75, 200, 20));
	m_shaderLabel->setText("Shader");

	m_shaderComboBox = new QComboBox(this);
	m_shaderComboBox->setGeometry(QRect(150, 75, 200, 20));
	//populate shader combo box
	for each (std::string shaderStr in m_shaderList)
	{
		m_shaderComboBox->addItem(QString(shaderStr.c_str()));
	}

	//smooth normals
	m_normalSmoothLabel = new QLabel(this);
	m_normalSmoothLabel->setGeometry(QRect(50, 100, 200, 20));
	m_normalSmoothLabel->setText("Smooth normals");

	m_smoothNormalsCheckBox = new QCheckBox(this);
	m_smoothNormalsCheckBox->setGeometry(QRect(150, 100, 20, 20));

	QObject::connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	QObject::connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QObject::connect(this, SIGNAL(modelImportTrigger(const ModelImportOptions*)), m_viewerGLWidget, SLOT(modelImportAccept(const ModelImportOptions*)));
	QMetaObject::connectSlotsByName(this);
}

/*
* Sends an import request to the Viewer with the specified parameters
*/
void ImportDialog::accept()
{
	close();

	ModelImportOptions* options = new ModelImportOptions();
	options->smoothNormal = m_smoothNormalsCheckBox->isChecked();
	options->path = m_fileLineEdit->text().toUtf8();
	options->position = Vec3(m_posXSpinBox->value(), m_posYSpinBox->value(), m_posZSpinBox->value());
	options->shader = m_shaderComboBox->currentText().toUtf8();

	//send onModelImportTrigger signal
	emit modelImportTrigger(options);
}