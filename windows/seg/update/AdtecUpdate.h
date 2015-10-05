#pragma once
#include "mfc_foundation.h"
/*
�������Ϊ�˳���ĸ���ģ��д�ģ���ɹ����У�
1. �رճ��򣬲���������ģ��
2. ������ģ�鱾���Ƿ��и��£�����еĻ���������������ɸ���ģ��ĸ���
*/

class AdtecUpdate
{
private:
	AdtecUpdate(void);
	~AdtecUpdate(void);
public:
	static AdtecUpdate* GetInstance(){
		if (s_Update == NULL){
			s_Update = new AdtecUpdate();
		}
		return s_Update;
	}

	struct UpdateInfo 
	{
		CString strAppName;
		CString strUrl;
		CString strVersionTag;
		CString strVersion;
		CString strExeToRestart;
	};

	void Update(UpdateInfo* pUpdate, void (* pBeforeExit)()){
		BeforeExit(pBeforeExit);
		UpdateImpl(pUpdate->strAppName, pUpdate->strUrl, pUpdate->strVersionTag, pUpdate->strVersion, pUpdate->strExeToRestart);
		CloseCurrentProcess();
	}

	void UpdateSelf(){
		WIN32_FIND_DATA wfd;
		BOOL rValue = FALSE;
		CString strPath = GetModuleDirectory() + _T("UpdateDir\\");
		HANDLE hFind = ::FindFirstFile(strPath, &wfd);
		if ((hFind!=INVALID_HANDLE_VALUE) &&
			(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			rValue = TRUE;
		}
		FindClose(hFind);
		if (!rValue){
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			::CreateDirectory(strPath, &attrib);
		}

		CFileFind finder;
		CString str = GetModuleDirectory() + _T("UpdateDir\\*.*");
		BOOL bWorking = finder.FindFile(str);
		while (bWorking){
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
				continue;
			if (finder.IsDirectory())
				continue;
			str = GetModuleDirectory() + finder.GetFileName();
			if (CopyFile(finder.GetFilePath(), str, FALSE))
				DeleteFile(finder.GetFilePath());
		}
	}

private:
	void BeforeExit(void (* pBeforeExit)()){
		//TODO:���Ҫ�ڳ�����Ҫ�˳���ǰ��һЩ�����Ļ���������������Ӵ���
		if (pBeforeExit != NULL)
			pBeforeExit();
	}

	void CloseCurrentProcess(){
		exit(-1);
	}

	/*
	* �����������˵�� zipupdate.exe $appname=xxxxx$ $url=http://xx,x,x.x.x/xx.zip$ $versiontag=HKMA/SOFTWARE/XXXX/AAA$ $versionvalue=$ $restartexe=XXXXXXX$
	* ��ͬ����֮���� $����
	* appname: ��ʾ���ó�������
	* url: ��ʾ�����ļ��ĵ�ַ���ļ�������zip�ļ�, zip �ļ�����ҿ��� zipupdate.exe Ŀ¼����
	* versiontag: ��ʾ��װUNZIP��ɺ�, �汾ע���·��
	* versionvalue: ��ʾ�汾��Ϣ��VALUEֵ
	* restartexe: ��ʾ������ɺ��������������exe������������exe ���棬��BASE64����

	  ��������
		$appname=��������$ $url=http://127.0.0.1/7z920.7z$ $versiontag=Software\\RxWebStart\\Test$ $versionvalue=7.5$ $restartexe=AstSwitch.exe$ 
	*/
	void UpdateImpl(CString strAppname, CString strUrl, CString strVersionTag, CString strVersion, CString strExeToRestart)
	{
		CString strCommandLine = _T("$appname=") + strAppname + _T("$ ");
		strCommandLine += _T("$url=") + strUrl + _T("$ ");
		strCommandLine += _T("$versiontag=") + strVersionTag + _T("$ ");
		strCommandLine += _T("$versionvalue=") + strVersion + _T("$ ");
		strCommandLine += _T("$restartexe=") + strExeToRestart + _T("$");
		CString csCmdLine = "";
		csCmdLine = csCmdLine + "\""+ GetModuleDirectory() + "UpdateExe.exe\" " + strCommandLine;
		OutputDebugString(_T("Update����") + csCmdLine);
		STARTUPINFO si;
		PROCESS_INFORMATION pi;	
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );	
		if( !CreateProcess( NULL, // No module name (use command line). 
			(LPTSTR)(LPCSTR)csCmdLine,// Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
			) 
		{
			return;
		}
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
private:
	static AdtecUpdate* s_Update;
};
