// dllresearce.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <process.h>
#include <Windows.h>



unsigned int __stdcall thrd(void*);

void print(char* pTips, int flag = 0){
	printf(pTips);
	printf(" -- %d\n", flag);
	int count = 3;
	while (count > 0){
		printf("wait for %d second ... : %d\n", count--, flag);
		Sleep(1000);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int param[3] = {1, 2, 0};
	
	thrd((void*)&param[2]);

	unsigned int thrdid = 0;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;

	HANDLE hThread[2];
	printf("start thread %d\n", param[0]);
	hThread[0] = (HANDLE)_beginthreadex(&sa, 0, &thrd, (void*)&param[0], 0, &thrdid);
	if (hThread == 0){
		DWORD dwRet = GetLastError();
		printf("%d\n", dwRet);
	}
	Sleep(500);
	printf("start thread %d\n", param[1]);
	hThread[1] = (HANDLE)_beginthreadex(&sa, 0, &thrd, (void*)&param[1], 0, &thrdid);
	if (hThread == 0){
		DWORD dwRet = GetLastError();
		printf("%d\n", dwRet);
	}
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	
	return 0;
}


unsigned int __stdcall thrd(void* pthrdid)
{
	char buf[100];
	printf("ready for LoadLibrary -- %d", *(int*)(pthrdid));

	HMODULE hModule = LoadLibrary(L"dlltest.dll");
	if (hModule){
		sprintf_s(buf, "ready for FreeLibrary", *(int*)(pthrdid));
		print(buf, *(int*)pthrdid);
		FreeLibrary(hModule);
	}
	
	printf("ready for exit thread %d\n", *(int*)pthrdid);
// 	sprintf_s(buf, "ready for exitting thread -- %d", *(int*)(pthrdid));
// 	print(buf, *(int*)pthrdid);
	return 0;
}


/*
ready for LoadLibrary -- threadid : 0
-----------------------------DLL_PROCESS_ATTACH
ready for FreeLibrary -- 0
wait for 3 second ... : 0
wait for 2 second ... : 0
wait for 1 second ... : 0

-----------------------------DLL_PROCESS_DETACH
ready for exit thread 0
start thread 1
ready for LoadLibrary -- threadid : 1
-----------------------------DLL_PROCESS_ATTACH
ready for FreeLibrary -- 1
wait for 3 second ... : 1
start thread 2

-----------------------------DLL_THREAD_ATTACH
ready for LoadLibrary -- threadid : 2
ready for FreeLibrary -- 2
wait for 3 second ... : 2
wait for 2 second ... : 1
wait for 2 second ... : 2
wait for 1 second ... : 1
wait for 1 second ... : 2
ready for exit thread 1

-----------------------------DLL_THREAD_DETACH

-----------------------------DLL_PROCESS_DETACH
ready for exit thread 2
*/