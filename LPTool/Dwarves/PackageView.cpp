
// PackageView.cpp : CPackageView ���ʵ��
//

#include "stdafx.h"
#include "Dwarves.h"
#include "PackageDoc.h"
#include "PackageView.h"
#include "CommandIDAlloter.h"
#include "ChildFrm.h"
#include <memory>
#include "DwarfViewInfo.h"
#include "DwarfViewProvider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NSYedaoqLayout;

// CPackageView

IMPLEMENT_DYNCREATE(CPackageView, CView)

BEGIN_MESSAGE_MAP(CPackageView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPackageView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, MinViewOpID, MaxViewOpID, &CPackageView::OnViewOperation)
END_MESSAGE_MAP()

// CPackageView ����/����

CPackageView::CPackageView()
	: FlowLayoutMain(EnumLayoutDirection::Vertical), GridViewer(&Grid, 1)
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

int CPackageView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	RECT rect = {0, 0, lpcs->cx, lpcs->cy};
	Grid.Create(rect, this, EIDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	CreateButton(BtnSearch, EIDC_BTNSEARCH, this, TEXT("����"));

	CFlowLayout* pFlow = FlowLayoutMain.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(BtnSearch.GetSafeHwnd());

	FlowLayoutMain.AddCtrl(Grid.GetSafeHwnd(), ResizeInfo::FillInfo, ResizeInfo::FillInfo);

	return 0;
}

void CPackageView::OnSize(UINT nType, int cx, int cy)
{
	FlowLayoutMain.Layout(LayoutPoint(3,3), LayoutSize(cx - 6, cy - 6));
}

//void CPackageView::OnInitialUpdate()
//{
//	__super::OnInitialUpdate();
//	ResetTitle();
//}

void CPackageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	__super::OnUpdate(pSender, lHint, pHint);
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

void CPackageView::OnViewOperation(UINT id)
{
	int opID = id - MinViewOpID;
	
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

void CPackageView::SetViewID(int id)
{ 
	if(id < 0)
	{
		_ASSERT(false);
		return;
	}

	ViewID = id; 

	ResetTitle();
	Initialize();
	ShowRecords();
}

void CPackageView::ResetTitle()
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

int CPackageView::Initialize()
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
			CreateButton(**BtnOps.rbegin(), EIDC_UNUSED + pEnumOp->Current().ID, this, pEnumOp->Current().Name.c_str());
			pFlowOps->AddCtrl((*BtnOps.rbegin())->GetSafeHwnd());
		}
	}
	
	GridViewer.Clear();
	GridViewer.Initialize(pView->GetViewColumnCollection());

	RECT rect;
	this->GetClientRect(&rect);
	OnSize(0, rect.right, rect.bottom);

	return 0;
}

int CPackageView::ShowRecords()
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
}


// CPackageView ��Ϣ�������

void CPackageView::CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle, UINT width, UINT height, DWORD dwStyle, CFont* pFont)
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