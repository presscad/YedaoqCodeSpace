
// HookInstallerDlg.h : ͷ�ļ�
//

#pragma once


// CHookInstallerDlg �Ի���
class CHookInstallerDlg : public CDialog
{
// ����
public:
	CHookInstallerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOOKINSTALLER_DIALOG };

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
	DWORD m_HWndHook;
	afx_msg void OnBnClickedOk();
};
