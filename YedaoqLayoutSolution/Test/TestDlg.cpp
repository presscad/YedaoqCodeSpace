
// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "..\YedaoqLayout\ZoomBlankLayout.h"
#include "..\YedaoqLayout\CtrlLayout.h"
#include "Dialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestDlg �Ի���

using namespace NSYedaoqLayout;


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDlg::IDD, pParent), m_Layout(EnumLayoutDirection::Vertical)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CmbTest);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_LIST1, m_LstTest);
	DDX_Control(pDX, IDC_TREE1, m_TreTest);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestDlg ��Ϣ�������

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CFlowLayout* pFlowLayout = m_Layout.AddFlow(
		EnumLayoutDirection::Horizon);
	
	CCtrlLayout* pCtrlLayout = pFlowLayout->AddCtrl(m_CmbTest.GetSafeHwnd(),
		ResizeInfo::FixedInfo,
		ResizeInfo::FixedInfo,
		AnchorInfo::AnchorFront,
		AnchorInfo::AnchorCenter);

	pFlowLayout->AddZoomBlank();
	pCtrlLayout = pFlowLayout->AddCtrl(m_BtnOK.GetSafeHwnd());
	pCtrlLayout = pFlowLayout->AddCtrl(m_BtnCancel.GetSafeHwnd());

	pFlowLayout = m_Layout.AddFlow(
		EnumLayoutDirection::Horizon, 
		ResizeInfo(EnumResizeMode::Zoomed, 100));

	pCtrlLayout = pFlowLayout->AddCtrl(m_TreTest.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Zoomed, 30),
		ResizeInfo(EnumResizeMode::Zoomed, 100));

	pCtrlLayout = pFlowLayout->AddCtrl(m_LstTest.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Zoomed, 70),
		ResizeInfo(EnumResizeMode::Zoomed, 100));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestDlg::OnPaint()
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_CmbTest.GetSafeHwnd())
	{
		m_Layout.Layout(LayoutPoint(3, 3), LayoutSize(cx - 6, cy - 6));
		this->Invalidate(FALSE);
	}
}

void CTestDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog1 dlg;
	dlg.DoModal();
	//OnOK();
}
