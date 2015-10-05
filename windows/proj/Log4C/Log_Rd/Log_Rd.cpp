// Log_Rd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LogRd.h"
//#include <iostream>
#include <vector>
#include <string>
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
{
	char buf[MEMUNIT_SIZE] = {0};
	LogRd::getSingleton()->setpid("LogWr");
	//std::vector<std::string> vbuf;
	//vbuf.reserve(640000);
	int count = 0;
	HANDLE hMem = GlobalAlloc(GHND, 400<<20);
	char* pBuf = (char*)GlobalLock(hMem);
	int len = 0;
	int len_t = 0;
	time_t t1 = time(0);
	while (time(0) - t1 < 11){
		if (LogRd::getSingleton()->get(buf)){
			//vbuf.push_back(buf);
			len_t = strlen(buf);
			memcpy(pBuf + len, buf, len_t);
			len += len_t;
		}
		//t1 = time(0);
	}
	LogRd::release();

	wchar_t filepath[] = L"F:\\Code_On_Devfan\\windows\\proj\\Log4C\\Debug\\rd.log";
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = 0;
	sa.lpSecurityDescriptor = 0;
	HANDLE hFile = CreateFile(filepath, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	unsigned long dwWrtten = 0;
	BOOL bRet = WriteFile(hFile, pBuf, len, &dwWrtten, 0);
	CloseHandle(hFile);
	GlobalUnlock(pBuf);
	GlobalFree(hMem);
	return 0;
}

