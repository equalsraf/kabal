#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include "kabal.h"
#include "iconprovider.h"

#include <QWebFrame>

Kabal::Kabal(QObject *parent)
:QObject(parent), logWidget(0)
{
	// System Tray
	tray.setIcon(QIcon(":icons/kabal.png"));

	QAction *disable = menu.addAction("Disable notifications");
	disable->setCheckable(true);
	connect(disable, SIGNAL(toggled(bool)),
		this, SLOT(setNotificationsDisabled(bool)));

	connect(&tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(systrayActivated(QSystemTrayIcon::ActivationReason)));

	tray.setContextMenu(&menu);
	tray.show();

	QDesktopWidget *desktop = QApplication::desktop();
	connect(desktop, SIGNAL(screenCountChanged(int)),
		this, SLOT(screenCountChanged(int)));

	screenCountChanged(desktop->screenCount());


	model.setLogFilePath(
		QDir(QDir::home().absoluteFilePath("kabal")).absoluteFilePath("kabal.html"));

	// FIXME: load path from notification model
	if ( !model.logFilePath().isEmpty() ) {
		fsWatcher.addPath(model.logFilePath());

		logWidget = new QWebView();
		logWidget->settings()->setUserStyleSheetUrl(QUrl("qrc:///css/log.css"));
		logWidget->settings()->setDefaultTextEncoding("utf-8");
		logWidget->load(QUrl::fromLocalFile(model.logFilePath()));
		connect(&fsWatcher, SIGNAL(fileChanged(QString)),
			logWidget, SLOT(reload()));
	}
}

QDeclarativeView* Kabal::createWidget()
{
	// Widget
	QDeclarativeView *view = new QDeclarativeView;
	view->engine()->addImageProvider(QLatin1String("icons"), new IconProvider());
	view->setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

	// Export methods
	view->rootContext()->setContextProperty("notificationModel", &model);
	view->rootContext()->setContextProperty("Window", view);

	// Load root qml object
	view->setSource(QUrl("qrc:///qml/kabal.qml"));
	view->move(10, 10);

	return view;
}

void Kabal::screenCountChanged(int count)
{
	// Remove unneed widgets
	if ( count < widgets.size() ) {
		for (int i=count; i< widgets.size(); i++) {
			widgets.takeAt(i)->deleteLater();
		}
	} else if ( count > widgets.size() ) {
		for (int i=widgets.size(); i<count ; i++) {
			widgets.append(createWidget());
		}
	}
	
	// Reposition remaining widgets
	for (int i=0; i<count; i++) {
		QRect screen = QApplication::desktop()->screenGeometry(i);
		widgets.at(i)->move(screen.left() + 10, screen.top() + 10);
	}
}

void Kabal::setNotificationsDisabled(bool disabled)
{
	if ( disabled ) {
		model.Notify("kabal", "Kabal", "Notifications are now disabled", 2);
		model.setNotificationsDisabled(disabled);
	} else {
		model.setNotificationsDisabled(disabled);
		model.Notify("kabal", "Kabal", "Notifications are now enabled", 2);
	}
}

void Kabal::systrayActivated(QSystemTrayIcon::ActivationReason why)
{
	if ( why != QSystemTrayIcon::Trigger || !logWidget) {
		return;
	}

	logWidget->reload();
	logWidget->setVisible(!logWidget->isVisible());
}

