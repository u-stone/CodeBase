#include "stdafx.h"
#include "ServiceImpl.h"
#include "serviceRoutine.h"
#include <Sddl.h>

//////////////////////////////////////////////////////////////////////////
//static variable
SERVICE_STATUS_HANDLE s_sHandle = 0;
SERVICE_STATUS        s_status;
DWORD                 s_dwErr = 0;
BOOL                  bDebug;
TCHAR                 szError[256];
SERVICE_TABLE_ENTRY sDispatchTable[]={
	{SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)service_main},
	{NULL, NULL}
};

BOOL reportStatusToSCM(
	DWORD dwCurrentCode, 
	DWORD dwWin32ExitCode, 
	DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	BOOL bRet = TRUE;

	if (!bDebug){
		if (dwCurrentCode == SERVICE_START_PENDING)
			s_status.dwControlsAccepted = 0;
		else 
			s_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

		s_status.dwCurrentState = dwCurrentCode;
		s_status.dwWin32ExitCode = dwWin32ExitCode;
		s_status.dwWaitHint = dwWaitHint;

		if (dwCurrentCode == SERVICE_RUNNING || 
			dwCurrentCode == SERVICE_STOPPED)
		{
			s_status.dwCheckPoint = 0;
		} else {
			s_status.dwCheckPoint = dwCheckPoint++;
		}

		bRet = SetServiceStatus(s_sHandle, &s_status);
		if (!bRet){
			add2MsgLog(L"SetServiceStatus");
		}
	}
	return bRet;
}

void WINAPI service_ctrl(DWORD ctrlCode){
	switch (ctrlCode)
	{
	case SERVICE_CONTROL_STOP:
		reportStatusToSCM(SERVICE_STOP_PENDING, NO_ERROR, 0);
		serviceStop();
		break;
		
	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		break;
	}
	reportStatusToSCM(s_status.dwCurrentState, NO_ERROR, 0);
}


void WINAPI service_main(int argc, TCHAR* argv[]){
	s_sHandle = RegisterServiceCtrlHandler(SERVICE_NAME, service_ctrl);
	if (!s_sHandle){
		goto clean;
	}
	s_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	s_status.dwServiceSpecificExitCode = 0;

	if (!reportStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 3000))
		goto clean;

	serviceStart(argc, argv);

clean:
	if (s_sHandle){
		reportStatusToSCM(SERVICE_STOPPED, s_dwErr, 0);
	}
}


void installService(){
	SC_HANDLE schService;
	SC_HANDLE schSCManager;
	TCHAR path[512];
	if (GetModuleFileName(NULL, path, 512) == 0){
		add2MsgLog(_T("unable to get file path."));
		return ;
	}
	schSCManager = OpenSCManager(NULL, NULL, 
		SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
	if (schSCManager){
		schService = CreateService(schSCManager,
			                       SERVICE_NAME,
								   SERVICE_DISPLAYNAME,
								   SERVICE_QUERY_STATUS,
								   SERVICE_WIN32_OWN_PROCESS,
								   SERVICE_DEMAND_START,
								   SERVICE_ERROR_NORMAL,
								   path,
								   NULL,
								   NULL,
								   SZDEPENDENCIES,
								   NULL,
								   NULL);
		if (schService){
			add2MsgLog(_T("installed"));
			CloseServiceHandle(schService);
		} else {
			add2MsgLog(_T("create service handle failed"));
		}
		CloseServiceHandle(schSCManager);
	} else {
		add2MsgLog(_T("add to scm failed"));
	}
}

void removeService(){
	SC_HANDLE schManager;
	SC_HANDLE schService;
	schManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (schManager){
		schService = OpenService(schManager, 
			SERVICE_NAME, DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
		if (schService){
			if (ControlService(schService, SERVICE_CONTROL_STOP, &s_status)){
				add2MsgLog(_T("stoping service frame ..."));
				Sleep(1000);
			} 
			while (QueryServiceStatus(schService, &s_status)){
				if (s_status.dwCurrentState == SERVICE_STOP_PENDING){
					_tprintf(L".");
					Sleep(1000);
				} else {
					break;
				}
			}
			if (s_status.dwCurrentState == SERVICE_STOP){
				_tprintf(_T("\n%s stopped.\n"), SERVICE_NAME);
			} else {
				_tprintf(_T("\n%s failed to stop. \n"), SERVICE_NAME);
			}

			if (DeleteService(schService))
				_tprintf(_T("%s removed."), SERVICE_NAME);
			else
				_tprintf(_T("Delete service failed - %s\n"), GetLastErrorText(szError, 256));

			CloseServiceHandle(schService);
		} else {
			_tprintf(_T("OpenService failed - %s\n"), GetLastErrorText(szError, 256));
		}

		CloseServiceHandle(schManager);
	} else {
		add2MsgLog(_T("connect scm failed"));
	}
}

void debugService(int argc, TCHAR* argv[]){
	bDebug = TRUE;

	int pArgc;
	TCHAR** pArgv = 0;
#ifdef UNICODE
	pArgv = CommandLineToArgvW(GetCommandLineW(), &pArgc);
	if (pArgv == NULL){
		_tprintf(_T("CommandLineToArgv returned NULL \n"));
		return ;
	}
#else
	pArgc = (DWORD)argc;
	pArgv = argv;
#endif

	_tprintf(_T("Debugging %s.\n"), SERVICE_DISPLAYNAME);
	SetConsoleCtrlHandler(ControlHandler, TRUE);
	serviceStart(pArgc, pArgv);

#ifdef UNICODE
	GlobalFree(pArgv);
#endif
}

BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_BREAK_EVENT:
	case CTRL_C_EVENT:
		_tprintf(_T("Stopping %s."), SERVICE_DISPLAYNAME);
		serviceStop();
		return TRUE;
		break;
	}
	return FALSE;
}

