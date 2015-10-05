// StopWatch.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "hpTimer.h"

#define K 1000
#define M 1000000
#define G 1000000000

//ʹ��hpTimer���Ը��ֲ����ĺ�ʱ

int _tmain(int argc, _TCHAR* argv[])
{
	const __int64 optor = 5 * M;
	ULONG64 l = 0;

	HANDLE hEvent = CreateEvent(0, TRUE, TRUE, 0);
	if (hEvent == 0)
		return 1;
 	hpTimer t;
	t.reset();

 	for (int i = 0; i <optor; ++i){
		WaitForSingleObject(hEvent, 1000);
 	}

	unsigned __int64 dur = t.now_cpu_cycle();
	__int64  dur_t = t.now_ns();

	std::cout << "CPU Cycle cost: " << dur/optor << std::endl;
	std::cout << "time cost: " << dur_t/optor << " ns" << std::endl;
	

	return 0;
}


/*

for (int i = 0; i <optor; ++i) ; 
ÿһ�ο�ѭ������7��CPU���ڣ�3ns

l = i;
ÿ�θ�ֵ����ʱ���ԼΪ9��CPU����,3ns

WaitForSingleObject(hEvent, 1000);
������¼������ڼ���Ļ���ÿһ�θú����ĵ��ô�Լ����1124��CPU����,471ns




*/