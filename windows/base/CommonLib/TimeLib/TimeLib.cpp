// TimeLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEMTIME st, lt;
	FILETIME   ft;
	GetSystemTime(&st);
	GetLocalTime(&lt);
	TIME_ZONE_INFORMATION tzi;
	DYNAMIC_TIME_ZONE_INFORMATION dtzi;
	GetTimeZoneInformationForYear(2015, &dtzi, &tzi);
	return 0;
}

