
// DBSchemaTableView.cpp : CDBSchemaTableView ���ʵ��
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"
#include "DBSchemaTableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBSchemaTableView

IMPLEMENT_DYNCREATE(CDBSchemaTableView, CView)

BEGIN_MESSAGE_MAP(CDBSchemaTableView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDBSchemaTableView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDBSchemaTableView ����/����

CDBSchemaTableView::CDBSchemaTableView()
{
	// TODO: �ڴ˴���ӹ������

}

CDBSchemaTableView::~CDBSchemaTableView()
{
}

BOOL CDBSchemaTableView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDBSchemaTableView ����

void CDBSchemaTableView::OnDraw(CDC* /*pDC*/)
{
	CDBSchemaMaintainerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CDBSchemaTableView ��ӡ


void CDBSchemaTableView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDBSchemaTableView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDBSchemaTableView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDBSchemaTableView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CDBSchemaTableView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDBSchemaTableView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CDBSchemaTableView ���

#ifdef _DEBUG
void CDBSchemaTableView::AssertValid() const
{
	CView::AssertValid();
}

void CDBSchemaTableView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDBSchemaMaintainerDoc* CDBSchemaTableView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBSchemaMaintainerDoc)));
	return (CDBSchemaMaintainerDoc*)m_pDocument;
}
#endif //_DEBUG


// CDBSchemaTableView ��Ϣ�������
