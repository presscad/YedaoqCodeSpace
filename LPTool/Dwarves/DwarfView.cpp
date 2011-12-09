
// DwarfView.cpp : CDwarfView ���ʵ��
//

#include "stdafx.h"
#include "Dwarves.h"
#include "DwarfDoc.h"
#include "DwarfView.h"
#include "CommandIDAlloter.h"
#include "ChildFrm.h"
#include <memory>
#include "DwarfViewInfo.h"
#include "DwarfViewProvider.h"
#include "SideWnd.h"
#include <Helper.h>
#include <GridCtrl.h>
#include <GridCellBase.h>
#include <EditStyle.h>
#include "ToolBarMaintianer.h"
#include <boost\algorithm\string\find.hpp>
//#include <xutility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NSYedaoqLayout;

bool   CDwarfView::BkColorInitFlag = false;

// CDwarfView

IMPLEMENT_DYNCREATE(CDwarfView, CView)

BEGIN_MESSAGE_MAP(CDwarfView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDwarfView::OnFilePrintPreview)
	ON_COMMAND(ToolBarCmbSearchID, &CDwarfView::OnToolBarCmbSearch)
	ON_CBN_EDITCHANGE(ToolBarCmbSearchID, &CDwarfView::OnToolBarCmbSearchTxtChanged)
	ON_CBN_EDITUPDATE(ToolBarCmbSearchID, &CDwarfView::OnToolBarCmbSearchTxtChanged)
	ON_CBN_SELCHANGE(ToolBarCmbSearchID, &CDwarfView::OnToolBarCmbSearchTxtChanged)
	ON_NOTIFY(GVN_SELCHANGED, EIDC_GRID, &CDwarfView::OnGridSelChanged)
	ON_NOTIFY(NM_DBLCLK, EIDC_GRID, &CDwarfView::OnGridSelDBClick)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, MinViewOpID, MaxViewOpID, &CDwarfView::OnViewOperation)
END_MESSAGE_MAP()

// CDwarfView ����/����

CDwarfView::CDwarfView()
	: FlowLayoutMain(EnumLayoutDirection::Vertical), GridViewer(&Grid, 1), GridEditViewer(&GridEdit, 0), ViewID(-1),
	SelectedRecords(&Grid, &GridViewer),
	RecordIdxFocused(-1), RecordIdxUpdated(-1)
{
	// TODO: �ڴ˴���ӹ������
	
}

CDwarfView::~CDwarfView()
{
}

BOOL CDwarfView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDwarfView ����

void CDwarfView::OnDraw(CDC* /*pDC*/)
{
	CDwarfDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

int CDwarfView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	if(!BkColorInitFlag)
	{
		BkColorInitFlag = true;
		::SetClassLong(this->GetSafeHwnd(), GCL_HBRBACKGROUND, (LONG)::GetSysColorBrush(COLOR_BTNFACE));
	}

	RECT rect = {0, 0, lpcs->cx, GridEdit.GetDefCellHeight() + 5 + 18};

	GridEdit.Create(rect, this, EIDC_GRIDEDIT, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	GridEdit.SetBkColor((COLORREF)::GetSysColor(COLOR_BTNFACE));
	GridEdit.GetDefaultCell(FALSE, FALSE)->SetBackClr((COLORREF)0xFFFFE0);

	Grid.Create(rect, this, EIDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	Grid.SetBkColor((COLORREF)::GetSysColor(COLOR_BTNFACE));
	Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr((COLORREF)0xAAFFE0);

	CreateButton(BtnSearch, EIDC_BTNSEARCH, this, TEXT("����"));

	CFlowLayout* pFlow = FlowLayoutMain.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(BtnSearch.GetSafeHwnd());

	FlowLayoutMain.AddCtrl(GridEdit.GetSafeHwnd(), ResizeInfo::FillInfo, ResizeInfo::FixedInfo);
	FlowLayoutMain.AddCtrl(Grid.GetSafeHwnd(), ResizeInfo::FillInfo, ResizeInfo::FillInfo);

	return 0;
}

void CDwarfView::OnSize(UINT nType, int cx, int cy)
{
	FlowLayoutMain.Layout(LayoutPoint(2,2), LayoutSize(cx - 4, cy - 4));
}

//void CDwarfView::OnInitialUpdate()
//{
//	__super::OnInitialUpdate();
//	ResetTitle();
//}

void CDwarfView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	__super::OnUpdate(pSender, lHint, pHint);
}

// CDwarfView ��ӡ


void CDwarfView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDwarfView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDwarfView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDwarfView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CDwarfView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDwarfView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

void CDwarfView::OnViewOperation(UINT id)
{
	int opID = id - MinViewOpID;
	IDwarfViewInfo* pViewInfo = CDwarfViewProvider::GetInstance()[GetViewID()];
	if(pViewInfo)
	{
		Context.SideViewID = CGlobalData::GetSideWnd()->GetCurrentSideViewID();
		pViewInfo->ExecuteOperation(opID, &Context);
	}
}

void CDwarfView::OnGridSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int idx = GridViewer.GetCurRecord(0); 
	if(idx != RecordIdxFocused)
	{
		RecordIdxFocused = idx;
		CGlobalData::GetSideWnd()->RefreshSideView();
	}
}

