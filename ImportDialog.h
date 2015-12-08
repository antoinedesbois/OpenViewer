
#ifndef __IMPORT_DIALOG_H__
#define __IMPORT_DIALOG_H__

#include "Scene.h"
#include "Viewer.h"
#include "viewergl.h"
#include "ObjectInformationWidget.h"

#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qdialogbuttonbox.h>

class ImportDialog : public QDialog
{
	Q_OBJECT

public:
	ImportDialog(ViewerGL* viewerGLWidget, const std::vector<std::string>& shaderList);

public slots:
	void accept();

signals:
	void modelImportTrigger(const ModelImportOptions* options);

private:
	//UI
	QDialogButtonBox *m_buttonBox;

	QLabel *m_fileLabel;
	QLineEdit *m_fileLineEdit;

	QLabel *m_positionLabel;
	QSpinBox *m_posXSpinBox;
	QSpinBox *m_posYSpinBox;
	QSpinBox *m_posZSpinBox;

	QLabel *m_shaderLabel;
	QComboBox *m_shaderComboBox;

	QLabel *m_normalSmoothLabel;
	QCheckBox *m_smoothNormalsCheckBox;

private:
	std::vector<std::string> m_shaderList;

	ViewerGL* m_viewerGLWidget;
};

#endif //__IMPORT_DIALOG_H__