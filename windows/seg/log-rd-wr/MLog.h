#pragma once
#include <Windows.h>

#define MEMFILE_SIZE (4 << 10)

class MLog
{
public:
	MLog(void);
	~MLog(void);

	enum LogClass{
		Detail = 0,
		Normal,
		FuncCall,
		Frame
	};
public:
	static MLog* getSingleton(){
		if (s_Singleton == 0){
			s_Singleton = new MLog();
		}
		return s_Singleton;
	}
	static void release(){
		if (s_Singleton)
			delete s_Singleton;
	}
	void setClass(LogClass lc){
		m_lc = lc;
	}
	void trace(const char* strlog, LogClass lc = Normal);
private:
	static MLog* s_Singleton;
	HANDLE       m_hMemFile;
	HANDLE       m_hReadEvent;
	void        *m_pBuffer;
	LogClass     m_lc;
private:
	void init();
	void uninit();
};