void serviceStart(DWORD argc, TCHAR* argv[]){
	serviceRoutineRun(argc, argv);
}

void serviceStop(){
	serviceRoutineStop();
}

BOOL createMyDACL(SECURITY_ATTRIBUTES *pSA)
{
	TCHAR* sd = 
		L"D:"
		L"(D;OICI;GA;;;BG)"
		L"(D;OICI;GA;;;AN)"
		L"(A;OICI;GRGWGX;;;AU)"
		L"(A;OICI;GA;;;BA)";
	if (sd == NULL){
		return false;
	}

	return ConvertStringSecurityDescriptorToSecurityDescriptor(
		sd, 
		SDDL_REVISION, 
		&pSA->lpSecurityDescriptor, 
		NULL);

}

void add2MsgLog(TCHAR* log)
{
	TCHAR msg[sizeof(SERVICE_NAME)/sizeof(TCHAR) + 100];
	HANDLE hEventSrouce;
	const TCHAR* logStr[2];
	DWORD dwErr;
	if (!bDebug){
		dwErr = GetLastError();
		hEventSrouce = RegisterEventSource(NULL, SERVICE_NAME);
		_stprintf_s(msg, sizeof(msg)/sizeof(msg[0]), _T("%s error: %d"), SERVICE_NAME, dwErr);
		logStr[0] = msg;
		logStr[1] = log;

		if (hEventSrouce){
			ReportEvent(hEventSrouce,
				EVENTLOG_ERROR_TYPE,
				0,
				0,
				NULL,
				2,
				0,
				logStr,
				NULL);
			DeregisterEventSource(hEventSrouce);

		}
	}
}

void startDispatch()
{
	if (!StartServiceCtrlDispatcher(sDispatchTable)){
		add2MsgLog(_T("StartServiceDispatcher failed"));
	}
}

TCHAR* GetLastErrorText(TCHAR *buf, DWORD dwSize){
	DWORD dwRet;
	TCHAR* tmp = NULL;
	dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		GetLastError(),
		LANG_NEUTRAL,
		(TCHAR*)tmp,
		0,
		NULL);
	if (!dwRet || (long)dwSize < (long)dwRet + 14){
		buf[0] = '\0';
	} else {
		if (NULL != tmp){
			tmp[lstrlen(tmp) - 2] = _T('\0');
			_stprintf_s(buf, dwSize, _T("%s (0x%x)"), tmp, GetLastError());
		}
	}
	if (NULL != tmp){
		LocalFree((HLOCAL)tmp);
	}
	return buf;
}


