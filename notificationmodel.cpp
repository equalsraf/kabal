#include "notificationmodel.h"

#include "notificationtimeout.h"


//
// DBus adaptor
//
NotificationModelAdaptor::NotificationModelAdaptor(NotificationModel* w)
: QDBusAbstractAdaptor((QObject* )w), widget(w)
{
	connect(w, SIGNAL(NotificationClosed(quint32, quint32)),
		this, SIGNAL(NotificationClosed(quint32, quint32)));
}

QStringList NotificationModelAdaptor::GetCapabilities() {
	return widget->GetCapabilities();
}

void NotificationModelAdaptor::GetServerInformation(QString& name, QString& vendor, QString& version)
{
	name = "Kabal";
	vendor = "equalsraf";
	version = "0.0alpha";
}

void NotificationModelAdaptor::GetServerInformation(QString& name, QString& vendor, QString& version, QString& spec)
{
	name = "Kabal";
	vendor = "equalsraf";
	version = "0.0alpha";
	spec = "1.0";
}

void NotificationModelAdaptor::CloseNotification(quint32 id) 
{
	widget->CloseNotification(id);
}


uint NotificationModelAdaptor::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, 
		const QString &summary, const QString &body, const QStringList &actions,
	       	const QVariantMap &hints, int expire_timeout)
{

	return widget->Notify(app_name, replaces_id, app_icon, summary, body, actions, hints, expire_timeout);
}


////////////////////////


NotificationModel::NotificationModel(QObject *parent)
:QAbstractListModel(parent), version(1),
	m_lastCriticalNotification(0),
	m_lastNotification(0), 
	m_running(true),
	m_notificationsDisabled(false),
	logFile(0), logDevice(0)
{
	QHash<int, QByteArray> roles;
	roles[ApplicationRole] = "application";
	roles[IconRole] = "icon";
	roles[SummaryRole] = "summary";
	roles[BodyRole] = "body";
	setRoleNames(roles);

	new NotificationModelAdaptor(this);
	QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this);
	if ( !QDBusConnection::sessionBus().registerService("org.freedesktop.Notifications") ) {
		qDebug() << __func__ << "There is another notification handler running, bailing out ...";
		m_running = false;
		return;
	}

}

void NotificationModel::setLogFilePath(const QString& path)
{
	m_logFilePath = path;

	QDir().mkpath(QFileInfo(path).dir().absolutePath());

	logDevice = new QFile(logFilePath(), this);
	if ( !logDevice->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) ) {
		qDebug() << "Unable to open logFile";
	} else {
		logFile = new QTextStream(logDevice);
	}


}


// Slots

QStringList NotificationModel::GetCapabilities() 
{
	return QStringList() << "body";
}

void NotificationModel::CloseNotification(quint32 id, quint32 reason)
{
	if ( !notifications.contains(id) ) {
		return;
	}

	QMutexLocker locker(&lock);
	int idx = notificationsOrder.indexOf(id);
	beginRemoveRows(QModelIndex(), idx, idx);
	notificationsOrder.removeOne(id);
	struct NotificationModel::notification gone = notifications.take(id);
	endRemoveRows();
	emit countChanged();
	emit NotificationClosed(id, reason);

	if ( id != m_lastNotification && id != m_lastCriticalNotification ) {
		return;
	}

	// If we got here there is nothing to show
	if ( gone.critical ) {
		closeCritical();
	} else {
		closeNotification();
	}
}

quint32 NotificationModel::Notify(const QString& app, uint replace, const QString& icon, 
		const QString& summary, const QString& body,
		const QStringList& actions, const QMap<QString, QVariant> &hints,
		int timeout) {

	if ( timeout == -1 || (timeout > 0 && timeout < 7000) ) {
		timeout = 10000;
	}

	qDebug() << app << summary << body << icon << hints << timeout;
	bool critical =  (hints.value("urgency").toUInt() == 2);

	if ( replace ) {
		CloseNotification(replace);
	}

	// Use the app name as app icon
	QString appIcon = icon;
	if ( appIcon.isEmpty() ) {
		appIcon = app;
	}

	// Extract image-data
	if ( hints.contains("image_data") ) {
		QByteArray img = hints.value("image_data").toByteArray();

		// The "image-data" and "icon_data" hints should be a raw image data structure of signature
		// (iiibiiay) which describes the width, height, rowstride, has alpha, bits per sample, 
		// channels and image data respectively.

		qDebug() <<__func__ << img.size();
	}

	struct NotificationModel::notification n = {
			app, 
			"image://icons/" + appIcon, 
			htmlToPlainText(summary),
			htmlToPlainText(body),
			timeout,
			critical
		};

	// Add notification
	QMutexLocker locker(&lock);
	int v = version++; // FIXME: THIS WILL OVERFLOW

	// Write to log file
	log(n);

	beginInsertRows(QModelIndex(), 0, 0);
	notificationsOrder.prepend(v);
	notifications.insert(v, n);
	endInsertRows();

	if ( critical ) {
		m_lastCriticalNotification = v;
	} else {
		m_lastNotification = v;
	}

	if ( timeout != 0 ) {
		new NotificationTimeout(this, v, timeout);
	}

	if ( critical ) {
		emit latestCriticalChanged(n.app, n.summary, n.body, n.icon);
	} else if ( !notificationsDisabled() ) {
		emit latestNotificationChanged(n.app, n.summary, n.body, n.icon);
	}

	emit countChanged();
	return v;
}

quint32 NotificationModel::Notify(const QString& app, const QString& summary, const QString& body, int timeout)
{
	return Notify(app, 0, "", summary, body, QStringList(), QMap<QString, QVariant>() , timeout );
}

void NotificationModel::log(struct NotificationModel::notification& n)
{
	if (!logFile) {
		return;
	}

	*logFile << QString("\n<div class=\"notification\">\n<h3>%1 from %2</h3>\n<p>\n%3\n</p>\n</div>\n").arg(n.summary).arg(n.app).arg(n.body);
	logFile->flush();
	logDevice->flush();
}

int NotificationModel::rowCount(const QModelIndex& index) const
{
	return notifications.size();
}

QVariant NotificationModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}
	if ( index.row() >= notifications.size() ) {
		return QVariant();
	}

	struct NotificationModel::notification n = notifications.value(notificationsOrder[index.row()]);
	switch(role) {
	case ApplicationRole:
		return n.app;
	case IconRole:
		return n.icon;
	case Qt::DisplayRole:
	case SummaryRole:
		return n.summary;
	case BodyRole:
		return n.body;
	}

	return QVariant();
}

/**
 * Disable notifications
 *
 * @warning This will NEVER disable critical notifications
 *
 */
void NotificationModel::setNotificationsDisabled(bool disabled)
{
	m_notificationsDisabled = disabled;
}

bool NotificationModel::notificationsDisabled()
{
	return m_notificationsDisabled;
}

/**
 * Utility function to convert HTML to plain text
 */
QString NotificationModel::htmlToPlainText(const QString& html)
{
	QTextDocument text;
	text.setHtml(html);
	return text.toPlainText();
}



