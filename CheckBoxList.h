#ifndef CHECKBOXLIST_H
#define CHECKBOXLIST_H

#include <QtGui>
#include <qcombobox.h>
#include <qstring.h>

class CheckBoxList : public QComboBox
{
	Q_OBJECT;

public:
	CheckBoxList(QWidget *widget = 0);
	virtual ~CheckBoxList();
	bool eventFilter(QObject *object, QEvent *event);
	virtual void paintEvent(QPaintEvent *);
	void SetDisplayText(QString text);
	QString GetDisplayText() const;

private:
	QString m_DisplayText;
};

#endif // CHECKBOXLIST_H