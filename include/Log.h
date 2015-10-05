#ifndef _LOG_H_
#define _LOG_H_



#include <Windows.h>

#define CONTENT_SIZE  ( 4 << 23 )
#define MEMFILE_SIZE  ( CONTENT_SIZE + 8)
#define MEMUNIT_SIZE    1024//(  1 << 10 )
#define TOTAL_SIZE      32768//(  4 << 13 )


class Log
{
public:
	Log(){
		m_lc = Normal;
	}
	~Log(){
		
	}

	enum LogClass{
		Detail = 0,
		Normal,
		FuncCall,
		Frame
	};



public:
	void setClass(LogClass lc){
		m_lc = lc;
	}	


protected:
	LogClass       m_lc;

};

static char s_strlc[][10] = {
	"Detail",
	"Normal",
	"FuncCall",
	"Frame"
};

static wchar_t s_strlcw[][10] = {
	L"Detail",
	L"Normal",
	L"FuncCall",
	L"Frame"
};



#endif