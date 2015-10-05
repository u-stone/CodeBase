#ifndef _STARTUP_H
#define _STARTUP_H

#ifdef LIB_EXPORT
  #define LIB_API __declspec(dllexport) 
#else
  #define LIB_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif
	/*
	������
	   keystr    ע�����
	   entryPath ע������ֵ
	   keyByte   ע������ַ����ĳ��ȣ����ֽڼ��㣬����Ϊ0
	   pathByte  ע������ֵ���ĳ��ȣ����ֽڼ��㣬����Ϊ0
	*/

	//add or remove startup entry for multibyte version
	LIB_API bool AddStartupEntryA(const char* keystr, int keyByte, const char* entryPath, int pathByte);
	LIB_API bool RemoveStartupEntryA(const char* keystr);
	LIB_API bool QueryStartupEntryA(const char* keystr, int bytes);

	//wide bytes version
	LIB_API bool AddStartupEntryW(const wchar_t* keystr, int keyByte, const wchar_t* entryPath, int pathByte);
	LIB_API bool RemoveStartupEntryW(const wchar_t* keystr);
	LIB_API bool QueryStartupEntryW(const wchar_t* keystr, int keyByte);


#ifdef __cplusplus
}
#endif


#ifdef _UNICODE
  #define AddStartupEntry AddStartupEntryW
  #define RemoveStartupEntry RemoveStartupEntryW
  #define QueryStartupEntry QueryStartupEntryW
#else
  #define AddStartupEntry AddStartupEntryA
  #define RemoveStartupEntry RemoveStartupEntryA
  #define QueryStartupEntry QueryStartupEntryA
#endif //_UNICODE




#endif //_STARTUP_H