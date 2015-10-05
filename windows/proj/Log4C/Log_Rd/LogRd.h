#pragma once
#include <Windows.h>
#include "Log.h"

class LogRd : public Log
{
public:
	LogRd(void);
	LogRd(const char* pid);
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

public:
	bool get(char* pLog);
	void setpid(char* pid){
		memcpy(m_strPID, pid, sizeof(pid));
	}

private:
	void init();
	void uninit();

private:
	static LogRd*  s_Singleton;
	HANDLE         m_hMemFile;
	HANDLE         m_hReadEvent;
	char          *m_pBuffer;
	long           m_iRd;
	long           m_iWr;
	char           m_buf[MEMUNIT_SIZE];
	char           m_strPID[10];

};

