#include "StdAfx.h"
#include "MLog.h"

MLog* MLog::s_Singleton = 0;
MLog::MLog(void)
{
	m_hMemFile = 0;
	m_hReadEvent = 0;
	init();
}


MLog::~MLog(void)
{
	uninit();
}

void MLog::init()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	//创建内存映射文件，得到缓存
	if (m_hMemFile == 0){
		m_hMemFile = OpenFileMapping(PAGE_READWRITE, FALSE, _T("Log-For-L"));
		if (m_hMemFile == 0){
			m_hMemFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
				                          &sa, PAGE_READWRITE, 0, 
										  MEMFILE_SIZE, _T("Log-For-L"));
		}
		m_pBuffer = MapViewOfFile(m_hMemFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		
	}
	m_hReadEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("Event-For-L"));
	if (m_hReadEvent == 0){
		m_hReadEvent = CreateEvent(&sa, TRUE, FALSE, _T("Event-For-L"));
	}	
}

void MLog::uninit()
{
	if (m_pBuffer)
		UnmapViewOfFile(m_pBuffer);
	if (m_hMemFile)
		CloseHandle(m_hMemFile);
	if (m_hReadEvent){
		CloseHandle(m_hReadEvent);
	}
}

void MLog::trace(const char* strlog, LogClass lc)
{
	if (lc != m_lc) return ;
	memset(m_pBuffer, 0, MEMFILE_SIZE);
	memcpy(m_pBuffer, strlog, strlen(strlog));
	SetEvent(m_hReadEvent);
}