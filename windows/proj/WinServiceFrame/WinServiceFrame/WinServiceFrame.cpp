// WinServiceFrame.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ServiceImpl.h"


int _tmain(int argc, TCHAR* argv[])
{
	if (argc > 1 && (*argv[1] == '-' || *argv[1] == '/')){
		if (_tcsicmp(L"install", argv[1] + 1) == 0){
			installService();
		} else if (_tcsicmp(L"remove", argv[1] + 1) == 0) {
			removeService();
		} else if (_tcsicmp(L"debug", argv[1] + 1) == 0){
			debugService(argc, argv);
		} else {
			return 0;
		}
		startDispatch();
	}
	return 0;
}

