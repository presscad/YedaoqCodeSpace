// MFC.h : MFC DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCApp
// �йش���ʵ�ֵ���Ϣ������� MFC.cpp
//

class CMFCApp : public CWinApp
{
public:
	CMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
