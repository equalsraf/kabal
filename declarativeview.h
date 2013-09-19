#ifndef KABAL_DECLARATIVEVIEW
#define KABAL_DECLARATIVEVIEW

#include <QDeclarativeView>

class DeclarativeView: public QDeclarativeView
{
	Q_OBJECT
	Q_PROPERTY(int screenCount READ screenCount)
	Q_PROPERTY(int screenNumber READ screenNumber)
	Q_PROPERTY(QRect screenGeometry READ screenGeometry)
	Q_PROPERTY(bool primaryScreen READ primaryScreen)
public:
	DeclarativeView(QWidget *parent=0);
	int screenCount();
	int screenNumber();
	QRect screenGeometry();
	bool primaryScreen();
signals:
	void mouseWheelUp();
	void mouseWheelDown();
protected:
	virtual void wheelEvent(QWheelEvent *ev);
};

#endif
