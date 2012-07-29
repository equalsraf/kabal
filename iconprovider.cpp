#include "iconprovider.h"
#include <QtGui>
IconProvider::IconProvider()
:QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
}

QPixmap IconProvider::requestPixmap(const QString& id, QSize *size, const QSize &requestedSize)
{
	QIcon icon;
	QUrl url = QUrl::fromLocalFile(id);
	if ( QIcon::hasThemeIcon(id) ) {
		icon = QIcon::fromTheme(id);
	} else {
		// This only works on *nix/X11
		QString path = QDir("/usr/share/pixmaps/").absoluteFilePath(id + ".");
		foreach (QString ext, QStringList() << "png" << "jpg" << "xpm"  ) {
			if ( QFile::exists(path + ext) ) {
				icon = QIcon(path + ext);
				break;
			}
		}
	}

	if ( icon.isNull() && QFile::exists(":icons/" + id + ".png") ) {
		icon = QIcon(":icons/" + id + ".png");
	}

	if ( icon.isNull() ) {
		icon = QIcon(":icons/dialog-information.png");
	}

	*size = icon.actualSize(requestedSize);
	QPixmap pix = icon.pixmap(requestedSize);

	if ( !pix.isNull() ) {
		return pix;
	}

	// The pixmap is null
	//
	if ( icon.availableSizes().isEmpty() ) {
		pix = icon.pixmap(32);
	} else {
		pix = icon.pixmap(icon.availableSizes().last());
	}
	return pix;
}
