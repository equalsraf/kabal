#ifndef KABAL_DECLARATIVEVIEW
#define KABAL_DECLARATIVEVIEW

#include <QDeclarativeView>

class DeclarativeView: public QDeclarativeView
{
	Q_OBJECT
public:
	DeclarativeView(QWidget *parent=0);
signals:
	void mouseWheelUp();
	void mouseWheelDown();
protected:
	virtual void wheelEvent(QWheelEvent *ev);
};

#endif
