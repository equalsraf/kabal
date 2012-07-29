#ifndef KAVAL_NOTIFICATIONTIMEOUT
#define KAVAL_NOTIFICATIONTIMEOUT

#include <QTimer>
#include "notificationmodel.h"

//
// A helper class to launch notification timeout
// events - i.e. a singleshot timer that calls
// NotificationModel::CloseNotification( id )
//
class NotificationTimeout: public QTimer
{
	Q_OBJECT
public:
	NotificationTimeout(NotificationModel *m, int id, int timeout);
protected slots:
	void timedOut();
private:
	int m_id;
	NotificationModel *m_model;
};

#endif
