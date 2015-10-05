#pragma once
#include <Windows.h>

/*
    ����һ����Ч��ʱ�Ĺ����࣬�Ե����𣬻���ߵ����̵߳����ȼ������
*/
class hpTimer
{
public:
	hpTimer(void);
	~hpTimer(void);

public:

	//���ϴμ�ʱ��ʼ��ʹ�ö���ʱ�䣨�����̵߳�����ɵ����ߣ���������3��ʱ�䵥λ�ĺ���

	__int64 now_ms(){//����
		QueryPerformanceCounter(&m_liPerCur);
		m_iDur = (m_liPerCur.QuadPart - m_liPerStart.QuadPart) * 1000;
		m_liPerStart.QuadPart = m_liPerCur.QuadPart;
		return m_iDur / m_liPerFreq.QuadPart;
	}
	
	__int64 now_us(){//΢��
		QueryPerformanceCounter(&m_liPerCur);
		m_iDur = (m_liPerCur.QuadPart - m_liPerStart.QuadPart) * 1000000;
		m_liPerStart.QuadPart = m_liPerCur.QuadPart;
		return m_iDur / m_liPerFreq.QuadPart;
	}


	__int64 now_ns(){//����
		QueryPerformanceCounter(&m_liPerCur);
		m_iDur = (m_liPerCur.QuadPart - m_liPerStart.QuadPart) * 1000000000;
		m_liPerStart.QuadPart = m_liPerCur.QuadPart;
		return m_iDur / m_liPerFreq.QuadPart;
	}

	//���ϴμ�ʱ��ʼ��ʹ�ö���CPU����
	unsigned __int64 now_cpu_cycle(){
		QueryThreadCycleTime(GetCurrentThread(), &m_uiCPUCycleCur);
		m_uiDur = m_uiCPUCycleCur - m_uiCPUCycleStart;
		m_uiCPUCycleStart = m_uiCPUCycleCur;
		return m_uiDur;
	}
	
	//���¿�ʼCPU��ʱ��ʱ���ʱ
	void reset(){
		QueryPerformanceCounter(&m_liPerStart);
		QueryThreadCycleTime(GetCurrentThread(), &m_uiCPUCycleStart);		
	}

private:
	__int64          m_iDur;              
	unsigned __int64 m_uiDur;             //CPUʹ��������

	LARGE_INTEGER    m_liPerFreq;

	LARGE_INTEGER    m_liPerStart;
	LARGE_INTEGER    m_liPerCur;

	unsigned __int64 m_uiCPUCycleStart;
	unsigned __int64 m_uiCPUCycleCur;

	int              m_iThrdPriority;
};

