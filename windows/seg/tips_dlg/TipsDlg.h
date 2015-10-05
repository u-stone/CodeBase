#pragma once


// CTipsDlg 对话框
#include "resource.h"

class CTipsDlg : public CDialog
{
	DECLARE_DYNAMIC(CTipsDlg)

public:
	CTipsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTipsDlg();

// 对话框数据
	enum { IDD = IDD_TipsDlg };

	BOOL OnInitDialog();

public:
	CString getTips();
	void setTips(CString strTips);
	void setTL(int x, int y);

private:
	CFont m_font;
	CString m_strTips;
	CDC* m_pDC;
	CFont* m_oldFont;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class CSingleTips {
public:
	static CTipsDlg* getTips(){
		OutputDebugString(_T("in getTips()"));
		if (s_TipsDlg == NULL){
			OutputDebugString(_T("in getTips() 1"));
			s_TipsDlg = new CTipsDlg();
			OutputDebugString(_T("in getTips() 2"));
			s_TipsDlg->Create(CTipsDlg::IDD);
			OutputDebugString(_T("in getTips() 3"));
		}
		OutputDebugString(_T("in getTips() 4"));
		return s_TipsDlg;
	}
private:
	static CTipsDlg* s_TipsDlg;
};