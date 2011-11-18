#include "StdAfx.h"
#include "DwarfSideTab.h"
#include "DwarfViewInfo.h"
#include <Layout\Layout.h>
#include "DwarfViewProvider.h"

using namespace NSYedaoqLayout;

CDwarfSideTab::CDwarfSideTab(void)
	: FlowLayoutMain(EnumLayoutDirection::Vertical), GridViewer(&Grid, 1), View(0), ValidityCounter(-1),
	SelectedRecords(&Grid, &GridViewer)
{
}

CDwarfSideTab::~CDwarfSideTab(void)
{
}

BEGIN_MESSAGE_MAP(CDwarfSideTab, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDwarfSideTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect = {0, 0, 10, 10};
	Grid.Create(rect, this, EIDC_GRID);

	FlowLayoutMain.AddCtrl(Grid.GetSafeHwnd(), 
		ResizeInfo::FillInfo, 
		ResizeInfo::FillInfo,
		AnchorInfo(EnumAnchorMode::Front, 0),
		AnchorInfo(EnumAnchorMode::Front, 0));

	return 0;
}

void CDwarfSideTab::OnSize(UINT nType, int cx, int cy)
{
	//CDockablePane::OnSize(nType, cx, cy);

	// ѡ��ؼ�Ӧ��������������:
	FlowLayoutMain.Layout(NSYedaoqLayout::LayoutPoint(0,0), NSYedaoqLayout::LayoutSize(cx, cy));
}

int CDwarfSideTab::Initialize(CWnd* pParent, IDwarfViewInfo* pView)
{
	if(!pView) return -1;
	View = pView;

	RECT rect = {0,0,1,1};
	CWnd::Create(NULL, TEXT("TAB"), LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE, rect, pParent, pView->GetViewID());

	GridViewer.Initialize(pView->GetViewColumnCollection());

	return 1;
}

bool CDwarfSideTab::IsRelatedToMainView()
{
	return true;
}

int CDwarfSideTab::GetViewID() 
{
	if(View)
	{
		return View->GetViewID();
	}

	return -1;
}

int CDwarfSideTab::GetValidityCounter()
{
	return ValidityCounter;
}

int CDwarfSideTab::SetValidityCounter(int val)
{
	ValidityCounter = val;
	return 1;
}

int CDwarfSideTab::ContentUpdate(DwarfViewOperationContext* pCtx)
{
	std::auto_ptr<IEnumerator<IDBRecord>> pEnumRec(View->EnumRecordAsRelatedView(pCtx));
	if(pEnumRec.get())
	{
		GridViewer.Clear();
		GridViewer.Fill(*pEnumRec);
		Grid.AutoSizeColumns();
	}

	return 1;
}

IDBRecord* CDwarfSideTab::GetFocusedRecord()
{
	int idx = GridViewer.GetCurRecord(&FocusedRecord);
	return (idx >= 0) ? &FocusedRecord : 0;
}

IEnumerator<IDBRecord>*	CDwarfSideTab::GetSelectedRecords()
{
	return &SelectedRecords;
}