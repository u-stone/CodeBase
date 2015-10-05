#include "serviceRoutine.h"
#include "ServiceImpl.h"
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

HANDLE hServiceStopEvent;

void serviceRoutineRun(DWORD argc, TCHAR* argv[]){
	if (!reportStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 3000))
		goto clean;

	hServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hServiceStopEvent == NULL)
		goto clean;
	HANDLE hEvent[2] = {0, 0};
	hEvent[0] = hServiceStopEvent;
	if (!reportStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 3000))
		goto clean;
	hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hEvent[1] == NULL)
		goto clean;
	if (!reportStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 3000)){
		goto clean;
	}
	SECURITY_ATTRIBUTES *pSD = (PSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
	if (pSD == NULL)
		goto clean;
	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
		goto clean;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = TRUE; //why?

	if (!createMyDACL(&sa)){
		return ;
	}
	if (!reportStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 3000))
		goto clean;

	TCHAR* pipename = L"\\\\.\\pipe\\simple";
	for (DWORD index = 1; index < argc - 1; ++index){
		if (((*(argv[index]) == L'/') || (*(argv[index]) == L'-')) && 
			(!_tcsicmp(L"pipe", argv[index] + 1)) && (index + 1 < argc)){
			pipename = argv[++index];
		}
	}

	HANDLE hPipe = CreateNamedPipe(pipename,
		 FILE_FLAG_OVERLAPPED | PIPE_ACCESS_DUPLEX,
		 PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		 1,
		 0,
		 0,
		 1000,
		 &sa);
	if (hPipe == INVALID_HANDLE_VALUE){
		add2MsgLog(TEXT("unable to create named pipe"));
		goto clean;
	}
	if (!reportStatusToSCM(SERVICE_RUNNING, NO_ERROR, 0)){
		goto clean;
	}

	//finished initialize

	OVERLAPPED os;
	BOOL bRet;
	DWORD cbRead, cbWritten;
	DWORD dwWait;
	TCHAR szIn[80];
	TCHAR szOut[sizeof(szIn)/sizeof(szIn[0]) + 100];
	for (;;){
		memset(&os, 0, sizeof(os));
		os.hEvent = hEvent[1];
		ResetEvent(hEvent[1]);

		ConnectNamedPipe(hPipe, &os);

		if (GetLastError() == ERROR_IO_PENDING){
			DWORD dwWait = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);
			if (dwWait != WAIT_OBJECT_0 + 1){
				break;
			}
		}

		memset(&os, 0, sizeof(os));
		os.hEvent = hEvent[1];
		ResetEvent(hEvent[1]);

		memset(szIn, '\0', sizeof(szIn));
		
		bRet = ReadFile(hPipe, szIn, sizeof(szIn), &cbRead, &os);
		if (!bRet && (GetLastError() == ERROR_IO_PENDING)){
			dwWait = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);
			if (dwWait != WAIT_OBJECT_0 + 1)
				break;
		}

		StringCchPrintf(szOut, sizeof(szOut), TEXT("Hello %s"), szIn);

		memset(szOut, 0, sizeof(szOut));
		os.hEvent = hEvent[1];
		ResetEvent(hEvent[1]);

		bRet = WriteFile(hPipe, szOut, sizeof(szOut), &cbWritten, &os);
		if (!bRet && (GetLastError() == ERROR_IO_PENDING)){
			dwWait = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);
			if (dwWait != WAIT_OBJECT_0 + 1)
				break;
		}
		DisconnectNamedPipe(hPipe);
	}
clean:
	if (hPipe != INVALID_HANDLE_VALUE)
		CloseHandle(hPipe);
	if (hServiceStopEvent)
		CloseHandle(hServiceStopEvent);
	if (hEvent[1])
		CloseHandle(hEvent[1]);
	if (pSD)
		free(pSD);
}

void serviceRoutineStop(){
	if (hServiceStopEvent)
		SetEvent(hServiceStopEvent);
}