#ifndef KABAL_IMAGEPROVIDER
#define KABAL_IMAGEPROVIDER

#include <QQuickImageProvider>
#include "notificationmodel.h"

class ImageProvider: public QQuickImageProvider
{
public:
	ImageProvider(NotificationModel *);
	QImage requestImage(const QString& id, QSize *size, const QSize &requestedSize);
private:
	NotificationModel *model;
};

#endif

