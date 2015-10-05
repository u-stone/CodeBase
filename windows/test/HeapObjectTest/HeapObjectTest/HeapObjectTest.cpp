// HeapObjectTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "heapobj.h"
#include "hpTimer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0x000000ff;
	int * pNonHeapPtrs = (int * )malloc(count * sizeof(int *));
	memset(pNonHeapPtrs, 0, count * sizeof(NonHeapObj *));
	int * pHeapPtrs = (int * )malloc(count * sizeof(int*));
	memset(pHeapPtrs, 0, count * sizeof(HeapObj *));
	NonHeapObj* pNonHeapObj = 0;
	HeapObj* pHeapObj = 0;
	int *pint = pNonHeapPtrs;

	hpTimer t;
	t.reset();
	for (int i = 0; i < count; ++i){
		*pint = (int)(int*)new NonHeapObj();
		pint++;
	}

	__int64 cpu_new_non = t.now_cpu_cycle();
	__int64 time_new_non = t.now_ns();
	cout << "new NonHeapObj with " << time_new_non << " ns" << endl;
	cout << "new NonHeapObj with " << cpu_new_non << " cpu cycle" << endl;


	pint = pHeapPtrs;
	t.reset();

	for (int i = 0; i < count; ++i){
		*pint = (int)(int*)new HeapObj();
		++pint;
	}

	__int64 cpu_new_heap = t.now_cpu_cycle();
	__int64 time_new_heap = t.now_ns();
	cout << "new HeapObj with " << time_new_heap << " ns" << endl;
	cout << "new HeapObj with " << cpu_new_heap << " cpu cycle" << endl;
 
 	pint = pNonHeapPtrs;
 	t.reset();
 	for (int i = 0; i < count; ++i){
 		delete (NonHeapObj*)(*pint++);
 	}
 
 	__int64 cpu_delete_non = t.now_cpu_cycle();
 	__int64 time_delete_non = t.now_ns();
 	cout << "delete NonHeapObj ary with " << time_delete_non << " ns" << endl;
 	cout << "delete NonHeapObj ary with " << cpu_delete_non << " cpu cycle" << endl;
 	
 	pint = pHeapPtrs;
 	t.reset();
 	for (int i = 0; i < count; ++i){
 		delete (HeapObj*)(*pint++);
 	}
 
 	__int64 cpu_delete_heap = t.now_cpu_cycle();
 	__int64 time_delete_heap = t.now_ns();
 	cout << "delete HeapObj ary with " << time_delete_heap << " ns" << endl;
 	cout << "delete HeapObj ary with " << cpu_delete_heap << " cpu cycle" << endl;
 
 	cout << "time non  new: " << time_new_non << endl;
 	cout << "time heap new: " << time_new_heap << endl;
 	cout << "time non  del: " << time_delete_non << endl;
 	cout << "time heap del: " << time_delete_heap << endl;
 	cout << "time non  new & del: " << time_new_non + time_delete_non << endl;
 	cout << "time heap new & del: " << time_new_heap + time_delete_heap << endl;
 
 	cout << "cpu non  new: " << cpu_new_non << endl;
 	cout << "cpu heap new: " << cpu_new_heap << endl;
 	cout << "cpu non  del: " << cpu_delete_non << endl;
 	cout << "cpu heap del: " << cpu_delete_non << endl;
 	cout << "cpu non  new & del: " << cpu_new_non + cpu_delete_non << endl;
 	cout << "cpu heap new & del: " << cpu_new_heap + cpu_delete_heap << endl;

	free(pNonHeapPtrs);
	free(pHeapPtrs);
	return 0;
}

/*

count == 255时的输出:

new NonHeapObj with 74274 ns
new NonHeapObj with 160572 cpu cycle
new HeapObj with 132697 ns
new HeapObj with 295433 cpu cycle
delete NonHeapObj ary with 83785 ns
delete NonHeapObj ary with 185120 cpu cycle
delete HeapObj ary with 70198 ns
delete HeapObj ary with 154617 cpu cycle
time non  new: 74274
time heap new: 132697
time non  del: 83785
time heap del: 70198
time non  new & del: 158059
time heap new & del: 202895
cpu non  new: 160572
cpu heap new: 295433
cpu non  del: 185120
cpu heap del: 185120
cpu non  new & del: 345692
cpu heap new & del: 450050

*/