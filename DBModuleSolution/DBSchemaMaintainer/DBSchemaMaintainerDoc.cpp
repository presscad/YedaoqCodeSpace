
// DBSchemaMaintainerDoc.cpp : CDBSchemaMaintainerDoc ���ʵ��
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"
#include "Module\Schema\BuildInSchemaSerializer.h"
#include "DBCommon\DBNameMappingNone.h"
#include "SqliteSource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBSchemaMaintainerDoc

IMPLEMENT_DYNCREATE(CDBSchemaMaintainerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBSchemaMaintainerDoc, CDocument)
	ON_COMMAND(ID_FILE_OPENDB, &CDBSchemaMaintainerDoc::OnOpenDB)
END_MESSAGE_MAP()


// CDBSchemaMaintainerDoc ����/����

CDBSchemaMaintainerDoc::CDBSchemaMaintainerDoc()
{
	// TODO: �ڴ����һ���Թ������
	
}

CDBSchemaMaintainerDoc::~CDBSchemaMaintainerDoc()
{
}

int	CDBSchemaMaintainerDoc::LoadDBSchema(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping)
{
	DBModule_.Clear(true);

	if(DBModule_.AttachToDatabase(dbAdapter, dbFactory, dbNameMapping) < 1)
	{
		return 0;
	}

	return 1;
}

int CDBSchemaMaintainerDoc::LoadBuildInSchema(LPCTSTR lpszSchemaFile)
{
	if(!lpszSchemaFile)
	{
		TCHAR pszFilter[] = TEXT("Database Schema File(*.dbschema)|*.dbschema|All Files(*.*)|*.*|");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);
		if(dlg.DoModal() != IDOK)
		{
			return 0;
		}
		File_ = dlg.GetPathName();
	}
	else if(_taccess(lpszSchemaFile, 0) != 0)
	{
		_ASSERT(FALSE);
		return 0;
	}
	else
	{
		File_ = lpszSchemaFile;
	}

	CBuildInSchemaSerializer serializer; 
	CBuildInSchemaSerializer::FileRowCollection rows;

	//read file
	CStdioFile file(GetFile(), CStdioFile::modeWrite | CStdioFile::modeCreate);
	CString line;
	while(file.ReadString(line))
	{
		rows.push_back((LPCTSTR)line);
	}
	file.Close();

	// read schema
	DBModule_.Clear(true);
	serializer.Read(make_iterator_enumerator(rows.begin(), rows.end()), DBModule_);

	return 1;
}

int CDBSchemaMaintainerDoc::SaveBuildInSchema()
{
	if(GetFile().IsEmpty())
	{
		TCHAR pszFilter[] = TEXT("Database Schema File(*.dbschema)|*.dbschema|All Files(*.*)|*.*|");
		CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);
		if(dlg.DoModal() != IDOK)
		{
			return 0;
		}
		File_ = dlg.GetPathName();
	}

	CBuildInSchemaSerializer serializer; 
	CBuildInSchemaSerializer::FileRowCollection rows;
	serializer.Write(DBModule_, rows);

	CStdioFile file(GetFile(), CStdioFile::modeWrite | CStdioFile::modeCreate);

	for (CBuildInSchemaSerializer::FileRowCollection::iterator iter = rows.begin(); iter != rows.end(); ++iter)
	{
		file.WriteString(iter->c_str());
		file.WriteString(TEXT("\r\n"));
	}

	file.Close();

	return 1;
}

BOOL CDBSchemaMaintainerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CDBSchemaMaintainerDoc ���л�

void CDBSchemaMaintainerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CDBSchemaMaintainerDoc ���

#ifdef _DEBUG
void CDBSchemaMaintainerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBSchemaMaintainerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CDBSchemaMaintainerDoc ����
void CDBSchemaMaintainerDoc::OnOpenDB()
{
	CSqliteSourceManager sourceMgr;
	IDBConnection* pConn = sourceMgr.PromptSelectDBSource(0);
	if(!pConn)
	{
		return;
	}

	IDBDataAdapter* pAdapter;
	IDBFactory*		pFactory;
	IDBNameMapping* pMapping = new CDBNameMappingNone;

	sourceMgr.OpenDBConnection(pConn, &pAdapter, &pFactory);
	LoadDBSchema(pAdapter, pFactory, pMapping);

	UpdateAllViews(NULL);
}