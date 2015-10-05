#include "StdAfx.h"
#include "cpuPCtrl.h"


cpuPCtrl::cpuPCtrl(void)
{
	m_nPressure = 0;
	m_bStart = false;
	m_hMainWorkerThrd = 0;
}


cpuPCtrl::~cpuPCtrl(void)
{
	setQuit(true);
	WaitForSingleObject(m_hMainWorkerThrd, INFINITE);
}

void cpuPCtrl::setPressure(DWORD value)
{
	if (value >= 0 && value <= 100)
		m_nPressure = int(value * 0.87);  //排除非核心代码带来的干扰
}

inline DWORD cpuPCtrl::getPressure()
{
	return m_nPressure;
}

unsigned int __stdcall worker(void* p){
	cpuPCtrl* param = (cpuPCtrl*)p;
	DWORD ticktalk = GetTickCount();
	while (1){
		if (param->getQuit())
			break;
		if (!param->getStart()){
			Sleep(1000);
			continue;
		}
		while (GetTickCount() - ticktalk <= param->getPressure())
			;
		Sleep(100 - param->getPressure());
		ticktalk = GetTickCount();
	}
	return 0;
}


unsigned int __stdcall mainThread(void *p){
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	unsigned int id;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	int nProcessor = si.dwNumberOfProcessors;
	HANDLE* hThrds = new HANDLE[nProcessor];
	int thrdAffanity = 1;
	for (int i = 0; i < nProcessor; ++i){
		hThrds[i] = (HANDLE)_beginthreadex(&sa, 0, worker, p, THREAD_SUSPEND_RESUME, &id);
		SetThreadAffinityMask(hThrds[i], 1 << i);
		ResumeThread(hThrds[i]);
	}
	WaitForMultipleObjects(nProcessor, hThrds, TRUE, INFINITE);
	return 0;
}

bool cpuPCtrl::start()
{
	if (m_hMainWorkerThrd != 0){
		m_bStart = true;
		return false;
	}
	m_bStart = true;
	m_bQuit = false;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	unsigned int id;

	m_hMainWorkerThrd = (HANDLE)_beginthreadex(&sa, 0, mainThread, this, 0, &id);
	if (m_hMainWorkerThrd == 0)
		return false;
	return true;
}

void cpuPCtrl::setQuit(bool bQuit)
{
	m_bQuit = bQuit;
}

bool cpuPCtrl::getQuit()
{
	return m_bQuit;
}

//DWORD cpuPCtrl::getPeriod()
//{
//	return m_dwPeriod;
//}

