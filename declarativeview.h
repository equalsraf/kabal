#ifndef KABAL_DECLARATIVEVIEW
#define KABAL_DECLARATIVEVIEW

#include <QQuickView>

class DeclarativeView: public QQuickView
{
	Q_OBJECT
	Q_PROPERTY(int screenCount READ screenCount)
	Q_PROPERTY(QRect screenGeometry READ screenGeometry)
public:
	DeclarativeView(QWindow *parent=0);
	int screenCount();
	QRect screenGeometry();
signals:
	void mouseWheelUp();
	void mouseWheelDown();
protected:
	virtual void wheelEvent(QWheelEvent *ev);
};

#endif
