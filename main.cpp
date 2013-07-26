#include <QtGui>
#include "kabal.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	// FIXME: this is DEBUG!!!
	Kabal *kabal = new Kabal(QUrl( QDir(QDir().filePath("qml")).filePath("kabal.qml") ));
	//Kabal *kabal = new Kabal();
	if ( !kabal->isRunning() ) {
		return -1;
	}

	app.setQuitOnLastWindowClosed(false);
	return app.exec();
}
