// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>

int _tmain(int argc, _TCHAR* argv[])
{
	char    outbuf[180];                         // size of the szOut Buffer defined in SERVICE.C -> ServiceStart
	DWORD   bytesRead;
	BOOL    ret;
	LPTSTR   lpszPipeName = L"\\\\.\\pipe\\simple";
	LPTSTR   lpszString = L"World";
	size_t      ndx;
	HRESULT hr;

	// allow user to define pipe name
	for ( ndx = 1; ndx < (size_t)argc; ndx++ )
	{
		if ( (*argv[ndx] == '-') || (*argv[ndx] == '/') )
		{
			if ( !(_tcsicmp( L"pipe", argv[ndx]+1 )) && ( (ndx + 1) < (size_t)argc))
			{
				lpszPipeName = argv[++ndx];
			}
			else if ( !(_tcsicmp( L"string", argv[ndx]+1 )) && ( (ndx + 1) < (size_t)argc))
			{
				lpszString = argv[++ndx];
			}
			else
			{
				printf("usage: client [-pipe <pipename>] [-string <string>]\n");
				exit(1);
			}
		}
		else
		{
			printf("usage: client [-pipe <pipename>] [-string <string>]\n");
			exit(1);
		}

	}

	hr = StringCchLength(lpszString,100,&ndx);
	if (FAILED(hr))
	{
		printf("Buffer is too large!\n");
		return 0;
	}

	ret = CallNamedPipe(lpszPipeName,
		lpszString, ndx,
		outbuf,180 ,
		&bytesRead, NMPWAIT_WAIT_FOREVER);

	if (!ret)
	{
		printf("client: CallNamedPipe failed, GetLastError = %d\n", GetLastError());
		exit(1);
	}

	printf("client: received: %s\n", outbuf);
	return 0;
}

