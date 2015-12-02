#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Viewer.h"
#include "ObjectInformationWidget.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:

	//Model Import
	void on_actionImport_Model_triggered(); //menu trigger

	//About
	void on_actionAbout_triggered();

	//Object view
	void on_actionObject_View_triggered();

private:
    Ui::MainWindow *ui;

	ObjectInformationWidget* m_objectInfoWidget;

	Viewer* getViewer() const;
};

#endif // MAINWINDOW_H
