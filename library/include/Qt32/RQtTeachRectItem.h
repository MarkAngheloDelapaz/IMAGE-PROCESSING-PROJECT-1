#ifndef RQTTEACHRECTITEM_H
#define RQTTEACHRECTITEM_H

#include <QGraphicsObject>

class RQtTeachRectItem : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit RQtTeachRectItem(QGraphicsItem* parent = 0);
	
public:
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
	QRect m_rect;
	bool m_left;
	bool m_top;
	bool m_right;
	bool m_bottom;
	bool m_all;
	int m_outerMargin;
	int m_innerMargin;
	QRect m_range;
	QRectF m_pressedRect;
	bool m_teaching;
	
signals:
	void changed();
	void released();
	
public:
	QRect rect() const { return m_rect; }
	void setRect(const QRect& rectangle) { m_rect = rectangle; }
	void setRect(int x, int y, int width, int height) { m_rect.setRect(x, y, width, height); }
	void setRange(QRect& range);
	
public slots:
	void start(bool start = true);
	void stop() { start(false); }
};

#endif // RQTTEACHRECTITEM_H
