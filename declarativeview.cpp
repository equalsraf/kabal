#include "declarativeview.h"
#include <QWheelEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>


DeclarativeView::DeclarativeView(QWindow *parent)
:QQuickView(parent)
{

}

void DeclarativeView::wheelEvent(QWheelEvent *ev)
{
	if (ev->delta() > 0) {
		emit mouseWheelUp();
	} else if ( ev->delta() < 0) {
		emit mouseWheelDown();
	}
}

int DeclarativeView::screenCount()
{
	return QGuiApplication::screens().size();
}

QRect DeclarativeView::screenGeometry()
{
	return screen()->availableGeometry();
}

