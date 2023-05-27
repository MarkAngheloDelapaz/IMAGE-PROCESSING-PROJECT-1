#ifndef RInspectView_H
#define RInspectView_H

#include "R_stdlib.h"

#include "RInspect/RInspectImageView.h"
#include "RInspect/RInspectViewTeachItem.h"

class RInspectView : public RInspectImageView
{
	Q_OBJECT
public:
	explicit RInspectView(QWidget* parent = 0);
	~RInspectView();

signals:

public slots:
	void updateitem();

protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	int	m_senseFlag;
	int m_sensingFlag;
	int m_prevSenseFlag;
	bool m_pressed;
	QPointF m_pos;
	QPointF m_startPos;
	R_RECT m_startRect;

signals:
	void updated();
	void changedata();
	void updateRect();
	void updateRect(int flag);

public:
	R_RECT* m_rect;
	QList<RInspectViewTeachItem *> m_teachitem;
	RInspectViewTeachItem *m_senseitem;
	RInspectViewTeachItem *m_pressitem;

	void SetTeachItem(RInspectViewTeachItem *item);
	void RemoveTeachItem(RInspectViewTeachItem *item);
};

#endif // RInspectView_H
