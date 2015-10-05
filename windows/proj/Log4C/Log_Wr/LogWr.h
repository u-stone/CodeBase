#pragma once
#include "Log.h"

/*

  rd_index | wt_index | content-list
     4          4         4 << 20

  content-list:
  content-1 | content-2 | ...
   1 << 10     1 << 10
*/

class LogWr : public Log
{
public:
	LogWr(void);
	~LogWr(void);

public:
	static LogWr* getSingleton(){
		if (s_Singleton == 0){
			s_Singleton = new LogWr();
		}
		return s_Singleton;
	}
	static void release(){
		if (s_Singleton){
			s_Singleton->serialize();
			delete s_Singleton;
		}
	}

public:
	void trace(const char* strlog, LogClass lc = Normal);
	void trace(const wchar_t* strlog, LogClass lc = Normal);
	void serialize();

private:
	static LogWr  *s_Singleton;
	HANDLE         m_hMemFile;
	HANDLE         m_hReadEvent;
	char          *m_pBuffer;
	long           m_iRd;
	long           m_iWr;
	unsigned int   m_LogIndex;
	char           m_buf[MEMUNIT_SIZE];
	wchar_t        m_bufw[MEMUNIT_SIZE];
	SYSTEMTIME     m_st;

private:
	void init();
	void uninit();
};

