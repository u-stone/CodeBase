// Log_Wr.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LogWr.h"
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int index = 0;
	char buf[100] = {0};
	time_t t1 = time(0);
	while (time(0) - t1 < 10){
		sprintf_s(buf, 100, "Hello World - %d", ++index);
		LogWr::getSingleton()->trace(buf);
		//Sleep(10);
	}
	LogWr::release();
	return 0;
}

