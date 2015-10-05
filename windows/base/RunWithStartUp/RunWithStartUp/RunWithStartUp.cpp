// RunWithStartUp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#ifndef LIB_EXPORT
  #define LIB_EXPORT
#endif

#include "startup.h"
#include "OSInfo.h"
#include <string.h>
using namespace std;

//32位和64位程序对32位和64位操作系统的注册表的访问是不一样的。
//64bit系统下操作注册表的注意事项  http://blog.sina.com.cn/s/blog_43ca06360100h5uv.html

//注意：这里假设本程序只有32位版本，如果将来要添加64位版本，那么IsWow64()判断之外，还要另加判断



//add or remove startup entry for multibyte version
LIB_API bool AddStartupEntryA(const char* keystr, int keyByte, const char* entryPath, int pathByte)
{
	if (keyByte == 0) keyByte = strlen(keystr);
	if (pathByte == 0) pathByte = strlen(entryPath);
	HKEY   hKey;
	char* lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	DWORD value = 0;
	if (IsWow64())
		value = KEY_WOW64_32KEY;
	long lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE | value, &hKey);
	char *pTmp = new char[keyByte + 1];
	memset(pTmp, 0, keyByte + 1);
	memcpy(pTmp, keystr, keyByte);
	bool ret = true;
	if(lRet== ERROR_SUCCESS){
		RegSetValueExA(hKey, pTmp, 0,REG_SZ,(const BYTE*)entryPath, pathByte);
		RegCloseKey(hKey); 
	} else {
		ret = false;
	}
	delete []pTmp;
	pTmp = 0;
	return ret;
}
LIB_API bool RemoveStartupEntryA(const char* keystr)
{
	HKEY   hKey; 
	char* lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	DWORD value = 0;
	if (IsWow64())
		value = KEY_WOW64_32KEY;
	long lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE | value, &hKey); 
	if(lRet== ERROR_SUCCESS){
		RegDeleteValueA(hKey, keystr);
		RegCloseKey(hKey); 
	} else {
		OutputDebugStringA("false");
		return false;
	}
	return true;
}
LIB_API bool QueryStartupEntryA(const char* keystr, int bytes)
{
	if (bytes == 0) bytes = strlen(keystr);
	HKEY   hKey; 
	bool ret = false;
	char* lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	DWORD value = 0;
	if (IsWow64())
		value = KEY_WOW64_32KEY;
  	int len = bytes + 1;
  	char *pTmp = new char[len];
  	memset(pTmp, 0, len);
  	memcpy(pTmp, keystr, len);
	long lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_READ | value, &hKey); 
	if(lRet== ERROR_SUCCESS){
 		lRet = RegQueryValueExA(hKey, pTmp, 0, 0, 0, 0);
 		if (lRet == ERROR_SUCCESS) ret = true;
	}
	delete [] pTmp;
	pTmp = 0;
	return ret;
}

//wide bytes version
LIB_API bool AddStartupEntryW(const wchar_t* keystr, int keyByte, const wchar_t* entryPath, int pathByte)
{
	if (keyByte == 0) keyByte = wcslen(keystr) * sizeof(wchar_t);
	if (pathByte == 0) pathByte = wcslen(entryPath) * sizeof(wchar_t);
	HKEY   hKey; 
	LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	DWORD value = 0;
	if (IsWow64())
		value = KEY_WOW64_32KEY;
	long lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE | value, &hKey);
	wchar_t * pTmp = new wchar_t[keyByte + 2];
	memset(pTmp, 0, keyByte + 2);
	memcpy(pTmp, keystr, keyByte);
	bool ret = true;
	if(lRet== ERROR_SUCCESS){
		RegSetValueExW(hKey, pTmp, 0, REG_SZ,(const BYTE*)entryPath, pathByte);
		RegCloseKey(hKey); 
	} else {
		ret = false;
	}
	delete []pTmp;
	pTmp = 0;
	OutputDebugStringA(ret? "suc" : "failed");
	return ret;
}
LIB_API bool RemoveStartupEntryW(const wchar_t* keystr)
{
	HKEY   hKey; 
	wchar_t* lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	DWORD value = 0;
	if (IsWow64())
		value = KEY_WOW64_32KEY;
	long lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE | value, &hKey); 
	if(lRet== ERROR_SUCCESS){
		RegDeleteValueW(hKey, keystr);
		RegCloseKey(hKey); 
	} else {
		return false;
	}
	return true;
}

LIB_API bool QueryStartupEntryW(const wchar_t* keystr, int bytes)
{
	if (bytes == 0) bytes = wcslen(keystr) * sizeof(wchar_t);
	HKEY   hKey; 
	wchar_t* lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	DWORD value = 0;
	bool ret = false;

	if (IsWow64()) value = KEY_WOW64_32KEY;

	long lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_READ | value, &hKey); 
	if(lRet== ERROR_SUCCESS){
		lRet = RegQueryValueExW(hKey, keystr, 0, 0, 0, 0);
		if (lRet == ERROR_SUCCESS) ret = true;
		RegCloseKey(hKey);
	}
	return ret;
}

