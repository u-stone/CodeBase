// allocSpeed.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "hpTimer.h"
#include <Windows.h>
#include <iostream>
using namespace  std;

void fun_null(){
	
}
void func(){
	char buf[512];
}

const int count = 10240;
__int64 now_t = 0;
unsigned __int64 now_c = 0;

void print(const char* pTips){
	cout << pTips << " : \t" << now_c/count << " cpu cycle \t" << now_t/count << " ns;" << endl;
}

#define CALC_TIME now_c = t.now_cpu_cycle(); now_t = t.now_ns();

int _tmain(int argc, _TCHAR* argv[])
{
	//malloc        free
	//new           delete 
	//heapalloc     heapfree
	//virtualalloc  virtualfree
	//ջ��           ��ջ�ͷ�
	//LocalAlloc    LoacalFree

	char* pBuffer[count] = {0};
	int sizes[count] = {0};
	for (int i = 0; i < count; ++i){
		sizes[i] = i;
	}


	hpTimer t;
	//let's begin...
	t.reset();
	for (int i = 0; i < count; ++i){
		pBuffer[i] = (char*)malloc(sizes[i]);
	}
	for (int i = 0; i < count; ++i){
		free(pBuffer[i]);
	}
	CALC_TIME
	print("malloc - free pair ");
	
	t.reset();
	//
	for (int i = 0; i < count; ++i){
		pBuffer[i] = new char[sizes[i]];
	}
	for (int i = 0; i < count; ++i){
		delete [] pBuffer[i];
	}
	CALC_TIME
	print("new - delete pair ");


	HANDLE hHeap = HeapCreate(0, 0, 0);
	for (int i = 0; i < count; ++i){
		pBuffer[i] = (char*)HeapAlloc(hHeap, 0, sizes[i]);
	}
	for (int i = 0; i < count; ++i){
		HeapFree(hHeap, 0, pBuffer[i]);
	}
	CALC_TIME
	print("HeapAlloc - HeapFree pair ");
	HeapDestroy(hHeap);

	t.reset();
	for (int i = 0; i < count; ++i){
		pBuffer[i] = (char*)VirtualAlloc(0, sizes[i], MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}
	for (int i = 0; i < count; ++i){
		VirtualFree(pBuffer[i], sizes[i], MEM_RELEASE);
	}
	CALC_TIME
	print("VirtualAlloc - VirtualFree pair ");
	
	t.reset();
	for (int i = 0; i < count; ++i)
		fun_null();
	CALC_TIME
	print("fun_null");

	t.reset();
	for (int i = 0; i < count; ++i)
		func();
	CALC_TIME
	print("func");

	return 0;
}

/*
Debug:
malloc - free pair               :   12704 cpu cycle          5676 ns;
new - delete pair                :   10767 cpu cycle          4813 ns;
HeapAlloc - HeapFree pair        :   4686 cpu cycle           2295 ns;
VirtualAlloc - VirtualFree pair  :   1995 cpu cycle           886 ns;
fun_null                         :   65 cpu cycle             29 ns;
func                             :   126 cpu cycle            56 ns;
�ӽ��������
0. ǰ�������ݴ�ŵ��ٶȱ����ǣ�  5 : 5 : 2 : 1
1. VirutalAlloc - VirtualFree�������ģ�
2. HeapAlloc - HeapFree���֮��
3. malloc - free���new-delete������޶������������Ǻ��߸���һЩ��

������������ջ�ϵ����ݣ�func�����ݼ�ȥfun_null������֮���� ����ٶ���졣
//////////////////////////////////////////////////////////////////////////
Release:
malloc - free pair               :   4584 cpu cycle  2037 ns;
new - delete pair                :   4411 cpu cycle  2003 ns;
HeapAlloc - HeapFree pair        :   4791 cpu cycle  2726 ns;
VirtualAlloc - VirtualFree pair  :   1986 cpu cycle  893 ns;
fun_null                         :   0 cpu cycle     0 ns;
func                             :   0 cpu cycle     0 ns;
���Կ�����������������У�
0. �ٶȱ��������ǣ� 2 : 2 : 2 : 1
1. ǰ�������������ˣ�����mallo-free new-delete���ٶ�����������HeapAlloc-HeapFree���ٶȷ��������ˡ�
2. VirualAlloc-VirtualFree�����ݼ���ûʲô�仯
3. Release�汾�Ż����˲��õ��ڴ�����

���Ľ����ǣ�VirtualAlloc-VirtualFree��ʤ��
*/
