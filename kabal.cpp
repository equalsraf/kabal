#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include "kabal.h"
#include "iconprovider.h"
#include "imageprovider.h"


Kabal::Kabal(const QUrl& source, QObject *parent)
:QObject(parent), m_qmlSource(QUrl("qrc:///qml/kabal.qml")),
	settings(QDir(QDir::home().absoluteFilePath(".kabal")).absoluteFilePath("config"), QSettings::IniFormat),
	trayIcon(":icons/kabal.png"), trayIconDisabled(":icons/kabal_s.png")
{
	if ( source.isValid() ) {
		m_qmlSource = source;
	}

	// System Tray
	tray.setIcon(trayIcon);

	// SysTray menu
	QAction *disable = menu.addAction("Disable notifications");
	disable->setCheckable(true);
	connect(disable, SIGNAL(toggled(bool)),
		&model, SLOT(setNotificationsDisabled(bool)));

	connect(&model, SIGNAL(notificationsToggled(bool)),
			this, SLOT(notificationsDisabled(bool)));
	connect(&model, SIGNAL(notificationsToggled(bool)),
			disable, SLOT(setChecked(bool)));

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

	connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()),
			&model, SLOT(CloseAllNotifications()));
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
	
	// settings
	int x, y;
	x = settings.value("x", 10).toInt();
	y = settings.value("y", 10).toInt();
	QString corner = settings.value("corner", "").toString().toLower();
	int corner_opt;

	if ( corner == "topleft" ) {
		corner_opt = Qt::TopLeftCorner;
	} else if ( corner == "topright" ) {
		corner_opt = Qt::TopRightCorner;
	} else if ( corner == "bottomleft" ) {
		corner_opt = Qt::BottomLeftCorner;
	} else if ( corner == "bottomright" ) {
		corner_opt = Qt::BottomRightCorner;
	} else {
		qDebug() << "Unknown value for corner, assuming top left";
		corner_opt = Qt::TopLeftCorner;
	}

	// Reposition remaining widgets
	for (int i=0; i<count; i++) {
		QRect screen = QApplication::desktop()->screenGeometry(i);

		QWidget *W = widgets.at(i);
		switch( corner_opt ) {
		case Qt::TopRightCorner:
			W->move(screen.right() - W->width() - x, screen.top() + y);
			break;
		case Qt::BottomLeftCorner:
			W->move(screen.left() + x, screen.bottom()- W->height() - y);
			break;
		case Qt::BottomRightCorner:
			W->move(screen.right() - W->width() - x, screen.bottom()- W->height() - y);
			break;
		default:
			// default is top left corner
			W->move(screen.left() + x, screen.top() + y);
		}
	}
	
	model.setMinimalTimeout( settings.value("mintime", 7000).toInt() );
}

void Kabal::notificationsDisabled(bool disabled)
{
	if ( disabled ) {
		model.Critical("kabal_s", "Kabal", "Notifications are now disabled", 2);
		tray.setIcon(trayIconDisabled);
	} else {
		model.Critical("kabal", "Kabal", "Notifications are now enabled", 2);
		tray.setIcon(trayIcon);
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

