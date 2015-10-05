#include "StdAfx.h"
#include "LogRd.h"

LogRd* LogRd::s_Singleton = 0;

LogRd::LogRd(void)
{
	m_hMemFile = 0;
	m_hReadEvent = 0;
	m_pBuffer = 0;
	m_iRd = 0;
	memset(m_strPID, 0, sizeof(m_strPID));
	init();
}


LogRd::~LogRd(void)
{
	uninit();
}

bool LogRd::get(char* pLog)
{
	DWORD dwRet = WaitForSingleObject(m_hReadEvent, 1);
	if (dwRet == WAIT_TIMEOUT)
		return false;

	ResetEvent(m_hReadEvent);

	//memcpy((void*)&m_iRd, m_pBuffer, 4);
	memcpy((void*)&m_iWr, m_pBuffer + 4, 4);
	//if (m_iRd >= m_iWr)
	//	return false;

	memcpy(pLog, m_pBuffer + 8 + m_iRd * MEMUNIT_SIZE, MEMUNIT_SIZE);
	pLog[MEMUNIT_SIZE - 1] = '\0';

	//InterlockedIncrement(&m_iRd);
	++m_iRd %= TOTAL_SIZE;
	//memcpy(m_pBuffer, (void*)&m_iRd, 4);

	return true;	
}

void LogRd::init()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	wchar_t buf[15] = {0};
	wchar_t *id = L"LogWr";
	swprintf_s(buf, sizeof(buf)/sizeof(buf[0]), L"Event-%s", id);
	m_hReadEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, buf);
	if (m_hReadEvent == 0){
		m_hReadEvent = CreateEvent(&sa, TRUE, FALSE, buf);
	}
	WaitForSingleObject(m_hReadEvent, INFINITE);
	ResetEvent(m_hReadEvent);

	if (m_hMemFile == 0){
		swprintf_s(buf, sizeof(buf)/sizeof(buf[0]), L"File-%s", id);
		m_hMemFile = OpenFileMapping(PAGE_READWRITE, FALSE, buf);
		if (m_hMemFile == 0){
			m_hMemFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
				&sa, PAGE_READWRITE, 0, 
				MEMFILE_SIZE, buf);
		}
		m_pBuffer = (char*)MapViewOfFile(m_hMemFile, FILE_MAP_READ/* | FILE_MAP_WRITE*/, 0, 0, 0);
		if (m_pBuffer == 0)
			throw ;
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


