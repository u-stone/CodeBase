#include "StdAfx.h"
#include "ioPCtrl.h"
#include <stdio.h>

ioPCtrl::ioPCtrl(void)
{
	m_bReadStart = false;
	m_bWriteStart = false;
	m_bWriteQuit = m_bReadQuit = false;
	m_WriteThroughout = 0;
	m_ReadThroughout = 0;
	m_hMainThrd = 0;
	m_pReadBuffer = 0;
	m_pWriteBuffer = 0;
	memset(m_ReadFileName, 0, sizeof(m_ReadFileName));
	memset(m_WriteFileName, 0, sizeof(m_WriteFileName));
}

ioPCtrl::~ioPCtrl(void)
{
	setReadQuit(true);
	setWriteQuit(true);
	WaitForSingleObject(m_hMainThrd, INFINITE);
	if (m_pReadBuffer)
		GlobalFree(m_pReadBuffer);
	if (m_pWriteBuffer)
		GlobalFree(m_pWriteBuffer);
	DeleteFile(m_WriteFileName);
	DeleteFile(m_ReadFileName);
}

unsigned int __stdcall writeworker(void* p){
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;

	ioPCtrl* pCtrl = (ioPCtrl*)p;

	DWORD bytewritten = 0;
	DeleteFile(pCtrl->getWriteFileName());

	HANDLE hFile = CreateFile(pCtrl->getWriteFileName(), GENERIC_WRITE | GENERIC_READ, 
		0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return 1;
	SetFilePointer(hFile, FILE_WRITE_SIZE, 0, FILE_BEGIN);
	SetEndOfFile(hFile);
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);


	char* pBuffer = 0;
	unsigned int size = 0;
	if (!pCtrl->getWriteBuffer(&pBuffer, size))
		return 1;

	BOOL bRet = TRUE;
	int bytecount = 0;
	int filepos = 0;
	int duriation = 0;
	DWORD ticktalk = 0;
	while (1){
		if (pCtrl->getWriteQuit())
			break;
		if (!pCtrl->getWriteStart()){
			Sleep(1000);
			continue;
		}
		bytecount = 0;
		ticktalk = GetTickCount();
		while (bytecount < pCtrl->getWriteThroughout()){
			bRet = WriteFile(hFile, pBuffer, size, &bytewritten, 0);
			if (!bRet || bytewritten == 0)
				break;
			bytecount += bytewritten;
			filepos += bytewritten;
			//FlushFileBuffers(hFile);
			if (filepos >= FILE_WRITE_SIZE){
				SetFilePointer(hFile, 0, 0, FILE_BEGIN);
				filepos = 0;
			}
		}
		if ((duriation = GetTickCount() - ticktalk) < 1000)
			Sleep(1000 - duriation);
	}
	CloseHandle(hFile);
	return 0;
}

unsigned int __stdcall readworker(void* p){
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	TCHAR buf[30] = {0};

	ioPCtrl* pCtrl = (ioPCtrl*)p;

	DeleteFile(pCtrl->getReadFileName());

	DWORD bytewritten = 0;
	char* pBuffer = 0;
	unsigned int size = 0;
	if (!pCtrl->getReadBuffer(&pBuffer, size))
		return 1;
	memset(pBuffer, 0, size);

	HANDLE hFile = CreateFile(pCtrl->getReadFileName(), GENERIC_WRITE | GENERIC_READ, 0, &sa, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return 1;

	
	int bytecount = 0;
	while (bytecount < FILE_READ_SIZE){
		WriteFile(hFile, pBuffer, size, &bytewritten, 0);
		bytecount += bytewritten;
	}
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
	
	hFile = CreateFile(pCtrl->getReadFileName(), GENERIC_READ, 0, &sa, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return 1;

	DWORD byteread = 0;
	BOOL bRet = TRUE;

	int duriation = 0;
	DWORD ticktalk = 0;
	while (1){
		if (pCtrl->getReadQuit())
			break;
		if (!pCtrl->getReadStart()){
			Sleep(1000);
			continue;
		}
		bytecount = 0;
		ticktalk = GetTickCount();
		while (bytecount < pCtrl->getReadThroughout()){
			bRet = ReadFile(hFile, pBuffer, size, &byteread, 0);
			if (!bRet)
				break;
			bytecount += byteread;
			if (byteread == 0)
				SetFilePointer(hFile, 0, 0, FILE_BEGIN);
			byteread = 0;
		}
		if ((duriation = GetTickCount() - ticktalk) < 1000)
			Sleep(1000 - duriation);
	}
	CloseHandle(hFile);
	return 0;
}

unsigned int __stdcall mainThrd(void* p){
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	unsigned int id = 0;
	HANDLE hThrds[2];
	hThrds[0] = (HANDLE)_beginthreadex(&sa, 0, writeworker, p, 0, &id);
	hThrds[1] = (HANDLE)_beginthreadex(&sa, 0, readworker, p, 0, &id);
	WaitForMultipleObjects(2, hThrds, TRUE, INFINITE);
	return 0;
}

bool ioPCtrl::startRead()
{
	m_bReadStart = true;
	return start();
}

bool ioPCtrl::startWrite()
{
	m_bWriteStart = true;
	return start();
}

bool ioPCtrl::start()
{
	if (m_hMainThrd != 0){
		return false;
	}

	if (!init())
		return false;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = 0;
	unsigned int id = 0;

	m_hMainThrd = (HANDLE)_beginthreadex(&sa, 0, mainThrd, this, 0, &id);
	if (m_hMainThrd == 0)
		return false;

	return true;
}

bool ioPCtrl::init()
{
	if (m_pWriteBuffer == 0)
		m_pWriteBuffer = (char*)GlobalAlloc(GMEM_FIXED, BUFFER_WRITE_SIZE);
	if (m_pReadBuffer == 0)
		m_pReadBuffer = (char*)GlobalAlloc(GMEM_FIXED, BUFFER_READ_SIZE);
	if (m_pReadBuffer == 0 || m_pWriteBuffer == 0)
		return false;
	swprintf_s(m_ReadFileName, L"readfile.tmp");
	swprintf_s(m_WriteFileName, L"writefile.tmp");
	return true;
}

bool ioPCtrl::getReadBuffer(char** ppBuffer, unsigned int& size)
{
	if (m_pReadBuffer == 0)
		return false;
	*ppBuffer = m_pReadBuffer;
	size = BUFFER_READ_SIZE;
	return true;
}

bool ioPCtrl::getWriteBuffer(char** ppBuffer, unsigned int& size)
{
	if (m_pWriteBuffer == 0)
		return false;
	*ppBuffer = m_pWriteBuffer;
	size = BUFFER_WRITE_SIZE;
	return true;
}