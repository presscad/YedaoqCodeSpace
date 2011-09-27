
// TestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "..\YedaoqLayout\FlowLayout.h"

// CTestDlg �Ի���
class CTestDlg : public CDialog
{
// ����
public:
	CTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CmbTest;
	CButton m_BtnOK;
	CButton m_BtnCancel;
	CListBox m_LstTest;
	CTreeCtrl m_TreTest;

	NSYedaoqLayout::CFlowLayout m_Layout;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
