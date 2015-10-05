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
	/* ��ʾ��URL��ַ����һ���ļ���DestFile                                  */
	/************************************************************************/
	static BOOL DownloadFile(LPCSTR lpURL,LPCSTR lpDestFile);
	//��Get�ķ����ӷ����������ȡ��Ϣ
	static CString GetFromWeb(LPCSTR lpUrl);
	static DWORD   GetLastError();
	static CString GetLastErrorMsg();
	//------------------------------------------------------------------ 
	// @ProxyType �������ͣ���HTTP,SOCKS,FTP... 
	// @ProxyAddr ���ش���������ĵ�ַ 
	// @ProxyPort ���ش���������Ķ˿� 
	// ����ֵ 
	// 0: �޴��� 
	// -1: ��ѯIE�Ĵ���ʧ�� 
	// -2: �޴����͵Ĵ��� 
	// -3: �������� 
	// -4: �޴���������Ϣ 
	static int GetIEProxy(CString ProxyType,CString &ProxyAddr,int &ProxyPort);
};
