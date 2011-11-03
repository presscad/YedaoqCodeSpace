
// HookInstallerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HookInstaller.h"
#include "HookInstallerDlg.h"
#include "DebugLog.h"
//#include "..\CoCreateInstanceHook\CoCreateInstanceHook.h"
#include <Helper.h>

HHOOK*	WINAPI GetHookHandle();
HMODULE	WINAPI GetHookModule();
LRESULT CALLBACK  GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT	WINAPI   Clear();

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookInstallerDlg �Ի���

CHookInstallerDlg::CHookInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHookInstallerDlg::IDD, pParent)
	, m_HWndHook(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHookInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_HWndHook);
}

BEGIN_MESSAGE_MAP(CHookInstallerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHookInstallerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHookInstallerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CHookInstallerDlg ��Ϣ�������

BOOL CHookInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHookInstallerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHookInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHookInstallerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData();
	
	DWORD		idThread = GetWindowThreadProcessId((HWND)m_HWndHook, 0);
	HINSTANCE	hInst	 = GetHookModule();
	HHOOK		hHook	 = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, GetHookModule(), idThread);  

	*GetHookHandle() = hHook;

	if (*GetHookHandle()) 
	{
		//PostThreadMessage(idThread, WM_ENABLEAPIHOOK, 0, 0);
	} 
	else 
	{
		return;
	}
	//WTRACE("WM_ENABLEAPIHOOK");
	return;

}

void CHookInstallerDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
