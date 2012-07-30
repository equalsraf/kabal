#ifndef KABAL_IMAGEPROVIDER
#define KABAL_IMAGEPROVIDER

#include <QDeclarativeImageProvider>
#include "notificationmodel.h"

class ImageProvider: public QDeclarativeImageProvider
{
public:
	ImageProvider(NotificationModel *);
	QImage requestImage(const QString& id, QSize *size, const QSize &requestedSize);
private:
	NotificationModel *model;
};

#endif