void CDwarfView::OnGridSelDBClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pNm = (NM_GRIDVIEW*)pNotifyStruct;

	if(pNm->iRow > 0 && pNm->iRow < Grid.GetRowCount())
	{
		RecordIdxUpdated = GridViewer.GetCurRecord(&RecordUpdated);
		GridEditViewer.SetRecordAt(0, RecordUpdated);

		GridEdit.Invalidate();
	}
}

void CDwarfView::OnToolBarCmbSearch()
{	
	bool	bOk = false;
	LPCTSTR strCell;

	// ��ȡ����������
	int idx = CToolBarMaintianer::GetInstance().m_Bar->CommandToIndex(ToolBarCmbSearchID);
	LPCTSTR strKey = static_cast<CMFCToolBarComboBoxButton*>(CToolBarMaintianer::GetInstance().m_Bar->GetButton(idx))->GetText();
	
	if(!strKey || !*strKey) return;				// ������Ϊ��ʱ������

	CCellID cellFocused = Grid.GetFocusCell();	// ��ȡ��ǰ���ڽ���״̬�ĵ�Ԫ��

	if(cellFocused.row >= 0) ++cellFocused.row;	// �ӽ��㵥Ԫ�����һ�п�ʼ����

	for (int i = max(cellFocused.row, GridViewer.HeadRowCount()); i < Grid.GetRowCount() && !bOk; ++i)
	{
		for (int j = 1; j < Grid.GetColumnCount(); ++j)
		{
			LPCTSTR strCell = Grid.GetCell(i,j)->GetText();
			if(boost::ifind_first(strCell, strKey))
			{
				Grid.SetFocusCell(i,j);
				Grid.ScrollToRow(i);
				Grid.Invalidate(TRUE);
				bOk = true;
				break;
			}
		}
	}
}

void CDwarfView::OnToolBarCmbSearchTxtChanged()
{	
	int idx = CToolBarMaintianer::GetInstance().m_Bar->CommandToIndex(ToolBarCmbSearchID);
	//static_cast<CMFCToolBarComboBoxButton*>(CToolBarMaintianer::GetInstance().m_Bar->GetButton(idx))->GetText();
	TTRACE(TEXT("����������%s\n"), static_cast<CMFCToolBarComboBoxButton*>(CToolBarMaintianer::GetInstance().m_Bar->GetButton(idx))->GetText());
	//TTRACE(TEXT("����������%s\n"), CToolBarMaintianer::GetInstance().m_ToolCmbSearch.GetText());
}


// CDwarfView ���

#ifdef _DEBUG
void CDwarfView::AssertValid() const
{
	CView::AssertValid();
}

void CDwarfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDwarfDoc* CDwarfView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDwarfDoc)));
	return (CDwarfDoc*)m_pDocument;
}
#endif //_DEBUG

void CDwarfView::SetViewID(int id)
{ 
	if(id < 0)
	{
		_ASSERT(false);
		return;
	}

	ViewID = id; 
	Context.MainViewID = id;

	ResetTitle();
	Initialize();
	ShowRecords();

	CChildFrame* pChildFrame = static_cast<CChildFrame*>(GetParentFrame());
	pChildFrame->OnMDIActivate(TRUE, pChildFrame, NULL);
}

void CDwarfView::ResetTitle()
{
	CChildFrame* pChildFrm = (CChildFrame*)GetParent();

	if(ViewID >= 0)
	{
		pChildFrm->SetWindowText(GetDocument()->GetDBModule()->Tables()[ViewID]->GetName().c_str());
	}
	else
	{
		pChildFrm->SetWindowText(TEXT("δ֪"));
	}
}

