#pragma once
#include <Windows.h>

/*
    这是一个高效计时的工具类，自调用起，会提高调用线程的优先级到最高
*/
class hpTimer
{
public:
	hpTimer(void);
	~hpTimer(void);

public:

	//自上次计时开始，使用多少时间（包含线程调度造成的休眠），下面是3种时间单位的函数

	__int64 now_ms(){//毫秒
		QueryPerformanceCounter(&m_liPerCur);
		m_iDur = (m_liPerCur.QuadPart - m_liPerStart.QuadPart) * 1000;
		m_liPerStart.QuadPart = m_liPerCur.QuadPart;
		return m_iDur / m_liPerFreq.QuadPart;
	}
	
	__int64 now_us(){//微秒
		QueryPerformanceCounter(&m_liPerCur);
		m_iDur = (m_liPerCur.QuadPart - m_liPerStart.QuadPart) * 1000000;
		m_liPerStart.QuadPart = m_liPerCur.QuadPart;
		return m_iDur / m_liPerFreq.QuadPart;
	}


	__int64 now_ns(){//纳秒
		QueryPerformanceCounter(&m_liPerCur);
		m_iDur = (m_liPerCur.QuadPart - m_liPerStart.QuadPart) * 1000000000;
		m_liPerStart.QuadPart = m_liPerCur.QuadPart;
		return m_iDur / m_liPerFreq.QuadPart;
	}

	//自上次计时开始，使用多少CPU周期
	unsigned __int64 now_cpu_cycle(){
		QueryThreadCycleTime(GetCurrentThread(), &m_uiCPUCycleCur);
		m_uiDur = m_uiCPUCycleCur - m_uiCPUCycleStart;
		m_uiCPUCycleStart = m_uiCPUCycleCur;
		return m_uiDur;
	}
	
	//重新开始CPU计时和时间计时
	void reset(){
		QueryPerformanceCounter(&m_liPerStart);
		QueryThreadCycleTime(GetCurrentThread(), &m_uiCPUCycleStart);		
	}

private:
	__int64          m_iDur;              
	unsigned __int64 m_uiDur;             //CPU使用周期数

	LARGE_INTEGER    m_liPerFreq;

	LARGE_INTEGER    m_liPerStart;
	LARGE_INTEGER    m_liPerCur;

	unsigned __int64 m_uiCPUCycleStart;
	unsigned __int64 m_uiCPUCycleCur;

	int              m_iThrdPriority;
};

