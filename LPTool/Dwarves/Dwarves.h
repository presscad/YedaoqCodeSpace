
// Dwarves.h : Dwarves Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


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

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	
	afx_msg void OnFileOpen();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDwarvesApp theApp;
