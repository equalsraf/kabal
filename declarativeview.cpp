#include "declarativeview.h"
#include <QWheelEvent>
#include <QApplication>
#include <QDesktopWidget>


DeclarativeView::DeclarativeView(QWidget *parent)
:QDeclarativeView(parent)
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
	return QApplication::desktop()->screenCount();
}

int DeclarativeView::screenNumber()
{
	return QApplication::desktop()->screenNumber(this);
}

QRect DeclarativeView::screenGeometry()
{
	return QApplication::desktop()->screenGeometry(this);
}

bool DeclarativeView::primaryScreen()
{
	return screenNumber() == QApplication::desktop()->primaryScreen();
}


