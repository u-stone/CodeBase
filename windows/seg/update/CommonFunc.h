#pragma once
#include <WinVer.h>

CString GetProductVersion(LPCTSTR lpszFileName)     
{   
	CString strVersion = "";   
	TCHAR szSrcfn[MAX_PATH]; // Ҫ��ȡ��Ϣ���ļ�   
	if(lpszFileName == NULL)   
		::GetModuleFileName(NULL, szSrcfn, sizeof(szSrcfn));   
	else   
		lstrcpy(szSrcfn, lpszFileName);  
	// ���ļ���Ϣ   
	DWORD dwVerHnd = 0;   
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(szSrcfn, &dwVerHnd);   
	if(dwVerInfoSize)   
	{  
		BYTE *buffer = new BYTE[dwVerInfoSize];
		VS_FIXEDFILEINFO *pFileInfo;
		WORD nVer[4]; 
		if (GetFileVersionInfo(szSrcfn, dwVerHnd, dwVerInfoSize, buffer)) {
			if (VerQueryValue(buffer, _T("\\"), (LPVOID *)&pFileInfo, (PUINT)&dwVerInfoSize)) 
			{				 
				nVer[0] = HIWORD(pFileInfo->dwProductVersionMS);   
				nVer[1] = LOWORD(pFileInfo->dwProductVersionMS);   
				nVer[2] = HIWORD(pFileInfo->dwProductVersionLS);   
				nVer[3] = LOWORD(pFileInfo->dwProductVersionLS);   
				strVersion.Format(_T("%d.%d.%d.%d"), nVer[0], nVer[1],nVer[2],nVer[3]);  
			}
		}
		delete buffer;   
	}   
	return strVersion;   
}
