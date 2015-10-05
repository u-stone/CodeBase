#pragma once
#include <Windows.h>

#define MEMFILE_SIZE (4 << 10)

class LogRd
{
public:
	LogRd(void);
	~LogRd(void);

public:
	static LogRd* getSingleton(){
		if (s_Singleton == 0){
			s_Singleton = new LogRd();
		}
		return s_Singleton;
	}
	static void release(){
		if (s_Singleton)
			delete s_Singleton;
	}
	static LogRd* s_Singleton;

	void get(void* pLog);

private:
	HANDLE       m_hMemFile;
	HANDLE       m_hReadEvent;
	void        *m_pBuffer;

	void init();
	void uninit();
};

