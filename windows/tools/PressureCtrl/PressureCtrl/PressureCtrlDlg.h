
// PressureCtrlDlg.h : 头文件
//

#pragma once
#include "cpuPCtrl.h"
#include "ioPCtrl.h"

// CPressureCtrlDlg 对话框
class CPressureCtrlDlg : public CDialogEx
{
// 构造
public:
	CPressureCtrlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PRESSURECTRL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	cpuPCtrl* m_pCPUCtrl;
	ioPCtrl*  m_pIOCtrl;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtncpuctrl();
	afx_msg void OnBnClickedbtndiskread();
	afx_msg void OnBnClickedbtnwritestart();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedbtncpustop();
	afx_msg void OnBnClickedbtnreadstop();
	afx_msg void OnBnClickedbtnwritestop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
