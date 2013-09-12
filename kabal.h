#ifndef KABAL_KBAL
#define KA

#include <QtGui>
#include <QDeclarativeView>

#include "notificationmodel.h"

class Kabal: public QObject
{
	Q_OBJECT
public:
	Kabal(const QUrl& source=QUrl(), QObject *parent=0);
	bool isRunning() { return model.isRunning(); }

protected:
	QDeclarativeView* createWidget();

public slots:
	void notificationsDisabled(bool);

protected slots:
	void screenCountChanged(int);
	void systrayActivated(QSystemTrayIcon::ActivationReason);

private:
	QUrl m_qmlSource;
	QMenu menu;
	QSystemTrayIcon tray;
	NotificationModel model;
	QList <QDeclarativeView*> widgets;
	QFileSystemWatcher fsWatcher;
	QSettings settings;
};

#endif
