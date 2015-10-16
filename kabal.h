#ifndef KABAL_KBAL
#define KA

#include <QtGui>
#include <QQuickView>
#include <QSystemTrayIcon>
#include <QMenu>

#include "notificationmodel.h"

class Kabal: public QObject
{
	Q_OBJECT
public:
	Kabal(const QUrl& source=QUrl(), QObject *parent=0);
	bool isRunning() { return model.isRunning(); }

protected:
	QQuickView* createWidget();

public slots:
	void notificationsDisabled(bool);

protected slots:
	void screenCountChanged(int);
	void repositionWidget(int);
	void systrayActivated(QSystemTrayIcon::ActivationReason);

private:
	QUrl m_qmlSource;
	QMenu menu;
	QSystemTrayIcon tray;
	NotificationModel model;
	QList <QQuickView*> widgets;
	QFileSystemWatcher fsWatcher;
	QSettings settings;
	QIcon trayIcon, trayIconDisabled;
};

#endif
