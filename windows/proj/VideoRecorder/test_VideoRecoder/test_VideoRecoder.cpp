// test_VideoRecoder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#define WIDTH  1600
#define HEIGHT 900
#include "capture.h"
#include "videoRecord.h"

int _tmain(int argc, _TCHAR* argv[])
{
	void* pRes = init();//初始化录屏dll
	void* pBuffer = 0;
	int len = 0;
	void * pFactory = CreateVideoRecord("E:\\test.mp4", WIDTH, HEIGHT, 10, 0);
	for (int i = 0; i < 160; ++i){//这里没有使用时间，而是限定次数（实际录制的时间并不严格按照这个数字录制）
		getScreen(pRes, &pBuffer, &len, WIDTH);
		IncomingVideo(pFactory, pBuffer, len);
	}
	ReleaseVideoRecord(pFactory);//
	uninit(pRes);//释放录屏资源
	
	return 0;
}

