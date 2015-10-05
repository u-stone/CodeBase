#pragma once

#include <afxmt.h>

class WInternet
{
public:
	WInternet(void);
	virtual ~WInternet(void);

private:
	static DWORD m_dwLastError;
	static CString m_strErrorLastMsg;
	static CCriticalSection m_Sync;
	static BYTE toHex(const BYTE &x);
public:
	static void ConvertUtf8ToGBK(CString& strUtf8);
	static CString URLEncode(CString sIn);
public:
	/************************************************************************/
	/* 表示从URL地址下载一个文件到DestFile                                  */
	/************************************************************************/
	static BOOL DownloadFile(LPCSTR lpURL,LPCSTR lpDestFile);
	//用Get的方法从服务器上面获取信息
	static CString GetFromWeb(LPCSTR lpUrl);
	static DWORD   GetLastError();
	static CString GetLastErrorMsg();
	//------------------------------------------------------------------ 
	// @ProxyType 代理类型，如HTTP,SOCKS,FTP... 
	// @ProxyAddr 返回代理服务器的地址 
	// @ProxyPort 返回代理服务器的端口 
	// 返回值 
	// 0: 无错误 
	// -1: 查询IE的代理失败 
	// -2: 无此类型的代理 
	// -3: 其他错误 
	// -4: 无代理设置信息 
	static int GetIEProxy(CString ProxyType,CString &ProxyAddr,int &ProxyPort);
};
