#include "iconprovider.h"
#include <QtGui>

QStringList IconProvider::extensions = QStringList() << "png" << "jpg" << "xpm" << "ico";

IconProvider::IconProvider()
:QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
}

QPixmap IconProvider::requestPixmap(const QString& id, QSize *size, const QSize &requestedSize)
{
	QIcon icon;
	QFileInfo fi(id);
	if ( fi.isAbsolute() && fi.exists() ) {
		icon = QIcon(fi.absoluteFilePath());
	} else if ( QIcon::hasThemeIcon(id) ) {
		icon = QIcon::fromTheme(id);
	} else {
		// This only works on *nix/X11
		QString path = QDir("/usr/share/pixmaps/").absoluteFilePath(id + ".");
		foreach (QString ext, IconProvider::extensions  ) {
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

	QSize rsize = requestedSize;
	if ( !rsize.isValid() ) {
		// FIXME: can we find a better default?
		rsize = QSize(48, 48);
	}

	*size = icon.actualSize(rsize);
	QPixmap pix = icon.pixmap(rsize);

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
