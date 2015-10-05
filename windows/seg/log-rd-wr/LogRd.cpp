#include "StdAfx.h"
#include "LogRd.h"

LogRd* LogRd::s_Singleton = 0;

LogRd::LogRd(void)
{
	m_hMemFile = 0;
	m_hReadEvent = 0;
	m_pBuffer = 0;
	init();
}


LogRd::~LogRd(void)
{
	uninit();
}

void LogRd::get(void* pLog)
{
	DWORD dwRet = WaitForSingleObject(m_hReadEvent, 5000);
	if (dwRet == WAIT_TIMEOUT)
		return ;
	ResetEvent(m_hReadEvent);
	memcpy(pLog, m_pBuffer, 100);
	
}

void LogRd::init()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	if (m_hMemFile == 0){
		m_hMemFile = OpenFileMapping(PAGE_READWRITE, FALSE, _T("Log-For-L"));
		if (m_hMemFile == 0){
			m_hMemFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
				&sa, PAGE_READWRITE, 0, 
				MEMFILE_SIZE, _T("Log-For-L"));
		}
		m_pBuffer = MapViewOfFile(m_hMemFile, FILE_MAP_READ, 0, 0, 0);
	}
	m_hReadEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("Event-For-L"));
	if (m_hReadEvent == 0){
		m_hReadEvent = CreateEvent(&sa, TRUE, FALSE, _T("Event-For-L"));
	}
}
void LogRd::uninit()
{
	if (m_pBuffer)
		UnmapViewOfFile(m_pBuffer);
	if (m_hMemFile)
		CloseHandle(m_hMemFile);
	if (m_hReadEvent){
		CloseHandle(m_hReadEvent);
	}
}