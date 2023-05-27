/******************************************************************************
	RTimer.h
	
	•ÏX—š—ğ
	Ver 4.01	2012/04/27	b’è”Å for LINUX 64bit Version
******************************************************************************/

#ifndef	RTIMER_H_
#define	RTIMER_H_

#include "R_time.h"

class RTimer
{
public:
	RTimer()		{	R_time_start(&m_timer);	}
//	virtual	~RTimer();
	void start()	{	R_time_start(&m_timer);	}
	int	read()		{	return R_time_read(&m_timer);	}
private:
	RTIME	m_timer;
};

#endif	// RTIMER_H_
