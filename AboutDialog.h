
#ifndef __ABOUT_DIALOG_H__
#define __ABOUT_DIALOG_H__

#include <qdialog.h>
#include <qlabel.h>

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	AboutDialog();

private:
	//UI
	QLabel* m_releaseVersion;
	QLabel* m_antoineAuthorLabel;
	QLabel* m_myriamAuthorLabel;
	//TODO okay button

};

#endif //__ABOUT_DIALOG_H__