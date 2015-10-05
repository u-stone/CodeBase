
// PressureCtrlDlg.h : ͷ�ļ�
//

#pragma once
#include "cpuPCtrl.h"
#include "ioPCtrl.h"

// CPressureCtrlDlg �Ի���
class CPressureCtrlDlg : public CDialogEx
{
// ����
public:
	CPressureCtrlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PRESSURECTRL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	cpuPCtrl* m_pCPUCtrl;
	ioPCtrl*  m_pIOCtrl;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
