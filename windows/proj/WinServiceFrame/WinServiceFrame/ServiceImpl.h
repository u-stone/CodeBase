#pragma once
#include <Windows.h>


//macro
#define SERVICE_NAME (L"WinServiceFrame")
#define SERVICE_DISPLAYNAME (L"WinServiceFrame-dispaly-name")
#define SZDEPENDENCIES (L"")



//Functions
void WINAPI service_ctrl(DWORD ctrlCode);
void __stdcall service_main(int argc, TCHAR* argv[]);

void installService();
void removeService();
void debugService(int argc, TCHAR* argv[]);
void startDispatch();

BOOL createMyDACL(SECURITY_ATTRIBUTES *pSA);
void serviceStart(DWORD argc, TCHAR* argv[]);
void serviceStop();

BOOL WINAPI ControlHandler(DWORD dwCtrlType);

BOOL reportStatusToSCM(
	DWORD dwCurrentCode, 
	DWORD dwWin32ExitCode, 
	DWORD dwWaitHint);
void add2MsgLog(TCHAR* log);
TCHAR* GetLastErrorText(TCHAR *buf, DWORD dwSize);

