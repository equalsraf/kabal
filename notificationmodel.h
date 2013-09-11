#ifndef __BART_NOTIFYWIDGET__
#define __BART_NOTIFYWIDGET__

#include <QtGui>
#include <QtDBus>

class NotificationModel;

Q_DECLARE_METATYPE(QVariantMap)

class NotificationModelAdaptor: public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Notifications")
public:
	NotificationModelAdaptor(NotificationModel* w);

public slots:

	QStringList GetCapabilities();
	void GetServerInformation(QString &name, QString &vendor, QString &version);
	void GetServerInformation(QString &name, QString &vendor, QString &version, QString &spec);

	uint Notify(const QString &app_name, uint replaces_id, const QString &app_icon, 
		const QString &summary, const QString &body, const QStringList &actions,
	       	const QVariantMap &hints, int expire_timeout);

	void CloseNotification(quint32 id);

signals:
	void NotificationClosed(quint32 id, quint32 reason);
	void ActionInvoked(quint32 id, const QString& reason);

private:
	NotificationModel *widget;

};

class NotificationModel: public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY( QString logFilePath READ logFilePath WRITE setLogFilePath)
	Q_PROPERTY( int notificationCount READ rowCount NOTIFY notificationCountChanged )
	Q_PROPERTY( bool notificationsDisabled READ notificationsDisabled WRITE setNotificationsDisabled NOTIFY notificationsToggled)
	Q_PROPERTY( int minimalTimeout READ minimalTimeout WRITE setMinimalTimeout )

	struct notification {
		quint32 uid;
		QString app;
		QString icon;
		QImage image;
		QString summary;
		QString body;
		int timeout;
		bool critical;
		//QMap<QString, QString> actions;
		QVariantList actionKeys, actionNames;
	};

public:
	enum NotificationRole {
		ApplicationRole = Qt::UserRole+1,
		IconRole,
		SummaryRole,
		BodyRole,
		ImageRole,
		UidRole,
		CriticalRole,
		ActionKeysRole,
		ActionNamesRole,
	};

	NotificationModel(QObject *parent=0);
	bool isRunning() { return m_running; }

	// Model
	virtual int rowCount(const QModelIndex& = QModelIndex()) const;
	virtual QVariant data(const QModelIndex&, int) const;

	bool notificationsDisabled();

	QString htmlToPlainText(const QString&);

	QString logFilePath() { return m_logFilePath; }
	void setLogFilePath(const QString &path);
	QImage getImage(qint32 id);

	void setMinimalTimeout(int t);
	int minimalTimeout() {return m_minimalTimeout;}

public slots:
	QStringList GetCapabilities();
	void CloseNotification(quint32 id, quint32 reason=3);
	void CloseAllNotifications();
	quint32 Notify(const QString& app, uint replace, const QString& icon, 
			const QString& summary, const QString& body,
			const QStringList& actions, const QMap<QString, QVariant> &hints,
			int timeout);
	void setNotificationsDisabled(bool);

	quint32 Critical(const QString& app, const QString& summary, const QString& body, int timeout);
	void invokeAction(quint32 uid, const QString& action);


signals:
	// For DBus
	void notificationClosed(quint32 id, quint32 reason);
	void ActionInvoked(quint32 id, const QString& reason);
	void notificationCountChanged(int count);
	void notificationsToggled(bool);

protected:
	void log(struct NotificationModel::notification&);
	QImage getImageFromHints(const QMap<QString, QVariant>& hints);
	void incrementCounter();

private:
	quint32 idcounter;
	QList<quint32> notificationsOrder;
	QList<quint32> criticalNotificationsOrder;
	QHash<quint32, struct NotificationModel::notification> notifications;
	bool m_running, m_notificationsDisabled;
	QTextStream* logFile;
	QFile *logDevice;
	QString m_logFilePath;
	int m_minimalTimeout;
};


#endif
