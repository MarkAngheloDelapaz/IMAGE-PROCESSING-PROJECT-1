#ifndef RQTIMAGE_H
#define RQTIMAGE_H

#include <QImage>

class RQtImage : public QImage
{
public:
	explicit RQtImage();
	
signals:
	
public slots:
	
public:
	static QImage fromGrayAddressPage(int page);
	static QImage fromBinaryAddressPage(int page);
};

#endif // RQTIMAGE_H
