
// DBSchemaTableView.cpp : CDBSchemaTableView ���ʵ��
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"
#include "DBSchemaTableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NSYedaoqLayout;

// CDBSchemaTableView

IMPLEMENT_DYNCREATE(CDBSchemaTableView, CView)

BEGIN_MESSAGE_MAP(CDBSchemaTableView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDBSchemaTableView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDBSchemaTableView ����/����

CDBSchemaTableView::CDBSchemaTableView()
	: Layouter(EnumLayoutDirection::Vertical), GridColViewer(&GridCol, 1), GridTabViewer(&GridTab, 1)
{
	// TODO: �ڴ˴���ӹ������
	GridTab_Name = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		80,
		true);

	GridTab_DBName = CDBColumnViewInfo(
		TEXT("���ݿ���"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		80,
		true);

	GridTab_BuildIn = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridTab_DBExist = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridTblColumns.Append(&GridTab_Name);
	GridTblColumns.Append(&GridTab_DBName);
	GridTblColumns.Append(&GridTab_BuildIn);
	GridTblColumns.Append(&GridTab_DBExist);
}

CDBSchemaTableView::~CDBSchemaTableView()
{
}

BOOL CDBSchemaTableView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	//cs.style ^= WS_BORDER;

	return CView::PreCreateWindow(cs);
}

int CDBSchemaTableView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	RECT rect = {0, 0, lpcs->cx, lpcs->cy};
	GridCol.Create(rect, this, 1, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	GridTab.Create(rect, this, 2, WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	RECT rect1 = { 0, 0, 150, 28 };
	CmbTab.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, rect1, this, 3);

	CmbTab.ShowWindow(SW_SHOW);

	CFlowLayout* pFlow = Layouter.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(CmbTab.GetSafeHwnd());

	pFlow = Layouter.AddFlow(
		EnumLayoutDirection::Horizon,
		ResizeInfo::FillInfo);

	pFlow->AddCtrl(
		GridTab.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Fixed, 300),
		ResizeInfo::FillInfo);

	pFlow->AddCtrl(
		GridCol.GetSafeHwnd(), 
		ResizeInfo::FillInfo, 
		ResizeInfo::FillInfo);

	GridTab.SetColumnCount(4);
	GridTabViewer.Initialize(GridTblColumns);

	GridCol.SetColumnCount(2);
	GridCol.SetRowCount(1);

	return S_OK;
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

afx_msg void CDBSchemaTableView::OnSize(UINT nType, int cx, int cy)
{
	Layouter.Layout(LayoutPoint(0, 0), LayoutSize(cx, cy));
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

void CDBSchemaTableView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CDBTableInfoEnumerator pEnumTbl(&(GetDocument()->GetDBModule()));
	GridTabViewer.Fill(pEnumTbl);
}


// CDBSchemaTableView ��Ϣ�������
