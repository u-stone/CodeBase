
// PressureCtrlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PressureCtrl.h"
#include "PressureCtrlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPressureCtrlDlg �Ի���




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


// CPressureCtrlDlg ��Ϣ�������

BOOL CPressureCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EdtCPUCtrl)->SetWindowText(_T("50"));
	GetDlgItem(IDC_EdtReadThroughout)->SetWindowText(_T("20"));
	GetDlgItem(IDC_EdtWriteThroughout)->SetWindowText(_T("20"));

	SetTimer(1024, 1000, 0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPressureCtrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPressureCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPressureCtrlDlg::OnBnClickedbtncpuctrl()
{
	CString str;
	GetDlgItem(IDC_EdtCPUCtrl)->GetWindowText(str);
	if (str.IsEmpty()){
		MessageBox(_T("CPUѹ��ֵ����Ϊ��"));
		return ;
	}
	int value = _wtoi(str);
	if (value < 0 || value > 100){
		MessageBox(_T("CPUѹ��ֵӦ��λ��[0-100]��Χ�ڣ�"));
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
		MessageBox(_T("���̶�ȡ����������Ϊ��"));
		return ;
	}
	int value = _wtoi(str);
	if (value < 0 || value > 1024){
		MessageBox(_T("���̶�ȡ������Ӧ��λ��[0-1024]��Χ�ڣ�"));
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
		MessageBox(_T("����д������������Ϊ��"));
		return ;
	}
	int value = _wtoi(str);
	if (value < 0 || value > 1024){
		MessageBox(_T("����д��������Ӧ��λ��[0-1024]��Χ�ڣ�"));
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
