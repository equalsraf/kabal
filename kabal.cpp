#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include "kabal.h"
#include "iconprovider.h"
#include "imageprovider.h"


Kabal::Kabal(const QUrl& source, QObject *parent)
:QObject(parent), m_qmlSource(QUrl("qrc:///qml/kabal.qml"))
{
	if ( source.isValid() ) {
		m_qmlSource = source;
	}

	// System Tray
	tray.setIcon(QIcon(":icons/kabal.png"));

	// SysTray menu
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
		QDir(QDir::home().absoluteFilePath(".kabal")).absoluteFilePath("log"));

}

QDeclarativeView* Kabal::createWidget()
{
	// Widget
	QDeclarativeView *view = new QDeclarativeView;
	view->engine()->addImageProvider(QLatin1String("icons"), new IconProvider());
	view->engine()->addImageProvider(QLatin1String("images"), new ImageProvider(&model));

	view->setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

	// Export methods
	view->rootContext()->setContextProperty("notificationModel", &model);
	view->rootContext()->setContextProperty("Window", view);

	// Load root qml object
	view->setSource(m_qmlSource);
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
		model.Critical("kabal", "Kabal", "Notifications are now disabled", 2);
		model.setNotificationsDisabled(disabled);
	} else {
		model.setNotificationsDisabled(disabled);
		model.Critical("kabal", "Kabal", "Notifications are now enabled", 2);
	}
}

void Kabal::systrayActivated(QSystemTrayIcon::ActivationReason why)
{
	if ( why != QSystemTrayIcon::Trigger && model.rowCount()) {
		foreach (QWidget *w, widgets) {
			w->show();
		}
	}
}

