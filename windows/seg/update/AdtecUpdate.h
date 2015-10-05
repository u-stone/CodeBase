#pragma once
#include "mfc_foundation.h"
/*
这个类是为了程序的更新模块写的，完成功能有：
1. 关闭程序，并启动更新模块
2. 检查更新模块本身是否有更新，如果有的话，帮助主程序完成更新模块的更新
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
		//TODO:如果要在程序需要退出以前做一些动作的话，可以在这里添加代码
		if (pBeforeExit != NULL)
			pBeforeExit();
	}

	void CloseCurrentProcess(){
		exit(-1);
	}

	/*
	* 升级程序参数说明 zipupdate.exe $appname=xxxxx$ $url=http://xx,x,x.x.x/xx.zip$ $versiontag=HKMA/SOFTWARE/XXXX/AAA$ $versionvalue=$ $restartexe=XXXXXXX$
	* 不同参数之间用 $隔开
	* appname: 表示引用程序名字
	* url: 表示下载文件的地址，文件必须是zip文件, zip 文件将会揭开在 zipupdate.exe 目录下面
	* versiontag: 表示安装UNZIP完成后, 版本注册表路径
	* versionvalue: 表示版本信息的VALUE值
	* restartexe: 表示整个完成后重新启动程序的exe名，参数跟在exe 后面，用BASE64编码

	  例子如下
		$appname=建设银行$ $url=http://127.0.0.1/7z920.7z$ $versiontag=Software\\RxWebStart\\Test$ $versionvalue=7.5$ $restartexe=AstSwitch.exe$ 
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
		OutputDebugString(_T("Update参数") + csCmdLine);
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
