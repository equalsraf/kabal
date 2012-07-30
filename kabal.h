#ifndef KABAL_KBAL
#define KA

#include <QtGui>
#include <QDeclarativeView>
#include <QWebView>

#include "notificationmodel.h"

class Kabal: public QObject
{
	Q_OBJECT
public:
	Kabal(QObject *parent=0);
	bool isRunning() { return model.isRunning(); }

protected:
	QDeclarativeView* createWidget();

public slots:
	void setNotificationsDisabled(bool);
	void showLogViewer();

protected slots:
	void screenCountChanged(int);
	void systrayActivated(QSystemTrayIcon::ActivationReason);

private:
	QMenu menu;
	QSystemTrayIcon tray;
	NotificationModel model;
	QList <QDeclarativeView*> widgets;
	QWebView *logWidget;
	QFileSystemWatcher fsWatcher;
};

#endif
