
#ifndef CRASHDUMP_H
#define CRASHDUMP_H

//////////////////////////////////////////////////////////////////////////
/*
   作用：
       程序崩溃的时候创建内存转储文件CrashDump.dmp，创建的位置同可执行文件位于同一路径下。
   用法：
       该类的使用方法是把这个文件添加到VC工程中即可，并include在一个使用的cpp/h文件中。
   原理：
       该类的工作原理是:VC编译器会首先创建全局变量和静态变量。
   适用范围：
       VC写的Win32程序
*/
//////////////////////////////////////////////////////////////////////////

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

class CrashDump{
public:
	CrashDump(){
		m_pOldEF = NULL;
		m_pOldEF = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashFilter);
	}
	~CrashDump(){
		SetUnhandledExceptionFilter(m_pOldEF);
	}
	static LONG CrashFilter(PEXCEPTION_POINTERS pep){
		TCHAR strFilePath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, strFilePath, MAX_PATH);
		TCHAR *pFind = _tcsrchr(strFilePath, '\\');
		if (pFind){
			SYSTEMTIME st;
			GetLocalTime(&st);
			TCHAR buf[300] = {0};
			TCHAR filename[MAX_PATH] = {0};
#ifdef UNICODE
			wcscpy_s(filename, pFind + 1);
			swprintf_s(buf, sizeof(buf) / sizeof(buf[0]), TEXT("%s %d-%d-%d--%d-%d-%d %d.dmp"), filename,
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
			strcpy_s(filename, pFind + 1);
			sprintf_s(buf, sizeof(buf) / sizeof(buf[0]), TEXT("%s %d-%d-%d--%d-%d-%d %d.dmp"), filename,
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#endif

			*(pFind+1) = 0;
			_tcscat_s(strFilePath, buf);
			CreateDumpFile(strFilePath, pep);
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
	static void CreateDumpFile(TCHAR* lpstrDumpFilePathName, PEXCEPTION_POINTERS pException){
		HANDLE hFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, 
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE){
			MINIDUMP_EXCEPTION_INFORMATION mei;
			mei.ExceptionPointers = pException;
			mei.ClientPointers = TRUE;
			mei.ThreadId = GetCurrentThreadId();

			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mei, NULL, NULL);

			CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
		} else {
			OutputDebugString(_T("Create Dump file failed!"));
		}
	}
private:
	LPTOP_LEVEL_EXCEPTION_FILTER m_pOldEF;
};

static CrashDump g_CrashDump;

#endif