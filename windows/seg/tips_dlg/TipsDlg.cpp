// TipsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LiveMeeting.h"
#include "TipsDlg.h"
#include ".\tipsdlg.h"


CTipsDlg* CSingleTips::s_TipsDlg = NULL;

// CTipsDlg 对话框

IMPLEMENT_DYNAMIC(CTipsDlg, CDialog)
CTipsDlg::CTipsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTipsDlg::IDD, pParent)
{

}

CTipsDlg::~CTipsDlg()
{
	m_pDC->SelectObject(m_oldFont);
	ReleaseDC(m_pDC);
	delete m_pDC;
	m_font.DeleteObject();
}

void CTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTipsDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTipsDlg 消息处理程序

CString CTipsDlg::getTips()
{
	return m_strTips;
}

void CTipsDlg::setTips(CString strTips)
{
	m_strTips = strTips;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_Tips);
	pWnd->SetFont(&m_font, TRUE);
	pWnd->SetWindowText(m_strTips);
	Invalidate(TRUE);
}

void CTipsDlg::setTL(int x, int y)
{
	CSize sz = m_pDC->GetTextExtent(m_strTips);
	sz.cx += 6; sz.cy += 6;
	::MoveWindow(GetSafeHwnd(), x, y, sz.cx, sz.cy, TRUE);
}

BOOL CTipsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pDC = new CClientDC(this);
	m_font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, _T("宋体"));
	m_oldFont = m_pDC->SelectObject(&m_font);
	return TRUE;
}
HBRUSH CTipsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_STATIC){
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,225));
	}
	return hbr;
}

BOOL CTipsDlg::OnEraseBkgnd(CDC* pDC)
{
  	CRect rc; 
  	GetClientRect(rc);
	pDC->FillSolidRect(&rc , RGB(255,255,225) );
	pDC->SelectStockObject(BLACK_PEN);
	pDC->Rectangle(&rc);
	return TRUE;
}

void CTipsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (cx *cy > 0){
		::MoveWindow(GetDlgItem(IDC_STATIC_Tips)->GetSafeHwnd(), 3, 3, cx-6, cy-6, TRUE);
	}
}