int CDwarfView::Initialize()
{	
	IDwarfViewInfo* pView = CDwarfViewProvider::GetInstance()[ViewID];

	if(!pView)
	{
		_ASSERT(false);
		return 0;
	}

	pView->Initialize();

	CFlowLayout* pFlowOps = static_cast<CFlowLayout*>(FlowLayoutMain[0]);
	pFlowOps->Clear();
	pFlowOps->AddCtrl(BtnSearch.GetSafeHwnd());

	for(OpBtnCollection::iterator iter = BtnOps.begin(); iter != BtnOps.end(); ++iter)
	{
		delete *iter;
	}
	BtnOps.clear();

	int DlgItemID = EIDC_UNUSED;
	std::auto_ptr<IEnumerator<DwarfViewOperationItem>> pEnumOp(pView->EnumOperation());
	if(pEnumOp.get())
	{	
		while(pEnumOp->MoveNext())
		{
			BtnOps.push_back(new CButton());
			CreateButton(**BtnOps.rbegin(), MinViewOpID + pEnumOp->Current().ID, this, pEnumOp->Current().Name.c_str());
			pFlowOps->AddCtrl((*BtnOps.rbegin())->GetSafeHwnd());
		}
	}
	
	//��ʼ��չʾ��
	GridViewer.Clear();
	GridViewer.Initialize(pView->GetViewColumnCollection());

	//��ʼ���༭��
	GridEditViewer.Initialize(pView->GetViewColumnCollection());
	GridEdit.SetRowCount(1);
	GridEdit.GetCell(0,0)->SetState(GVIS_READONLY);
	for (int iCol = 1; iCol < GridEdit.GetColumnCount(); ++iCol)
	{
		GridEdit.GetCell(0, iCol)->SetState(GridEdit.GetCell(0, iCol)->GetState() & ~GVIS_READONLY);
	}	

	RECT rect;
	this->GetClientRect(&rect);
	OnSize(0, rect.right, rect.bottom);

	return 0;
}

int	CDwarfView::RemoveRecordUpdated()
{
	if(RecordIdxUpdated < 0 || RecordIdxUpdated >= GridViewer.GetRecordCount())
	{
		return 0;
	}

	GridViewer.DelRecordAt(RecordIdxUpdated, 0);
	RecordIdxUpdated = -1;

	Grid.Invalidate();
	return 1;
}

int	CDwarfView::RemoveRecordFocused()
{
	if(RecordIdxFocused < 0 || RecordIdxFocused >= GridViewer.GetRecordCount())
	{
		return 0;
	}

	GridViewer.DelRecordAt(RecordIdxFocused, 0);
	RecordIdxFocused = -1;

	Grid.Invalidate();
	return 1;
}

int	CDwarfView::RemoveRecordSelected()
{
	for (int i = GridViewer.GetRecordCount() + GridViewer.HeadRowCount() - 1; i >= GridViewer.HeadRowCount(); --i)
	{
		if(CEditStyleBool::GetInstance().strTrue == Grid.GetCell(0, i)->GetText())
		{
			Grid.DeleteRow(i);
		}
	}

	Grid.Invalidate();
	
	return 1;
}

int	CDwarfView::AddRecord(const IDBRecord& rec)
{
	GridViewer.NewRecordAt(0, rec);

	Grid.Invalidate();
	return 1;
}

int CDwarfView::ShowRecords()
{
	IDwarfViewInfo* pView = CDwarfViewProvider::GetInstance()[ViewID];

	if(!pView)
	{
		_ASSERT(false);
		return 0;
	}

	std::auto_ptr<IEnumerator<IDBRecord>> pEnumRec(pView->EnumRecord());
	GridViewer.Fill(*pEnumRec);
	
	Grid.AutoSizeColumns();

	//ʹ����Grid�ĵ�Ԫ���ȱ���һ��
	for (int iCol = 0; iCol < Grid.GetColumnCount(); ++iCol)
	{
		GridEdit.SetColumnWidth(iCol, Grid.GetColumnWidth(iCol));
	}
	GridEdit.Invalidate();

	return 1;
}

IDBRecord* CDwarfView::GetFocusedRecord()
{
	int idx = GridViewer.GetCurRecord(&RecordFocused);
	return (idx >= 0) ? &RecordFocused : 0;
}

IEnumerator<IDBRecord>*	CDwarfView::GetSelectedRecords()
{
	return &SelectedRecords;
}

int CDwarfView::GetUpdatedRecord( IDBRecord* cur, IDBRecord* ori )
{
	if(cur)
	{
		GridEditViewer.GetRecordAt(0, cur);
	}

	if(ori)
	{
		if(RecordIdxUpdated < 0 || RecordIdxUpdated >= GridViewer.GetRecordCount())
		{
			MsgboxPrompt(TEXT("��ѡ˫����¼�����б༭��"));
			return -1;
		}
		DBRecordAssign(*ori, RecordUpdated);
	}

	return 1;
}

// CDwarfView ��Ϣ�������

void CDwarfView::CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle, UINT width, UINT height, DWORD dwStyle, CFont* pFont)
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

