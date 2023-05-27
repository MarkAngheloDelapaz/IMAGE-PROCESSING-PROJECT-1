#ifndef RQTGRAPHICSVIEW_H
#define RQTGRAPHICSVIEW_H

#include <QGraphicsView>

class RQtGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit RQtGraphicsView(QWidget* parent = 0);

protected:
	void resizeEvent(QResizeEvent* event);
	void showEvent(QShowEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

signals:
	
public slots:
	
private:
	bool m_fitInView;

signals:
	void mouseMoved(QPointF point);

public slots:
	void setFitInView(bool enable = true);
	void setScale(qreal scale);
	void enlarge();
	void shrink();

private:
	int m_zoomValue;
	int m_zoomStep;
	QGraphicsPixmapItem* m_pixmap;
};

#endif // RQTGRAPHICSVIEW_H
