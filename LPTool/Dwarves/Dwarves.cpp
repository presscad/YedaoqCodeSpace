
// Dwarves.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Dwarves.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DwarfDoc.h"
#include "DwarfView.h"
#include "DBInterface\DBSourcePath.h"
#include "DBInterface\DBDataAdapter.h"
//#include "DBNameMappingLP.h"
#include "SqliteNameMapping.h"
#include "SqliteSource.h"
#include "LocalPlayerObjEnum.h"
#include "CommandIDAlloter.h"
#include <Helper.h>
#include "DBModuleLP.h"

using namespace NSDBModule;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDwarvesApp

BEGIN_MESSAGE_MAP(CDwarvesApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDwarvesApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CDwarvesApp::OnFileOpen)
	ON_COMMAND_RANGE(MinMenuViewID, MaxMenuViewID, &CDwarvesApp::OnViewOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDwarvesApp ����

CDwarvesApp::CDwarvesApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDwarvesApp ����

CDwarvesApp theApp;


// CDwarvesApp ��ʼ��

BOOL CDwarvesApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_DwarvesTYPE,
		RUNTIME_CLASS(CDwarfDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CDwarfView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_DwarvesTYPE))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if(cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew)
	{
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//��ʼ��Ψһ���ĵ�����
	CDwarfDoc* pDoc = CGlobalData::GetCurrentDoc();
	pDoc->m_bAutoDelete = FALSE;
	pDoc->SetTitle(TEXT("��ѡ�����ݿ�"));
	pDoc->SetDBModule(&CDBModuleLP::GetInstance());
	pDocTemplate->AddDocument(pDoc);

	return TRUE;
}

CDwarfView* CDwarvesApp::GetExistedViewByID(int id)
{
	CDwarfDoc* pDoc = CGlobalData::GetCurrentDoc();
	POSITION pos = pDoc->GetFirstViewPosition();
	while(pos)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(!pView) continue;
		CDwarfView* pDwarfView = dynamic_cast<CDwarfView*>(pView);
		if(pDwarfView && pDwarfView->GetViewID() == id)
		{
			return pDwarfView;
		}
	}

	return 0;
}

CDwarfView* CDwarvesApp::NewView(int id)
{
	CDwarfDoc*		pDoc		= CGlobalData::GetCurrentDoc();
	CDocTemplate*	pTemplate	= pDoc->GetDocTemplate();
	
	ASSERT_VALID(pTemplate);

	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc, 0);
	if (pFrame == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
		return 0;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDoc);

	CDwarfView* pView = (CDwarfView*)pFrame->GetActiveView();
	if(pView)
	{
		TTRACE(TEXT("GetActiveView Successed~\r\n"));
		pView->SetViewID(id);
	}

	return pView;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CDwarvesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CDwarvesApp::OnFileOpen()
{
	CSqliteSourceManager smr;
	std::auto_ptr<IDBConnection> pConn(smr.PromptSelectDBSource(0));
	IDBDataAdapter* PAdapter;
	IDBFactory*		pFactory;
	IDBNameMapping* pMapping = new CSqliteNameMapping();

	if(!pConn.get() || !smr.OpenDBConnection(pConn.get(), &PAdapter, &pFactory))
	{
		return;
	}

	CDBModuleLP::GetInstance().AttachToDatabase(PAdapter, pFactory, pMapping);
	CDBModuleLP::GetInstance().RefreshData();

	CGlobalData::GetCurrentDoc()->SetTitle(pConn->ToString().c_str());

	RegDeleteKey(HKEY_CURRENT_USER, TEXT("Software\\Ӧ�ó��������ɵı���Ӧ�ó���\\Dwarves"));

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	pMainFrame->ClearDwarfViewmenuList();
	pMainFrame->OnSetMenu(pMainFrame->GetMenuBar()->GetHMenu());
}

void CDwarvesApp::OnViewOpen(UINT id)
{
	int viewID = id - MinMenuViewID;

	CDwarfView* pView = GetExistedViewByID(viewID);

	if(!pView)
	{
		pView = NewView(viewID);
	}
	else
	{
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		pMainFrame->MDIActivate(pView->GetParentFrame());

		//CMFCTabCtrl& tab = pMainFrame->GetMDITabs();
		//tab.SetActiveTab(tab.GetActiveTab());
		//tab.EnableActiveTabCloseButton(FALSE);

		/*CChildFrame* pChildFrame = static_cast<CChildFrame*>(pView->GetParentFrame());
		pChildFrame->SetActiveView(pView, TRUE);
		pChildFrame->MDIActivate();		*/
	}
}

// CDwarvesApp �Զ������/���淽��

void CDwarvesApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CDwarvesApp::LoadCustomState()
{
}

void CDwarvesApp::SaveCustomState()
{
}

// CDwarvesApp ��Ϣ�������



