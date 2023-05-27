#ifndef RInspectViewItem_H
#define RInspectViewItem_H

#include <QGraphicsObject>

#include "R_stdlib.h"

class RInspectViewTeachItem :public QGraphicsObject
{
	Q_OBJECT

public:
	typedef enum{
		TYPE_UNKNOWN,
		TYPE_RECT,
		TYPE_CIRCLE,
		NUM_TYPE
	} RINSPTEACH_TYPE;


	explicit RInspectViewTeachItem(QString str = "", QGraphicsItem* parent = 0);
	explicit RInspectViewTeachItem(R_RECT *rect, QString str = "", QGraphicsItem *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	int GetSize() const;
	int CheckSense(QPointF pos);

	int SetColor(QColor color);

	void press(QPointF pos);
	void release();

	static bool less(const RInspectViewTeachItem *d1,const RInspectViewTeachItem *d2) { return (d1->GetSize() < d2->GetSize()); }
	static bool greater(const RInspectViewTeachItem *d1,const RInspectViewTeachItem *d2) { return (d1->GetSize() > d2->GetSize()); }

signals:
	void valuechanged();

public slots:
	void test();

public:
	RINSPTEACH_TYPE m_type;
	QRectF m_boundingRect;
	void* m_value;
	int m_flag;
	int *m_baseX;
	int *m_baseY;

protected:
	QString m_comment;
	int	m_senseFlag;
	int m_sensingFlag;
	int	m_sensitivity;
	int m_prevSenseFlag;
	bool m_pressed;
	//QPointF m_pos;
	QPointF m_startPos;
	char m_startvalue[128];

	QPen *m_pen;
};

#endif // RInspectViewItem_H
