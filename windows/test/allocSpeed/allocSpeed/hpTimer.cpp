#include "StdAfx.h"

#define LIB_EXPORT

#include "hpTimer.h"


hpTimer::hpTimer(void)
{
	m_iThrdPriority = GetThreadPriority(GetCurrentThread());
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	QueryPerformanceFrequency(&m_liPerFreq);

	//reset();
}


hpTimer::~hpTimer(void)
{
	SetThreadPriority(GetCurrentThread(), m_iThrdPriority);
}


