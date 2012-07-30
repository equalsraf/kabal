#include <QtGui>
#include "kabal.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Kabal *kabal = new Kabal();
	if ( !kabal->isRunning() ) {
		return -1;
	}

	app.setQuitOnLastWindowClosed(false);
	return app.exec();
}
