
// PackageView.cpp : CPackageView ���ʵ��
//

#include "stdafx.h"
#include "Dwarves.h"

#include "PackageDoc.h"
#include "PackageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPackageView

IMPLEMENT_DYNCREATE(CPackageView, CView)

BEGIN_MESSAGE_MAP(CPackageView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPackageView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPackageView ����/����

CPackageView::CPackageView()
{
	// TODO: �ڴ˴���ӹ������

}

CPackageView::~CPackageView()
{
}

BOOL CPackageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPackageView ����

void CPackageView::OnDraw(CDC* /*pDC*/)
{
	CPackageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPackageView ��ӡ


void CPackageView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CPackageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPackageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPackageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPackageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPackageView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CPackageView ���

#ifdef _DEBUG
void CPackageView::AssertValid() const
{
	CView::AssertValid();
}

void CPackageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPackageDoc* CPackageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPackageDoc)));
	return (CPackageDoc*)m_pDocument;
}
#endif //_DEBUG


// CPackageView ��Ϣ�������
