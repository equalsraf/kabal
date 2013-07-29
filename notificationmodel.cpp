#include "notificationmodel.h"

#include "notificationtimeout.h"

// The "image-data" and "icon_data" hints should be a raw image data structure of signature
// (iiibiiay) which describes the width, height, rowstride, has alpha, bits per sample, 
// channels and image data respectively.
struct ImageData {
	qint32 width;
	qint32 height;
	qint32 rowstride;
	bool has_alpha;
	qint32 bits_per_sample;
	qint32 channels;
	QByteArray data;
};

QDBusArgument &operator<<(QDBusArgument &argument, const ImageData &img)
{
	argument.beginStructure();
	argument << img.width;
	argument << img.height;
	argument << img.rowstride;
	argument << img.has_alpha;
	argument << img.bits_per_sample;
	argument << img.channels;
	argument << img.data;
	argument.endStructure();
	return argument;
}
const QDBusArgument &operator>>(const QDBusArgument &argument, struct ImageData &img)
{
	argument.beginStructure();
	argument >> img.width;
	argument >> img.height;
	argument >> img.rowstride;
	argument >> img.has_alpha;
	argument >> img.bits_per_sample;
	argument >> img.channels;
	argument >> img.data;
	argument.endStructure();
	return argument;
}

Q_DECLARE_METATYPE(ImageData);

//
// DBus adaptor
//
NotificationModelAdaptor::NotificationModelAdaptor(NotificationModel* w)
: QDBusAbstractAdaptor((QObject* )w), widget(w)
{
	connect(w, SIGNAL(notificationClosed(quint32, quint32)),
		this, SIGNAL(NotificationClosed(quint32, quint32)));
	connect(w, SIGNAL(ActionInvoked(quint32, const QString&)),
		this, SIGNAL(ActionInvoked(quint32, const QString&)));
}

QStringList NotificationModelAdaptor::GetCapabilities() {
	return widget->GetCapabilities();
}

void NotificationModelAdaptor::GetServerInformation(QString& name, QString& vendor, QString& version)
{
	name = "Kabal";
	vendor = "equalsraf";
	version = "0.1";
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
		const QString &summary, const QString &body, const QStringList &actionsArr,
	       	const QVariantMap &hints, int expire_timeout)
{
	return widget->Notify(app_name, replaces_id, app_icon, summary, body, actionsArr, hints, expire_timeout);
}


////////////////////////


NotificationModel::NotificationModel(QObject *parent)
:QAbstractListModel(parent), idcounter(1),
	m_running(true),
	m_notificationsDisabled(false),
	logFile(0), logDevice(0)
{
	qDBusRegisterMetaType<ImageData>();
	QHash<int, QByteArray> roles;
	roles[ApplicationRole] = "application";
	roles[IconRole] = "icon";
	roles[SummaryRole] = "summary";
	roles[BodyRole] = "body";
	roles[UidRole] = "uid";
	roles[CriticalRole] = "critical";
	roles[ActionKeysRole] = "actionkeys";
	roles[ActionNamesRole] = "actionnames";
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
	if ( !logDevice->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) ) {
		qDebug() << "Unable to open logFile";
	} else {
		logFile = new QTextStream(logDevice);
		logFile->setCodec("UTF-8");
	}
}


// Slots

QStringList NotificationModel::GetCapabilities() 
{
	return QStringList() << "body" << "actions";
}

void NotificationModel::CloseNotification(quint32 id, quint32 reason)
{
	if ( !notifications.contains(id) ) {
		return;
	}

	const QList<quint32>& notlist = m_notificationsDisabled ?
						criticalNotificationsOrder:
						notificationsOrder;
	int idx = notlist.indexOf(id);
	beginRemoveRows(QModelIndex(), idx, idx);
	notificationsOrder.removeOne(id);

	struct notification n = notifications.take(id);
	if ( n.critical) {
		criticalNotificationsOrder.removeOne(id);
	}
	endRemoveRows();

	emit notificationClosed(id, reason);
	emit notificationCountChanged(notlist.size());
}

void NotificationModel::CloseAllNotifications()
{
	QHashIterator<quint32, struct notification> it(notifications);
	while( it.hasNext() ) {
		it.next();
		CloseNotification(it.key());
	}
}


QImage NotificationModel::getImageFromHints(const QMap<QString, QVariant>& hints)
{
	QVariant val;

	if ( hints.contains("image-data") ) {
		val = hints.value("image-data");
	} else if ( hints.contains("image_data") ) {
		val = hints.value("image_data");
	} else {
		return QImage();
	}

	if ( val.canConvert<QDBusArgument>() ) {
		QDBusArgument arg = val.value<QDBusArgument>();
		ImageData data;
		arg >> data;
		return QImage((uchar*)data.data.constData(), data.width, data.height, QImage::Format_ARGB32).rgbSwapped();
	}

	return QImage();
}


void NotificationModel::incrementCounter()
{

	if ( idcounter != INT_MAX ) {
		idcounter++;
		return;
	}

	if (notifications.size() == INT_MAX) {
		// Remove some notifications NOW
		quint32 oldest;
		if ( notificationsOrder.size()) {
			oldest = notificationsOrder.first();
		} else {
			oldest = criticalNotificationsOrder.first();
		}
		CloseNotification(oldest);
	}
	idcounter = 1;
	while ( notifications.contains(idcounter++) );
}

quint32 NotificationModel::Notify(const QString& app, quint32 replace, const QString& icon, 
		const QString& summary, const QString& body,
		const QStringList& actionsArr, const QMap<QString, QVariant> &hints,
		int timeout) {

	if ( timeout == -1 || (timeout > 0 && timeout < 1000) ) {
		timeout = 7000;
	}

	if ( replace ) {
		CloseNotification(replace);
	}

	// Use the app name as app icon
	QString appIcon = icon;
	if ( appIcon.isEmpty() ) {
		appIcon = app;
	}

	// Add notification
	quint32 uid = idcounter;
	incrementCounter();

	bool critical =  (hints.value("urgency").toUInt() == 2);

	// Extract image-data
	QString iconPath;
	QImage img = getImageFromHints(hints);
	if ( img.isNull() ) {
		iconPath = "image://icons/" + appIcon;
	} else {
		iconPath = "image://images/" + QString::number(uid);
	}

	// actions
	QVariantList actionKeys, actionNames;
	for ( int i=0; i+1<actionsArr.size(); i+=2) {
		actionKeys.append(actionsArr.at(i));
		actionNames.append(actionsArr.at(i+1));
	}

	struct NotificationModel::notification n = {
			uid,
			app, 
			iconPath,
			img,
			htmlToPlainText(summary),
			htmlToPlainText(body),
			timeout,
			critical,
			actionKeys,
			actionNames,
		};
	// Write to log file
	log(n);

	const QList<quint32>& notlist = m_notificationsDisabled ?
						criticalNotificationsOrder:
						notificationsOrder;
	beginInsertRows(QModelIndex(), notlist.size(), notlist.size());
	notificationsOrder.append(uid);
	if ( critical ) {
		criticalNotificationsOrder.append(uid);
	}
	notifications.insert(uid, n);
	endInsertRows();

	emit notificationCountChanged(notlist.size());

	if ( timeout != 0 ) {
		new NotificationTimeout(this, uid, timeout);
	}

	return uid;
}

quint32 NotificationModel::Critical(const QString& app, const QString& summary, const QString& body, int timeout)
{
	QMap<QString, QVariant> hints;
	hints.insert("urgency", 2);
	return Notify(app, 0, "", summary, body, QStringList(), hints , timeout );
}

void NotificationModel::log(struct NotificationModel::notification& n)
{
	if (!logFile) {
		return;
	}

	*logFile << QDateTime::currentDateTime().toString() << QString(" \"%1\" from *%2*: %3\n").arg(n.summary).arg(n.app).arg(n.body);
	logFile->flush();
	logDevice->flush();
}

int NotificationModel::rowCount(const QModelIndex& index) const
{
	if ( m_notificationsDisabled ) {
		return criticalNotificationsOrder.size();
	} else {	
		return notificationsOrder.size();
	}
}

QVariant NotificationModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	const QList<quint32>& notlist = m_notificationsDisabled ?
						criticalNotificationsOrder:
						notificationsOrder;

	if ( index.row() >= notlist.size() ) {
		return QVariant();
	}

	const struct NotificationModel::notification& n = notifications.value(notlist[index.row()]);
	switch(role) {
	case ApplicationRole:
		return n.app;
	case IconRole:
		return n.icon;
	case ImageRole:
		return n.image;
	case Qt::DisplayRole:
	case SummaryRole:
		return n.summary;
	case BodyRole:
		return n.body;
	case UidRole:
		return n.uid;
	case CriticalRole:
		return n.critical;
	case ActionKeysRole:
		return n.actionKeys;
	case ActionNamesRole:
		return n.actionNames;
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
	if ( disabled == m_notificationsDisabled ) {
		return;
	}

	beginResetModel();
	m_notificationsDisabled = disabled;
	endResetModel();
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

/**
 * Get image for notification with the given id
 */
QImage NotificationModel::getImage(qint32 id)
{
	return notifications.value(id).image;
}

void NotificationModel::invokeAction(quint32 id, const QString& actionkey)
{
	if ( !notifications.contains(id) ) {
		return;
	}

	const struct NotificationModel::notification& n = notifications.value(id);
	emit ActionInvoked(id, actionkey);
	CloseNotification(id);
}

