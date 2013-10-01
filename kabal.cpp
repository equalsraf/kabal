#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include "kabal.h"
#include "iconprovider.h"
#include "imageprovider.h"
#include "declarativeview.h"
#include <QxtGlobalShortcut>


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

	QxtGlobalShortcut *shortcut = new QxtGlobalShortcut( QKeySequence("CTRL+SHIFT+n"), this);
	connect(shortcut, SIGNAL(activated()),
		&model, SLOT(toggleNotifications()));

	QDesktopWidget *desktop = QApplication::desktop();
	connect(desktop, SIGNAL(screenCountChanged(int)),
		this, SLOT(screenCountChanged(int)));
	connect(desktop, SIGNAL(resized(int)),
		this, SLOT(repositionWidget(int)));

	screenCountChanged(desktop->screenCount());

	model.setTruncateLog(settings.value("trunclog", true).toBool());
	model.setLogFilePath(
		settings.value("log", QDir(QDir::home().absoluteFilePath(".kabal")).absoluteFilePath("log")).toString()
		);
	if ( QApplication::arguments().contains("--persistence") ) {
		model.setPersistence(true);
	} else {
		model.setPersistence(settings.value("persistence", false).toBool());
	}

	connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()),
			&model, SLOT(CloseAllNotifications()));
}

QDeclarativeView* Kabal::createWidget()
{
	// Widget
	DeclarativeView *view = new DeclarativeView;
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

/**
 * Reposition widget at given screen, to its correct
 * coordinates
 */
void Kabal::repositionWidget(int i)
{
	if ( i >= widgets.size() && i < 0) {
		return;
	}

	QWidget *W = widgets.at(i);
	if ( W == NULL ) {
		return;
	}
	QRect screen = QApplication::desktop()->screenGeometry(i);

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

/**
 * Called when the screen count changes to create/remove
 * the declarative view widgets
 */
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
		repositionWidget(i);
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

