#include "declarativeview.h"
#include <QWheelEvent>


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

