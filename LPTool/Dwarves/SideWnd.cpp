
#include "stdafx.h"

#include "SideWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "SideTab.h"
#include "DwarfViewInfo.h"
#include "DwarfViewProvider.h"
#include "DwarfSideTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSideBar

CSideWnd::CSideWnd()
	: m_MainViewID(CDwarfViewProvider::InvalidViewID)
{
}

CSideWnd::~CSideWnd()
{
}

BEGIN_MESSAGE_MAP(CSideWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CSideWnd::OnTabActivate)
END_MESSAGE_MAP()

int CSideWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ����ѡ�����:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ������ѡ�����\n");
		return -1;      // δ�ܴ���
	}

	// �����������:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	//if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
	//	!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
	//	!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	//{
	//	TRACE0("δ�ܴ����������\n");
	//	return -1;      // δ�ܴ���
	//}

	//m_wndOutputBuild.SetFont(&m_Font);
	//m_wndOutputDebug.SetFont(&m_Font);
	//m_wndOutputFind.SetFont(&m_Font);

	CString strTabName;
	BOOL bNameValid;

	// ���б����ڸ��ӵ�ѡ�:
	//bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	return 0;
}

void CSideWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// ѡ��ؼ�Ӧ��������������:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CSideWnd::OnTabActivate(WPARAM wParam, LPARAM lParam)
{
	int idTabActive = m_wndTabs.GetActiveTab();
	ASSERT(idTabActive == (int)wParam);

	CWnd* pWndActive = m_wndTabs.GetActiveWnd();
	if(!pWndActive) return 0;

	ISideTab* pSideTab = static_cast<ISideTab*>(static_cast<CDwarfSideTab*>(pWndActive));
	
	if(pSideTab->GetValidityCounter() != m_ValidityCounter && m_MainViewID != CDwarfViewProvider::InvalidViewID)
	{
		pSideTab->ContentUpdate(m_MainViewID, &m_Context);
		pSideTab->SetValidityCounter(m_ValidityCounter);
	}
}

void CSideWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void CSideWnd::IncreaseValidityCounter()
{
	++m_ValidityCounter;
}

void CSideWnd::ShowRelatedTabsForView(int viewID)
{
	ClearTabs();

	IDwarfViewInfo* pView = CDwarfViewProvider::GetInstance()[viewID];

	if(!pView) return;

	std::auto_ptr<IEnumerator<IDwarfViewInfo*>> pEnumView(pView->EnumReleatedView());

	if(pEnumView.get()) return;

	while(pEnumView->MoveNext())
	{
		CDwarfSideTab* tab = GetDwarfSideTab(pEnumView->Current()->GetViewID());
		m_wndTabs.AddTab(tab, pEnumView->Current()->ToString().c_str());
	}
}

void CSideWnd::ClearTabs()
{
	m_wndTabs.RemoveAllTabs();
}

void CSideWnd::OnMainViewActivated(int mainView)
{
	m_MainViewID = mainView;
}

void CSideWnd::OnMainViewContextChanged(const DwarfViewOperationContext* pCtx)
{
	if(pCtx)
	{
		m_Context = *pCtx;
		OnTabActivate(0, 0);
	}
}

CDwarfSideTab* CSideWnd::GetDwarfSideTab(int view)
{
	DwarfSideTabMap::iterator iter = m_SideTabCache.find(view);
	if(m_SideTabCache.end() == iter)
	{
		iter = m_SideTabCache.insert(std::make_pair(view, new CDwarfSideTab())).first;
		iter->second->Initialize(this, CDwarfViewProvider::GetInstance()[view]);
	}
	return iter->second;
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ��Ϣ��������

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("�������"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("������"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

