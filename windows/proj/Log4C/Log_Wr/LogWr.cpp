#include "StdAfx.h"
#include "LogWr.h"
#include <string>

LogWr* LogWr::s_Singleton = 0;
LogWr::LogWr(void)
{
	m_hMemFile = 0;
	m_hReadEvent = 0;
	m_iRd = 0;
	m_iWr = 0;
	m_LogIndex = 0;
	init();
}


LogWr::~LogWr(void)
{
	uninit();
}

void LogWr::init()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	//创建内存映射文件，得到缓存
	wchar_t buf[20] = {0};
	wchar_t *id = L"LogWr";
	if (m_hMemFile == 0){
		//int id = GetCurrentProcessId();
		swprintf_s(buf, sizeof(buf)/sizeof(buf[0]), L"File-%s", id);
		m_hMemFile = OpenFileMapping(PAGE_READWRITE, FALSE, buf);
		if (m_hMemFile == 0){
			m_hMemFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
				&sa, PAGE_READWRITE, 0, 
				MEMFILE_SIZE, buf);
		}
		m_pBuffer = (char*)MapViewOfFile(m_hMemFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (m_pBuffer == 0)
			throw;
	}
	swprintf_s(buf, sizeof(buf)/sizeof(buf[0]), L"Event-%s", id);
	m_hReadEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, buf);
	if (m_hReadEvent == 0){
		m_hReadEvent = CreateEvent(&sa, TRUE, FALSE, buf);
	}
	SetEvent(m_hReadEvent); //表示这里已经创建了内存映射文件
}

void LogWr::uninit()
{
	serialize();

	if (m_pBuffer)
		UnmapViewOfFile(m_pBuffer);
	if (m_hMemFile)
		CloseHandle(m_hMemFile);
	if (m_hReadEvent){
		CloseHandle(m_hReadEvent);
	}
}

void LogWr::trace(const char* strlog, LogClass lc)
{
	if (lc != m_lc) return ;
	GetLocalTime(&m_st);
	sprintf_s(m_buf, "%8d |  %2d:%2d:%2d-%3d | %8d | %8d | %s | %s \r\n", ++m_LogIndex, 
		                                                        m_st.wHour,
		                                                        m_st.wMinute,
		                                                        m_st.wSecond,
		                                                        m_st.wMilliseconds,
		                                                        GetCurrentProcessId(), 
													            GetCurrentThreadId(), 
													            s_strlc[lc], strlog);
	m_buf[MEMUNIT_SIZE-1] = '\0';
	memcpy(m_pBuffer + 8 + m_iWr * MEMUNIT_SIZE, m_buf, strlen(m_buf));

	//InterlockedIncrement(&m_iWr);
	if (++m_iWr == TOTAL_SIZE){
		m_iWr = 0;
		serialize();
	}

	memcpy(m_pBuffer+4, (void*)&m_iWr, 4);

	SetEvent(m_hReadEvent);
}

void LogWr::trace(const wchar_t* strlog, LogClass lc)
{
	if (lc != m_lc) return ;
	GetLocalTime(&m_st);
	swprintf_s(m_bufw, sizeof(m_bufw) * sizeof(wchar_t), L"%08d |  %d:%d:%d-%d | %8d | %8d | %s | %s \r\n", 
		                                                        ++m_LogIndex, 
		                                                        m_st.wHour,
																m_st.wMinute,
																m_st.wSecond,
																m_st.wMilliseconds,
		                                                        GetCurrentProcessId(), 
		                                                        GetCurrentThreadId(), 
		                                                        s_strlcw[lc], 
																strlog);
	m_bufw[MEMUNIT_SIZE-1] = '\0';
	memcpy((wchar_t*)m_pBuffer + 8 + m_iWr * MEMUNIT_SIZE, m_bufw, wcslen(m_bufw) * sizeof(wchar_t));

	InterlockedIncrement(&m_iWr);
	if (m_iWr == TOTAL_SIZE){
		m_iWr = 0;   //如果此时此刻读取日志的线程要来读数据，就会有数据被漏掉了
		serialize();
	}

	memcpy(m_pBuffer+4, (void*)&m_iWr, 4);

	SetEvent(m_hReadEvent);
}

void LogWr::serialize()
{
	wchar_t filepath[MAX_PATH] = {0};
	GetModuleFileName(NULL, filepath, MAX_PATH);
	int len = wcslen(filepath);
	while (--len){
		if (filepath[len] == '\\'){
			filepath[len] = '\0';
			break;
		}
	}
	SYSTEMTIME st;
	GetLocalTime(&st);
	wchar_t buf[40] = {0};
	swprintf_s(buf, sizeof(buf) / sizeof(buf[0]), L"\\log\\%d-%d-%d %d-%d-%d %d.log", 
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	
	wcscat_s(filepath, MAX_PATH, buf);
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = 0;
	sa.lpSecurityDescriptor = 0;
	HANDLE hFile = CreateFile(filepath, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return ;
	unsigned long dwWrtten = 0;
	sprintf_s(m_buf, "   No.   |  time        |Process-ID|Thread-ID | Class  | Content \r\n");
	WriteFile(hFile, m_buf, strlen(m_buf), &dwWrtten, 0);
	int index = 0;
	char tmp[MEMUNIT_SIZE];
	while (index < TOTAL_SIZE){
		memcpy(tmp, m_pBuffer + 8 + index++ * MEMUNIT_SIZE, MEMUNIT_SIZE);
		WriteFile(hFile, tmp, strlen(tmp), &dwWrtten, 0);
	}
	CloseHandle(hFile);
}

