
// PressureCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PressureCtrl.h"
#include "PressureCtrlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPressureCtrlDlg 对话框




CPressureCtrlDlg::CPressureCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPressureCtrlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCPUCtrl = new cpuPCtrl();
	m_pIOCtrl = new ioPCtrl();
}

void CPressureCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPressureCtrlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnCPUStart, &CPressureCtrlDlg::OnBnClickedbtncpuctrl)
	ON_BN_CLICKED(IDC_btnReadStart, &CPressureCtrlDlg::OnBnClickedbtndiskread)
	ON_BN_CLICKED(IDC_btnWriteStart, &CPressureCtrlDlg::OnBnClickedbtnwritestart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_btnCPUStop, &CPressureCtrlDlg::OnBnClickedbtncpustop)
	ON_BN_CLICKED(IDC_btnReadStop, &CPressureCtrlDlg::OnBnClickedbtnreadstop)
	ON_BN_CLICKED(IDC_btnWriteStop, &CPressureCtrlDlg::OnBnClickedbtnwritestop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPressureCtrlDlg 消息处理程序

BOOL CPressureCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EdtCPUCtrl)->SetWindowText(_T("50"));
	GetDlgItem(IDC_EdtReadThroughout)->SetWindowText(_T("20"));
	GetDlgItem(IDC_EdtWriteThroughout)->SetWindowText(_T("20"));

	SetTimer(1024, 1000, 0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPressureCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPressureCtrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPressureCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPressureCtrlDlg::OnBnClickedbtncpuctrl()
{
	CString str;
	GetDlgItem(IDC_EdtCPUCtrl)->GetWindowText(str);
	if (str.IsEmpty()){
		MessageBox(_T("CPU压力值不能为空"));
		return ;
	}
	int value = _wtoi(str);
	if (value < 0 || value > 100){
		MessageBox(_T("CPU压力值应该位于[0-100]范围内！"));
		return ;
	}
	m_pCPUCtrl->setPressure(value);
	m_pCPUCtrl->start();
}


void CPressureCtrlDlg::OnBnClickedbtndiskread()
{
	CString str;
	GetDlgItem(IDC_EdtReadThroughout)->GetWindowText(str);
	if (str.IsEmpty()){
		MessageBox(_T("磁盘读取吞吐量不能为空"));
		return ;
	}
	int value = _wtoi(str);
	if (value < 0 || value > 1024){
		MessageBox(_T("磁盘读取吞吐量应该位于[0-1024]范围内！"));
		return ;
	}
	m_pIOCtrl->setReadThroughout(value);
	m_pIOCtrl->startRead();
}


void CPressureCtrlDlg::OnBnClickedbtnwritestart()
{
	CString str;
	GetDlgItem(IDC_EdtWriteThroughout)->GetWindowText(str);
	if (str.IsEmpty()){
		MessageBox(_T("磁盘写入吞吐量不能为空"));
		return ;
	}
	int value = _wtoi(str);
	if (value < 0 || value > 1024){
		MessageBox(_T("磁盘写入吞吐量应该位于[0-1024]范围内！"));
		return ;
	}
	m_pIOCtrl->setWriteThroughout(value);
	m_pIOCtrl->startWrite();
}


void CPressureCtrlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete m_pIOCtrl;
	delete m_pCPUCtrl;
}


void CPressureCtrlDlg::OnBnClickedbtncpustop()
{
	m_pCPUCtrl->setStart(false);
}

void CPressureCtrlDlg::OnBnClickedbtnreadstop()
{
	m_pIOCtrl->setReadStart(false);
}

void CPressureCtrlDlg::OnBnClickedbtnwritestop()
{
	m_pIOCtrl->setWriteStart(false);
}


void CPressureCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1024){
		if (m_pCPUCtrl->getStart()){
			GetDlgItem(IDC_btnCPUStart)->EnableWindow(FALSE);			
			GetDlgItem(IDC_btnCPUStop)->EnableWindow(TRUE);
		} else {
			GetDlgItem(IDC_btnCPUStart)->EnableWindow(TRUE);			
			GetDlgItem(IDC_btnCPUStop)->EnableWindow(FALSE);
		}
		if (m_pIOCtrl->getReadStart()){
			GetDlgItem(IDC_btnReadStop)->EnableWindow(TRUE);			
			GetDlgItem(IDC_btnReadStart)->EnableWindow(FALSE);
		} else {
			GetDlgItem(IDC_btnReadStop)->EnableWindow(FALSE);			
			GetDlgItem(IDC_btnReadStart)->EnableWindow(TRUE);
		}
		if (m_pIOCtrl->getWriteStart()){
			GetDlgItem(IDC_btnWriteStop)->EnableWindow(TRUE);			
			GetDlgItem(IDC_btnWriteStart)->EnableWindow(FALSE);
		} else {
			GetDlgItem(IDC_btnWriteStop)->EnableWindow(FALSE);			
			GetDlgItem(IDC_btnWriteStart)->EnableWindow(TRUE);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
