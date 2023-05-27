#ifndef RIMAGEVIEW_H
#define RIMAGEVIEW_H

#include <QGraphicsView>
#include <QPointF>

class RInspectImageView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit RInspectImageView(QWidget* parent = 0);

signals:

public slots:

protected:
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

signals:
	void MousePositon(QPointF point);
	void signalWheelEvent(QWheelEvent* event);
	void signalScaleChanged(qreal value);

public slots:
	void Enlarge();
	void Shrink();
	void FitIn() { setScale(0); }
	void DotByDot() { m_zoom_value = 0; setScale(1); }
	void setScale(qreal scale);

private:
	int m_zoom_value;
	int m_zoom_step;
};

#endif // RIMAGEVIEW_H
