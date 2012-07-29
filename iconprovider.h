#ifndef KABAL_ICONPROVIDER
#define KABAL_ICONPROVIDER

#include <QDeclarativeImageProvider>

class IconProvider: public QDeclarativeImageProvider
{
public:
	IconProvider();
	QPixmap requestPixmap(const QString& id, QSize *size, const QSize &requestedSize);



};

#endif

