
#include "AboutDialog.h"



AboutDialog::AboutDialog()
{
	setWindowTitle("About");

	QFont fontRelease("Arial", 14, QFont::Bold);
	
	m_releaseVersion = new QLabel(this);
	m_releaseVersion->setGeometry(QRect(50, 50, 200, 20));
	m_releaseVersion->setText("Release: Beta");
	m_releaseVersion->setFont(fontRelease);

	QFont fontAuthors("Arial", 14, QFont::Normal);
	m_antoineAuthorLabel = new QLabel(this);
	m_antoineAuthorLabel->setGeometry(QRect(50, 100, 400, 20));
	m_antoineAuthorLabel->setText("Antoine Desbois: antoine.desbois@hotmail.ca");
	m_antoineAuthorLabel->setFont(fontAuthors);

	m_myriamAuthorLabel = new QLabel(this);
	m_myriamAuthorLabel->setGeometry(QRect(50, 150, 400, 20));
	m_myriamAuthorLabel->setText("Myriam Beauvais");
	m_myriamAuthorLabel->setFont(fontAuthors);
}