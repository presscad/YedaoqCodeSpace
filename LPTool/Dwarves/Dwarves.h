
// Dwarves.h : Dwarves Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "PackageDoc.h"


// CDwarvesApp:
// �йش����ʵ�֣������ Dwarves.cpp
//

class CDwarvesApp : public CWinAppEx
{
public:
	CDwarvesApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CPackageDoc	m_Doc;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	
	afx_msg void OnFileOpen();
	afx_msg void OnViewOpen(UINT id);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDwarvesApp theApp;
