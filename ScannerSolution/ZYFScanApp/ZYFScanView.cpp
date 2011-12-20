
// ZYFScanView.cpp : CZYFScanView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ZYFScanApp.h"
#endif

#include "ZYFScanDoc.h"
#include "ZYFScanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZYFScanView

IMPLEMENT_DYNCREATE(CZYFScanView, CView)

BEGIN_MESSAGE_MAP(CZYFScanView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CZYFScanView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CZYFScanView ����/����

CZYFScanView::CZYFScanView()
{
	// TODO: �ڴ˴���ӹ������

}

CZYFScanView::~CZYFScanView()
{
}

BOOL CZYFScanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CZYFScanView ����

void CZYFScanView::OnDraw(CDC* /*pDC*/)
{
	CZYFScanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CZYFScanView ��ӡ


void CZYFScanView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CZYFScanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CZYFScanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CZYFScanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CZYFScanView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CZYFScanView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CZYFScanView ���

#ifdef _DEBUG
void CZYFScanView::AssertValid() const
{
	CView::AssertValid();
}

void CZYFScanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZYFScanDoc* CZYFScanView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZYFScanDoc)));
	return (CZYFScanDoc*)m_pDocument;
}
#endif //_DEBUG


// CZYFScanView ��Ϣ�������
