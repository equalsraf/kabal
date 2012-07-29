#include "notificationtimeout.h"


NotificationTimeout::NotificationTimeout(NotificationModel *m, int id, int timeout)
:QTimer(m), m_id(id), m_model(m) {
		setInterval(timeout);
		setSingleShot(true);
		connect(this, SIGNAL(timeout()),
				this, SLOT(timedOut()));
		start();
}

void NotificationTimeout::timedOut() {
	m_model->CloseNotification(m_id, 1);
	deleteLater();
}


