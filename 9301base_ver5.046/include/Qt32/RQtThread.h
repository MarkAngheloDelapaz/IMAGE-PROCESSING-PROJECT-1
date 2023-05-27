#ifndef RQTTHREAD_H
#define RQTTHREAD_H

#include <QThread>

class RQtThread : public QThread
{
	Q_OBJECT
public:
	explicit RQtThread(QObject* parent = 0);
	
signals:
	
public slots:
	void start(Priority priority = InheritPriority);
	void stop();
	
private:
	volatile bool m_stopped;
	
protected:
	bool stopped() { return m_stopped; }
};

#endif // RQTTHREAD_H
