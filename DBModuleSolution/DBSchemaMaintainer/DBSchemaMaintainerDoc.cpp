
// DBSchemaMaintainerDoc.cpp : CDBSchemaMaintainerDoc ���ʵ��
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBSchemaMaintainerDoc

IMPLEMENT_DYNCREATE(CDBSchemaMaintainerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBSchemaMaintainerDoc, CDocument)
END_MESSAGE_MAP()


// CDBSchemaMaintainerDoc ����/����

CDBSchemaMaintainerDoc::CDBSchemaMaintainerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDBSchemaMaintainerDoc::~CDBSchemaMaintainerDoc()
{
}

BOOL CDBSchemaMaintainerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDBSchemaMaintainerDoc ���л�

void CDBSchemaMaintainerDoc::Serialize(CArchive& ar)
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


// CDBSchemaMaintainerDoc ���

#ifdef _DEBUG
void CDBSchemaMaintainerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBSchemaMaintainerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDBSchemaMaintainerDoc ����
