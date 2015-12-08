
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ImportDialog.h"
#include "AboutDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->ViewerWidget->initializeGL();
	m_objectInfoWidget = new ObjectInformationWidget(this, ui->ViewerWidget->getViewer());
	m_objectInfoWidget->hide();
	showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Viewer* MainWindow::getViewer() const
{
	return ui->ViewerWidget->getViewer();
}

/*
* Opens the 3D model import form
*/
void MainWindow::on_actionImport_Model_triggered()
{
	Viewer* viewer = getViewer();
	std::vector<std::string> shaders = viewer->getCurrentScene()->getShaderList();
	ImportDialog* dialog = new ImportDialog(ui->ViewerWidget, shaders);
	dialog->show();
}

void MainWindow::on_actionAbout_triggered()
{
	AboutDialog* aboutDialog = new AboutDialog();
	aboutDialog->show();
}

void MainWindow::on_actionObject_View_triggered()
{
	if (ui->actionObject_View->isChecked())
	{
		m_objectInfoWidget->refreshObjectList();
		m_objectInfoWidget->show();
		ui->ViewerWidget->resize(m_objectInfoWidget->DEFAULT_POSITION_X, 1080);
	}
	else
	{
		m_objectInfoWidget->hide();
		ui->ViewerWidget->resize(1920, 1080);
	}
}