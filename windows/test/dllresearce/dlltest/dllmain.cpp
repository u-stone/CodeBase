// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <stdio.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		printf("\n-----------------------------DLL_PROCESS_ATTACH\n");
		break;
	case DLL_THREAD_ATTACH:
		printf("\n-----------------------------DLL_THREAD_ATTACH\n");
		break;
	case DLL_THREAD_DETACH:
		printf("\n-----------------------------DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
		printf("\n-----------------------------DLL_PROCESS_DETACH\n");
		break;
	}
	return TRUE;
}

