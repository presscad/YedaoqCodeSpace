
// DwarfDoc.cpp : CDwarfDoc ���ʵ��
//

#include "stdafx.h"
#include "Dwarves.h"

#include "DwarfDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDwarfDoc

IMPLEMENT_DYNCREATE(CDwarfDoc, CDocument)

BEGIN_MESSAGE_MAP(CDwarfDoc, CDocument)
END_MESSAGE_MAP()


// CDwarfDoc ����/����

CDwarfDoc::CDwarfDoc()
	: DBModule(0)
{
	// TODO: �ڴ����һ���Թ������

}

CDwarfDoc::~CDwarfDoc()
{
}

BOOL CDwarfDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CDwarfDoc ���л�

void CDwarfDoc::Serialize(CArchive& ar)
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


// CDwarfDoc ���

#ifdef _DEBUG
void CDwarfDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDwarfDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDwarfDoc ����
