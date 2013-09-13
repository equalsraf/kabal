#include <QtGui>
#include "kabal.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
#ifdef KABAL_DEBUG
	Kabal *kabal = new Kabal(QUrl( QDir(QDir().filePath("qml")).filePath("kabal.qml") ));
	qDebug() << "Loading qml interface from local path qml/";
#else
	Kabal *kabal = new Kabal();
#endif
	if ( !kabal->isRunning() ) {
		return -1;
	}

	app.setQuitOnLastWindowClosed(false);
	return app.exec();
}
