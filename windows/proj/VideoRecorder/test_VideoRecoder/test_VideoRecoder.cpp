// test_VideoRecoder.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#define WIDTH  1600
#define HEIGHT 900
#include "capture.h"
#include "videoRecord.h"

int _tmain(int argc, _TCHAR* argv[])
{
	void* pRes = init();//��ʼ��¼��dll
	void* pBuffer = 0;
	int len = 0;
	void * pFactory = CreateVideoRecord("E:\\test.mp4", WIDTH, HEIGHT, 10, 0);
	for (int i = 0; i < 160; ++i){//����û��ʹ��ʱ�䣬�����޶�������ʵ��¼�Ƶ�ʱ�䲢���ϸ����������¼�ƣ�
		getScreen(pRes, &pBuffer, &len, WIDTH);
		IncomingVideo(pFactory, pBuffer, len);
	}
	ReleaseVideoRecord(pFactory);//
	uninit(pRes);//�ͷ�¼����Դ
	
	return 0;
}

