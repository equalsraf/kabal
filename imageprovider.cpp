#include "imageprovider.h"
#include <QtGui>
ImageProvider::ImageProvider(NotificationModel *model)
:QQuickImageProvider(QQuickImageProvider::Image), model(model)
{
}

QImage ImageProvider::requestImage(const QString& id, QSize *size, const QSize &requestedSize)
{
	int n_id;
	bool ok;
	n_id = id.toInt(&ok);
	if (!ok) {
		return QImage();
	}

	QImage img = model->getImage(n_id);
	*size = img.size();
	return img;
}
