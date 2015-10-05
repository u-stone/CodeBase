// StopWatch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "hpTimer.h"

#define K 1000
#define M 1000000
#define G 1000000000

//使用hpTimer测试各种操作的耗时

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
每一次空循环花费7个CPU周期，3ns

l = i;
每次赋值花费时间大约为9个CPU周期,3ns

WaitForSingleObject(hEvent, 1000);
如果是事件本身处于激活的话，每一次该函数的调用大约花费1124个CPU周期,471ns




*/