
// DBSchemaTableView.cpp : CDBSchemaTableView ���ʵ��
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"
#include "DBSchemaTableView.h"
#include "DBSchemaInfoRecord.h"
#include "Helper.h"

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
	ON_COMMAND(EIDC_BTNMERGE, &CDBSchemaTableView::OnBtnMergeClicked)
	ON_NOTIFY(GVN_SELCHANGED, EIDC_GRIDTBL, &CDBSchemaTableView::OnGridTblSelChanged)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDBSchemaTableView ����/����

CDBSchemaTableView::CDBSchemaTableView()
	: Layouter(EnumLayoutDirection::Vertical), GridColViewer(&GridCol, 1), GridTabViewer(&GridTab, 1)
{
	// TODO: �ڴ˴����ӹ������

	Grid_Select = CDBColumnViewInfo(
		TEXT(""), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		20,
		false);

	GridTab_Name = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		180,
		true);

	GridTab_DBName = CDBColumnViewInfo(
		TEXT("���ݿ���"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		180,
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

	GridTab_State = CDBColumnViewInfo(
		TEXT(""), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		20,
		true);

	GridTblColumns.AppendVirtual(&Grid_Select);
	GridTblColumns.Append(&GridTab_State);
	GridTblColumns.Append(&GridTab_Name);
	GridTblColumns.Append(&GridTab_DBName);
	GridTblColumns.Append(&GridTab_BuildIn);
	GridTblColumns.Append(&GridTab_DBExist);	

	GridCol_State = CDBColumnViewInfo(
		TEXT(""), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		20,
		true);

	GridCol_Name = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		180,
		true);

	GridCol_DBName = CDBColumnViewInfo(
		TEXT("���ݿ���"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		180,
		true);

	GridCol_Buildin = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridCol_DBExist = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridCol_CppType = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		80,
		false);

	GridCol_DBType = CDBColumnViewInfo(
		TEXT("���ݿ�����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		80,
		true);

	GridCol_KeyCol = CDBColumnViewInfo(
		TEXT("�ؼ���"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		false);

	GridCol_DBPK = CDBColumnViewInfo(
		TEXT("����"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridCol_DBNull = CDBColumnViewInfo(
		TEXT("�ɿ�"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridColColumns.AppendVirtual(&Grid_Select);
	GridColColumns.Append(&GridCol_State);
	GridColColumns.Append(&GridCol_Name);
	GridColColumns.Append(&GridCol_DBName);
	GridColColumns.Append(&GridCol_Buildin);
	GridColColumns.Append(&GridCol_DBExist);
	GridColColumns.Append(&GridCol_CppType);
	GridColColumns.Append(&GridCol_DBType);
	GridColColumns.Append(&GridCol_KeyCol);
	GridColColumns.Append(&GridCol_DBPK);
	GridColColumns.Append(&GridCol_DBNull);
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
	GridCol.Create(rect, this, EIDC_GRIDCOL, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	GridTab.Create(rect, this, EIDC_GRIDTBL, WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	CreateButton(BtnMerge, EIDC_BTNMERGE, this, TEXT("�ϲ�"));

	//RECT rect1 = { 0, 0, 150, 28 };
	//CmbTab.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, rect1, this, 3);
	//CmbTab.ShowWindow(SW_SHOW);

	CFlowLayout* pFlow = Layouter.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(BtnMerge.GetSafeHwnd());

	pFlow = Layouter.AddFlow(
		EnumLayoutDirection::Horizon,
		ResizeInfo::FillInfo);

	pFlow->AddCtrl(
		GridTab.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Fixed, 490),
		ResizeInfo::FillInfo);

	pFlow->AddCtrl(
		GridCol.GetSafeHwnd(), 
		ResizeInfo::FillInfo, 
		ResizeInfo::FillInfo);

	GridTab.SetColumnCount(6);
	GridTabViewer.Initialize(GridTblColumns);

	GridCol.SetColumnCount(11);
	GridColViewer.Initialize(GridColColumns);

	return S_OK;
}

// CDBSchemaTableView ����

void CDBSchemaTableView::OnDraw(CDC* /*pDC*/)
{
	CDBSchemaMaintainerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
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
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDBSchemaTableView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
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

	GridTabViewer.Clear();
	GridColViewer.Clear();
	GridTabViewer.Fill(pEnumTbl);
}


// CDBSchemaTableView ��Ϣ��������
void CDBSchemaTableView::OnGridTblSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CCellRange range = GridTab.GetSelectedCellRange();
	if(range.IsValid() && range.GetMinRow() >= GRIDHEADERROWCOUNT)
	{
		ShowColumnsOfTable(range.GetMinRow() - GRIDHEADERROWCOUNT);
	}
}

void CDBSchemaTableView::OnBtnMergeClicked()
{
	int iSelectedRowCount = 0;
	int iSelectedRows[2];

	for(int i = GRIDHEADERROWCOUNT; i < GridTab.GetRowCount(); ++i)
	{
		CGridCellBase* pCell = GridTab.GetCell(i, 0);
		if(CEditStyleBool::GetInstance().strTrue == pCell->GetValue())
		{
			iSelectedRows[iSelectedRowCount++] = i - GRIDHEADERROWCOUNT;
			if(iSelectedRowCount >= 2) break;
		}
	}

	if(iSelectedRowCount == 2)
	{
		GetDocument()->MergeTable(iSelectedRows[0], iSelectedRows[1]);
	}

	iSelectedRowCount = 0;
	for(int i = GRIDHEADERROWCOUNT; i < GridCol.GetRowCount(); ++i)
	{
		CGridCellBase* pCell = GridCol.GetCell(i, 0);
		TTRACE(pCell->GetValue());
		if(CEditStyleBool::GetInstance().strTrue == pCell->GetValue())
		{
			iSelectedRows[iSelectedRowCount++] = i - GRIDHEADERROWCOUNT;
			if(iSelectedRowCount >= 2) break;
		}
	}

	if(iSelectedRowCount == 2)
	{
		GetDocument()->MergeColumn(GridTabViewer.GetCurRecord(0), iSelectedRows[0], iSelectedRows[1]);
	}
}

int CDBSchemaTableView::ShowColumnsOfTable(int idxTbl)
{
	GridColViewer.Clear();
	GridColViewer.Fill(CDBColumnInfoEnumerator(&GetDocument()->GetDBModule().Tables()[idxTbl]->GetSchema()));
	return 1;
}

void CDBSchemaTableView::CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle, UINT width, UINT height, DWORD dwStyle, CFont* pFont)
{
	static CFont* pDefaultFont = 0;

	if(!pDefaultFont)
	{
		pDefaultFont = new CFont;
		pDefaultFont->CreateFont(16,		//   nHeight
			0,								//   nWidth
			0,								//   nEscapement
			0,								//   nOrientation
			0,								//   nWeight
			FALSE,							//   bItalic
			FALSE,							//   bUnderline
			0,								//   cStrikeOut
			ANSI_CHARSET,					//   nCharSet
			OUT_DEFAULT_PRECIS,				//   nOutPrecision
			CLIP_DEFAULT_PRECIS,			//   nClipPrecision
			DEFAULT_QUALITY,				//   nQuality
			DEFAULT_PITCH   |   FF_SWISS,	//   nPitchAndFamily
			_T( "Arial "));					//   lpszFac 
	}

	RECT rect = {0, 0, width, height};
	btn.Create(lpTitle, dwStyle, rect, pParent, id);
	if(!pFont) pFont = pDefaultFont;
	btn.SetFont(pFont);
	btn.ShowWindow(SW_SHOW);
}