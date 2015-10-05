#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;

class HeapObj
{
public:
	HeapObj(){
	}
	~HeapObj(){
	}

public:
	void* operator new (const size_t sz){
		//cout << "in operator new()" << endl;
		if (s_hHeap == 0)
			s_hHeap = HeapCreate(0, 0, 0);
		if (s_hHeap == 0)
			return 0;
		void* p = HeapAlloc(s_hHeap, 0, sz);
		if (p) ++s_ulCount;
		return p;
	}
	void operator delete (void* p){
		//cout << "in operator delete" << endl;
		HeapFree(s_hHeap, 0, p);
		if (--s_ulCount == 0){
			HeapDestroy(s_hHeap);
			s_hHeap = 0;
		}
	}
private:
	static HANDLE s_hHeap;
	static unsigned long s_ulCount;
	char   m_name[10];
	double m_height;
	int    m_iAge;
	int    m_iGender;
};

HANDLE HeapObj::s_hHeap = 0;
unsigned long HeapObj::s_ulCount = 0;


class NonHeapObj
{
public:
	NonHeapObj(){}
	~NonHeapObj(){}
private:
	char   m_name[10];
	double m_height;
	int    m_iAge;
	int    m_iGender;
};
