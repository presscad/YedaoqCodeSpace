
// PackageDoc.cpp : CPackageDoc ���ʵ��
//

#include "stdafx.h"
#include "Dwarves.h"

#include "PackageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPackageDoc

IMPLEMENT_DYNCREATE(CPackageDoc, CDocument)

BEGIN_MESSAGE_MAP(CPackageDoc, CDocument)
END_MESSAGE_MAP()


// CPackageDoc ����/����

CPackageDoc::CPackageDoc()
	: DBModule(0)
{
	// TODO: �ڴ����һ���Թ������

}

CPackageDoc::~CPackageDoc()
{
}

BOOL CPackageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CPackageDoc ���л�

void CPackageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CPackageDoc ���

#ifdef _DEBUG
void CPackageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPackageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPackageDoc ����
