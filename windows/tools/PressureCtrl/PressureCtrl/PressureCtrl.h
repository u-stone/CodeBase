
// PressureCtrl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPressureCtrlApp:
// �йش����ʵ�֣������ PressureCtrl.cpp
//

class CPressureCtrlApp : public CWinApp
{
public:
	CPressureCtrlApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPressureCtrlApp theApp;