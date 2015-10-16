#ifndef KABAL_ICONPROVIDER
#define KABAL_ICONPROVIDER

#include <QQuickImageProvider>

class IconProvider: public QQuickImageProvider
{
public:
	IconProvider();
	QPixmap requestPixmap(const QString& id, QSize *size, const QSize &requestedSize);
protected:
	static QStringList extensions;
};

#endif

