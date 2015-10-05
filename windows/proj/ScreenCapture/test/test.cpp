// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "capture.h"
#include <Windows.h>

#pragma comment(lib, "../../../../lib/ScreenCapture.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	void* p = init();
	void *pData = 0;
	int len = 0, width = 0;
	for (int i = 0; i < 20; ++i){
		set(p, i*60, i*3, 1024, 768);
		getScreen(p, &pData, &len, width);
		Sleep(100);
	}
	uninit(p);
	return 0;
}

